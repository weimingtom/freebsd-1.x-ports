#ident	"@(#)events.c	26.50	93/06/28 SMI"

/*
 *      (c) Copyright 1989 Sun Microsystems, Inc.
 */

/*
 *      Sun design patents pending in the U.S. and foreign countries. See
 *      LEGAL_NOTICE file for terms of the license.
 */


#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "i18n.h"
#include "ollocale.h"
#include "olwm.h"
#include "win.h"
#include "debug.h"
#include "globals.h"
#include "events.h"
#include "list.h"
#include "mem.h"
#include "error.h"


/* ===== externs ========================================================== */

extern void	ReapChildren();


/* ===== globals ========================================================== */

Time LastEventTime = (Time) 0;
KeySym *KbdMap = NULL;		/* pointer to current key table; 2d array */
int MinKeyCode = 0;
int MaxKeyCode = 0;
int KeySymsPerKeyCode = 0;


/* ===== private data ===================================================== */

static InterposerFunc interposer = NULL;
static void *closure;
static List *deferredEventListHead = NULL_LIST;
static List *deferredEventListTail = NULL_LIST;
static Bool delegationEnabled = False;

static struct timeval timeoutNext = {0, 0};
static TimeoutFunc timeoutFunc = NULL;
static void *timeoutClosure = NULL;

static XModifierKeymap *ModMap = NULL;

static explicitPointerGrab = False;


/* ===== private functions ================================================ */

/*
 * lookupWindow
 * 
 * Given an event, look up the WinInfo corresponding to the window field of 
 * the event.  For events that have no window field, return NULL.
 */
WinGeneric *
lookupWindow( event )
    XEvent *event;
{
    switch ( event->xany.type ) {
    case KeymapNotify:
    case MappingNotify:
	/* these events have no window field */
	return NULL;
    default:
	return WIGetInfo( event->xany.window );
    }
}


/*
 * saveTimestamp
 *
 * Given an event, update LastEventTime with its timestamp.  For events that 
 * have no timestamp, do nothing.
 */
void
saveTimestamp( event )
    XEvent *event;
{
    switch ( event->xany.type ) {
    case KeyPress:
    case KeyRelease:
	LastEventTime = event->xkey.time;
	break;
    case ButtonPress:
    case ButtonRelease:
	LastEventTime = event->xbutton.time;
	break;
    case MotionNotify:
	LastEventTime = event->xmotion.time;
	break;
    case EnterNotify:
    case LeaveNotify:
	LastEventTime = event->xcrossing.time;
	break;
    case PropertyNotify:
	LastEventTime = event->xproperty.time;
	break;
    case SelectionClear:
	LastEventTime = event->xselectionclear.time;
	break;
    case SelectionRequest:
	/*
	 * The timestamp in SelectionRequest events comes from other clients; 
	 * it's not generated by the server.  It's thus not clear whether we 
	 * should believe any timestamp in one of these events.
	 */
	/* LastEventTime = event->xselectionrequest.time; */
	break;
    case SelectionNotify:
	/*
	 * Ignore SelectionNotify events generated by other clients.
	 * Save only the timestamps from server-generated events.
	 */
	if (!event->xany.send_event)
	    LastEventTime = event->xselection.time;
	break;
    default:
	break;
    }
}


/*
 * Dispatches an event normally (i.e. not through an interposer).  Looks up
 * the window structure from the event's window ID and calls the appropriate
 * event function for that window's class.  All extension events are handled
 * through a single extension event handler in the class.  If explicitGrab is
 * set, then someone has explicitly grabbed the pointer and redispatched the
 * event to a handler that is expecting an implicit grab, and so which won't
 * ungrab the pointer.  We must therefore do an explicit ungrab of the
 * pointer.
 */
static void
dispatchEvent(dpy, event, winInfo)
Display *dpy;
XEvent *event;
WinGeneric *winInfo;
{
	EvFunc evFunc;

	if (GRV.PrintAll)
	    DebugEvent(event, "Dispatch - debug");

	saveTimestamp( event );

	if (winInfo == NULL)
	{
		/* REMIND should we do anything with these events? */
	    	if (GRV.PrintOrphans)
	    	    DebugEvent(event, "Orphaned (null window)");

		/*
		 * If a client unmaps and then remaps its window, the
		 * MapRequest comes in on the frame window, but we just
		 * destroyed that window (in response to the unmap) so it
		 * appears as an orphan window.  Hence we'll just create a new
		 * frame/etc via StateNew().
		 *
		 * A similar situation applies to ConfigureRequest events.  If
		 * a client unmaps and immediately configures its window, the
		 * event will come in on the now-destroyed frame and will be
		 * orphaned.  We call ClientConfigure() to handle orphaned
		 * ConfigureRequest events.
		 */
		if (event->xany.type == MapRequest) {
		    StateNew(dpy,None,event->xmaprequest.window,False,NULL);
		} else if (event->xany.type == ConfigureRequest) {
		    ClientConfigure(NULL, NULL, event);
		}

		return;
	}

	if (GRV.PrintAll)
	    DebugWindow(winInfo);

#ifdef DEBUG
        if (winInfo->class == NULL)
	{
	    ErrorGeneral("Window instance with NULL class pointer");
	}

	if (winInfo->core.kind != winInfo->class->core.kind)
	    ErrorWarning("Warning:  window with kind different than class kind");
#endif 

	if (event->xany.type >= LASTEvent)
	    evFunc = WinClass(winInfo)->core.extEventHdlr;
	else
	    evFunc = WinClass(winInfo)->core.xevents[event->xany.type];

	if (evFunc != NULL)
	{
	    (*evFunc)(dpy, event, winInfo);
	}
	else
	    if (GRV.PrintOrphans)
		DebugEvent(event, "Orphaned");

	if (explicitPointerGrab && event->type == ButtonRelease &&
	    AllButtonsUp(event))
	{
	    XUngrabPointer(dpy, event->xbutton.time);
	}
}

static int
dispatchInterposer(dpy, event)
Display *dpy;
XEvent *event;
{
	int disposition;
	WinGeneric *winInfo;
	XEvent *temp;

	if (GRV.PrintAll)
	    DebugEvent(event, "Dispatch - interposer");

	saveTimestamp( event );
	winInfo = lookupWindow(event);
	disposition = (*interposer)(dpy, event, winInfo, closure);

	switch (disposition) {
	case DISPOSE_DISPATCH:
	    dispatchEvent(dpy, event, winInfo);
	    break;
	case DISPOSE_DEFER:
	    temp = MemNew(XEvent);
	    *temp = *event;
	    if (deferredEventListHead == NULL_LIST) {
		deferredEventListHead = ListCons(temp, NULL_LIST);
		deferredEventListTail = deferredEventListHead;
	    } else {
		deferredEventListTail->next = ListCons(temp, NULL_LIST);
		deferredEventListTail = deferredEventListTail->next;
	    }
	    break;
	case DISPOSE_USED:
	    /* do nothing */
	    break;
	}
}

static void
doTimeout()
{
	TimeoutFunc f;
	void *closure;

	f = timeoutFunc;
	closure = timeoutClosure;
	TimeoutCancel();
	(*f)(closure);
}

static void
nextEventOrTimeout(dpy, event)
Display *dpy;
XEvent *event;
{
	int fd = ConnectionNumber(dpy);
	struct timeval polltime;
	fd_set rdset, wrset, xset;
	int ready = -1;

	while (XPending(dpy) == 0 && ready <= 0 && timeoutFunc != NULL) {
	    gettimeofday(&polltime,NULL);
	    if ((timeoutFunc != NULL) &&
		((polltime.tv_sec > timeoutNext.tv_sec) ||
		 ((polltime.tv_sec == timeoutNext.tv_sec) && 
		  (polltime.tv_usec >= timeoutNext.tv_usec))))
	    {
		doTimeout();
		continue;
	    }

	    polltime.tv_sec = timeoutNext.tv_sec - polltime.tv_sec;
	    polltime.tv_usec = 0;

	    FD_ZERO(&rdset);
	    FD_SET(fd,&rdset);
	    FD_ZERO(&wrset);
	    FD_ZERO(&xset);
	    FD_SET(fd,&xset);

	    ready = select(fd+1,&rdset,&wrset,&xset,&polltime);

	    gettimeofday(&polltime,NULL);
	    if ((timeoutFunc != NULL) &&
		((polltime.tv_sec > timeoutNext.tv_sec) ||
		 ((polltime.tv_sec == timeoutNext.tv_sec) && 
		  (polltime.tv_usec >= timeoutNext.tv_usec))))
	    {
		doTimeout();
	    }
	}

	XNextEvent(dpy, event);
}


/*
 * Update our own modifier map.  This is unfortunate, as we know Xlib already 
 * keeps this information around.
 */
static void
updateModifierMap(dpy)
    Display *dpy;
{
    if (ModMap != NULL)
	XFreeModifiermap(ModMap);
    ModMap = XGetModifierMapping(dpy);
}


/*
 * Update our own keyboard table.  This is unfortunate, as we know Xlib 
 * already keeps this information around.
 */
static void
updateKeyboardMap(dpy, e)
    Display *dpy;
    XEvent *e;
{
    if (KbdMap != NULL )
	XFree((char *)KbdMap);
    XDisplayKeycodes(dpy, &MinKeyCode, &MaxKeyCode);
    KbdMap = XGetKeyboardMapping(dpy, MinKeyCode, MaxKeyCode-MinKeyCode+1,
				 &KeySymsPerKeyCode);
}


/*
 * Handle MappingNotify events.  Refresh cached information about the keyboard 
 * and the pointer.
 */
void
handleMappingNotify(dpy, e)
    Display *dpy;
    XEvent *e;
{
    XMappingEvent	*mapEvent = (XMappingEvent *)e;
    extern XrmDatabase	OlwmDB;

    switch (mapEvent->request) {
    case MappingModifier:
	XRefreshKeyboardMapping(mapEvent);
	updateModifierMap(dpy);
	RefreshKeyGrabs(dpy, OlwmDB);
	RefreshButtonGrabs(dpy);
	break;
    case MappingKeyboard:
	XRefreshKeyboardMapping(mapEvent);
	updateKeyboardMap(dpy);
	RefreshKeyGrabs(dpy, OlwmDB);
	break;
    case MappingPointer:
	/* no need to handle pointer mapping changes */
	break;
    }
}


/*ARGSUSED*/
static void *
redispatchEvent(e,c)
    XEvent *e;
    void   *c;
{
    dispatchEvent(e->xany.display, e, lookupWindow(e));
    MemFree(e);
    return (void *)NULL;
}


/*
 * Compute t2 - t1 and return the time value in diff.  The tv_usec field is 
 * always in the range 0..999999.  This means that negative time values always 
 * have tv_sec less than zero.  For instance, negative one-half seconds is 
 * represented with tv_sec == -1 and tv_usec == 500000.
 */
static void
tvdiff(t1, t2, diff)
    struct timeval *t1, *t2, *diff;
{
    diff->tv_sec = t2->tv_sec - t1->tv_sec;
    diff->tv_usec = t2->tv_usec - t1->tv_usec;
    if (diff->tv_usec < 0) {
	diff->tv_sec -= 1;
	diff->tv_usec += 1000000;
    }
}


/* ===== public functions ================================================= */


/*
 * EventLoop
 * 
 * The main event loop.  Reads events from the wire and dispatches them.
 */
void
EventLoop( dpy )
    Display *dpy;
{
	XEvent		event;

	for (;;) {
		if (timeoutFunc == NULL)
		{
		    XNextEvent( dpy, &event );
		} 
		else
		{
		    nextEventOrTimeout(dpy, &event);
		}

		ReapChildren();

		/*
		 * Discard user events that have the Synthetic bit set.
		 *
		 * All device events (mouse and keyboard events) have types
		 * that fall in between KeyPress and MotionNotify.
		 */
		if (event.xany.send_event
			&& event.type <= MotionNotify
			&& event.type >= KeyPress)
		    continue;


		/*
		 * Handle MappingNotify events.  These events don't have 
		 * window field, so they can't be dispatched normally.
		 */
		if (event.xany.type == MappingNotify) {
		    handleMappingNotify(dpy, &event);
		    continue;
		}

		/*
		 * If there is an event interposer, and it returns True, that 
		 * means it has successfully processed the event.  We continue 
		 * around the loop instead of processing the event normally.
		 */
		if ( interposer != NULL)
		    dispatchInterposer(dpy, &event);
		else
		    dispatchEvent(dpy, &event, lookupWindow(&event));
	}

	/*NOTREACHED*/
}


/*
 * Propagate an event to this window's parent.  REMIND: doesn't update the 
 * event fields or the event coordinates.
 */
int
PropagateEventToParent(dpy,event,win)
Display *dpy;
XEvent *event;
WinGeneric *win;
{
	dispatchEvent(dpy,event,win->core.parent);
}


/*
 * Propagate a ButtonPress event to a child window.  Win is assumed to be a
 * child of the event window.  The event window and subwindow fields are
 * updated, and the coordinates are translated to the child's coordinate
 * system.  Sets the explicitPointerGrab flag.
 */
void
PropagatePressEventToChild(dpy, event, win)
    Display *dpy;
    XButtonPressedEvent *event;
    WinGeneric *win;
{
    event->window = win->core.self;
    event->subwindow = None;
    event->x -= win->core.x;
    event->y -= win->core.y;
    dispatchEvent(dpy, event, win);
    explicitPointerGrab = True;
}


/*
 * FindModifierMask
 *
 * Given a keycode, look in the modifier mapping table to see if this keycode 
 * is a modifier.  If it is, return the modifier mask bit for this key; 
 * otherwise, return zero.
 */
unsigned int
FindModifierMask(kc)
KeyCode	kc;
{
    int i, j;
    KeyCode *mapentry;

    if (ModMap == NULL || kc == 0)
	return 0;

    mapentry = ModMap->modifiermap;
    for (i=0; i<8; ++i) {
	for (j=0; j<(ModMap->max_keypermod); ++j) {
	    if (kc == *mapentry)
		return 1 <<
		    ( (mapentry - ModMap->modifiermap) / 
		      ModMap->max_keypermod );
	    ++mapentry;
	}
    }
    return 0;
}


/*
 * ModifierToKeysym
 *
 * Given a modifier number (not a mask) returns a keysym for it that occurs in 
 * the modifier map.  If there is no such modifier key, returns NoSymbol.
 */
KeySym
ModifierToKeysym(mod)
    unsigned int	mod;
{
    KeyCode		kc;
    extern Display	*DefDpy;

    kc = ModMap->modifiermap[mod * ModMap->max_keypermod];
    if (kc == 0)
	return NoSymbol;
    return(XKeycodeToKeysym(DefDpy, kc, 0));
}
 

/*
 * Wait on dpy for some events to come in or for a timeout to occur.  If
 * events come in, return True and change timeout to indicate the amount of
 * time remaining.  If no events come in before the timeout expires, return
 * False.  A negative timestamp is considered to have timed out immediately.
 */
Bool
AwaitEvents(dpy, timeout)
    Display *dpy;
    struct timeval *timeout;
{
    struct timeval starttime, curtime, diff1, diff2;
    fd_set rfds;
    int s;

    if (timeout->tv_sec < 0)
	return False;

    (void) gettimeofday(&starttime, NULL);

    while (1) {
	FD_ZERO(&rfds);
	FD_SET(ConnectionNumber(dpy), &rfds);
	s = select(ConnectionNumber(dpy)+1, &rfds, NULL, NULL, timeout);
	if (s == 0) {
	    /* we timed out without getting anything */
	    return False;
	}

	/*
	 * If an error occurred, report it and return False.
	 */
	if (s == -1 && errno != EINTR) {
	    perror("select");
#ifdef DEBUG
	    fputs("olwm: generating core dump for analysis...\n", stderr);
	    if (fork() == 0) {
		abort();
		exit(1);
	    }
#endif
	    return False;
	}

	/*
	 * Either we got interrupted or the descriptor became ready, or both.
	 * Compute the remaining time on the timeout.  This can be negative, 
	 * because there is a slight window for delays between the select() 
	 * call above and this gettimeofday() call.  This means that we can 
	 * return an indication of valid data to the caller, yet also return
	 * a value for the time remaining that is less than or equal to zero.
	 */
	(void) gettimeofday(&curtime, NULL);
	tvdiff(&starttime, &curtime, &diff1);
	tvdiff(&diff1, timeout, &diff2);
	*timeout = diff2;
	starttime = curtime;

	/*
	 * If we got some data, return True.  Otherwise, we were interrupted.
	 * If we timed out, return False.  If not, there is time remaining;
	 * continue around the loop.
	 */
	if (s > 0)
	    return True;

	if (timeout->tv_sec < 0)
	    return False;
    }
}
    

/***************************************************************************
* Interposer functions
***************************************************************************/

/*
 * InstallInterposer
 *
 * Install an event interposition function.  Nested interposition is illegal.  
 * If interposer delegation has been enabled, a new interposer will replace 
 * the current one without nesting.
 */
void
InstallInterposer(func, cl)
    InterposerFunc func;
    void *cl;
{
    if (interposer != NULL && !delegationEnabled) {
	fputs(GetString("olwm: warning, nested event interposer!\n"), stderr);
#ifdef DEBUG
	abort();
#endif /* DEBUG */
    }
    interposer = func;
    closure = cl;
    delegationEnabled = False;
}

/*
 * UninstallInterposer
 *
 * Uninstalls an event interposition function.  Does nothing if there is
 * currently no interposer.  Replays any queued events.  Inhibits focus
 * changes and colormap installation while replaying them.  This prevents 
 * redundant focus changes and colormap installation.
 */
void
UninstallInterposer()
{
    interposer = (InterposerFunc) NULL;
    delegationEnabled = False;

    if (deferredEventListHead != NULL) {
	ClientInhibitFocus(True);
	ColormapInhibit(True);
	ListApply(deferredEventListHead, redispatchEvent, 0);
	ListDestroy(deferredEventListHead);
	deferredEventListHead = deferredEventListTail = NULL_LIST;
	ClientInhibitFocus(False);
	ColormapInhibit(False);
    }
}


/*
 * Return the current interposer function.
 */
InterposerFunc
InterposerInstalled()
{
    return interposer;
}


/*
 * Enable delegation of one interposer to another.  This is used when one 
 * interposer wants to transfer the interposition to another interposer 
 * without replaying queued events.
 */
void
EnableInterposerDelegation()
{
    delegationEnabled = True;
}


/***************************************************************************
* Timeout functions
***************************************************************************/

/* TimeoutRequest(t,f,c) -- request that a timeout be generated t microseconds
 *	in the future; when the timeout occurs, the function f is called
 *	with the closure c.
 */
void
TimeoutRequest(t,f,c)
int t;
TimeoutFunc f;
void *c;
{
#ifdef DEBUG
	if (timeoutFunc != NULL)
	{
	    ErrorWarning("Timeout being set while another timeout current");
	}
#endif

	gettimeofday(&timeoutNext,NULL);
	timeoutNext.tv_sec += t / 1000000;
	timeoutNext.tv_usec += t % 1000000;
	if (timeoutNext.tv_usec >= 1000000) {
	    timeoutNext.tv_usec -= 1000000;
	    timeoutNext.tv_sec += 1;
	}
	timeoutFunc = f;
	timeoutClosure = c;
}

/* TimeoutCancel() -- cancel an outstanding timeout.
 */
void 
TimeoutCancel()
{
	timeoutNext.tv_sec = 0;
	timeoutNext.tv_usec = 0;
	timeoutFunc = NULL;
	timeoutClosure = NULL;
}


/*
 * Initialize the event handling system.  This function is called exactly
 * once at startup.
 */
void
InitEvents(dpy)
    Display *dpy;
{
    updateKeyboardMap(dpy);
    updateModifierMap(dpy);
}
