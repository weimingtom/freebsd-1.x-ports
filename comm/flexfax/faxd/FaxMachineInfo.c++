/*	$Header: /a/cvs/386BSD/ports/comm/flexfax/faxd/FaxMachineInfo.c++,v 1.1 1993/08/31 23:42:16 ljo Exp $
/*
 * Copyright (c) 1990, 1991, 1992, 1993 Sam Leffler
 * Copyright (c) 1991, 1992, 1993 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */
#include <ctype.h>
#include <osfcn.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#include "config.h"
#include "FaxMachineInfo.h"
#include "FaxTrace.h"
#include "class2.h"

extern "C" void syslog(int priority, const char *message, ...);	// XXX
static	fxBool getBoolean(const char* cp);
static	fxBool getLine(FILE*, char* line, int cc, char*& tag, char*& val);
static	int getNum(const char* s);

const fxStr FaxMachineCtlInfo::ctlDir(FAX_CTLDIR);

FaxMachineCtlInfo::FaxMachineCtlInfo()
{
    setDefaults();
}
FaxMachineCtlInfo::~FaxMachineCtlInfo() {}

void
FaxMachineCtlInfo::setDefaults()
{
    rejectNotice = "";
    tracingLevel = -1;
}

#define	isCmd(s,cmd)	(strcasecmp(s, cmd) == 0)

void
FaxMachineCtlInfo::restore(const fxStr& canon)
{
    setDefaults();
    mode_t omask = umask(022);
    FILE* fp = fopen(ctlDir | "/" | canon, "r");
    (void) umask(omask);
    if (fp) {
	char line[1024];
	char* tag;
	char* val;

	while (getLine(fp, line, sizeof (line), tag, val)) {
	    if (isCmd(tag, "rejectNotice")) {
		rejectNotice = val;
	    } else if (isCmd(tag, "tracingLevel")) {
		tracingLevel = getNum(val) & FAXTRACE_MASK;
	    }
	}
	fclose(fp);
    }
}

fxBool
FaxMachineCtlInfo::getTracingLevel(int& l) const
{
    if (tracingLevel != -1) {
	l = tracingLevel;
	return (TRUE);
    } else
	return (FALSE);
}

const fxStr FaxMachineInfo::infoDir(FAX_INFODIR);

FaxMachineInfo::FaxMachineInfo()
{
    setDefaults();
}
FaxMachineInfo::FaxMachineInfo(const fxStr& number)
{
    restore(number);
}
FaxMachineInfo::~FaxMachineInfo() {}

void
FaxMachineInfo::setDefaults()
{
    locked = 0;
    supportsHighRes = TRUE;
    supports2DEncoding = TRUE;
    supportsPostScript = FALSE;
    calledBefore = FALSE;
    maxPageWidth = 2432;		// 1728 at 303 mm
    maxPageLength = -1;			// unlimited
    maxSignallingRate = BR_14400;	// T.17 14.4KB
    changed = FALSE;
    csi = "";
}

int
FaxMachineInfo::operator==(const FaxMachineInfo& other) const
{
    return (
	supportsHighRes == other.supportsHighRes
     && supports2DEncoding == other.supports2DEncoding
     && supportsPostScript == other.supportsPostScript
     && calledBefore == other.calledBefore
     && maxPageWidth == other.maxPageWidth
     && maxPageLength == other.maxPageLength
     && maxSignallingRate == other.maxSignallingRate
     && csi == other.csi
    );
}

int
FaxMachineInfo::operator!=(const FaxMachineInfo& other) const
    { return !(*this == other); }

FILE*
FaxMachineInfo::openInfoFile(const fxStr& number, const char* mode)
{
    fxStr canon(number);
    for (int i = canon.length()-1; i >= 0; i--)
	if (!isdigit(canon[i]))
	    canon.remove(i,1);
    mode_t omask = umask(022);
    FILE* fp = fopen(infoDir | "/" | canon, mode);
    (void) umask(omask);
    return (fp);
}

void
FaxMachineInfo::restore(const fxStr& number)
{
    FaxMachineCtlInfo::restore(number);

    setDefaults();
    FILE* fp = openInfoFile(number, "r");
    if (fp) {
	restore(fp);
	fclose(fp);
    }
}

static fxBool
getBoolean(const char* cp)
{
    return (strcasecmp(cp, "on") == 0 || strcasecmp(cp, "yes") == 0);
}

static int
getNum(const char* s)
{
    return ((int) strtol(s, NULL, 0));
}

static fxBool
getLine(FILE* fp, char* line, int n, char*& tag, char*& val)
{
    while (fgets(line, n-1, fp)) {
	if (line[0] == '#')
	    continue;
	char* cp = strchr(line, '\n');
	if (cp)
	    *cp = '\0';
	val = strchr(line, ':');
	if (!val)
	    continue;
	*val++ = '\0';
	while (isspace(*val))
	    val++;
	tag = line;
	return (TRUE);
    }
    return (FALSE);
}

#define	HIRES	0
#define	G32D	1
#define	PS	2
#define	WD	3
#define	LN	4
#define	BR	5

static const char* brnames[] =
   { "2400", "4800", "7200", "9600", "12000", "14400" };
#define	NBR	(sizeof (brnames) / sizeof (brnames[0]))

void
FaxMachineInfo::restore(FILE* fp)
{
    char line[1024];
    char* tag;
    char* val;

    locked = 0;
    while (getLine(fp, line, sizeof (line), tag, val)) {
	int b;
	if (line[0] == '&') {			// locked down indicator
	    b = 1;
	    tag++;
	} else
	    b = 0;
	if (isCmd(tag, "supportsHighRes")) {
	    supportsHighRes = getBoolean(val);
	    locked |= (b<<HIRES);
	} else if (isCmd(tag, "supports2DEncoding")) {
	    supports2DEncoding = getBoolean(val);
	    locked |= (b<<G32D);
	} else if (isCmd(tag, "supportsPostScript")) {
	    supportsPostScript = getBoolean(val);
	    locked |= (b<<PS);
	} else if (isCmd(tag, "calledBefore")) {
	    calledBefore = getBoolean(val);
	} else if (isCmd(tag, "maxPageWidth")) {
	    maxPageWidth = atoi(val);
	    locked |= (b<<WD);
	} else if (isCmd(tag, "maxPageLength")) {
	    maxPageLength = atoi(val);
	    locked |= (b<<LN);
	} else if (isCmd(tag, "maxSignallingRate")) {
	    for (u_int i = 0; i < NBR; i++)
		if (strcmp(brnames[i], val) == 0) {
		    maxSignallingRate = i;
		    locked |= (b<<BR);
		    break;
		}
	} else if (isCmd(tag, "remoteCSI")) {
	    csi = val;
	}
    }
    changed = FALSE;
}

#define	isLocked(b)	(locked & (1<<b))

void
FaxMachineInfo::setSupportsHighRes(fxBool b)
{
    if (!isLocked(HIRES)) {
	 supportsHighRes = b;
	 changed = TRUE;
    }
}

void
FaxMachineInfo::setSupports2DEncoding(fxBool b)
{
    if (!isLocked(G32D)) {
	supports2DEncoding = b;
	changed = TRUE;
    }
}

void
FaxMachineInfo::setSupportsPostScript(fxBool b)
{
    if (!isLocked(PS)) {
	supportsPostScript = b;
	changed = TRUE;
    }
}

void
FaxMachineInfo::setCalledBefore(fxBool b)
{
    calledBefore = b;
    changed = TRUE;
}

void
FaxMachineInfo::setMaxPageWidth(int v)
{
    if (!isLocked(WD)) {
	maxPageWidth = v;
	changed = TRUE;
    }
}

void
FaxMachineInfo::setMaxPageLength(int v)
{
    if (!isLocked(LN)) {
	maxPageLength = v;
	changed = TRUE;
    }
}

void
FaxMachineInfo::setMaxSignallingRate(int v)
{
    if (!isLocked(BR)) {
	maxSignallingRate = v;
	changed = TRUE;
    }
}

void
FaxMachineInfo::setCSI(const fxStr& v)
{
    csi = v;
    changed = TRUE;
}

static void
putBoolean(FILE* fp, const char* tag, fxBool locked, fxBool b)
{
    fprintf(fp, "%s%s: %s\n", locked ? "&" : "", tag, b ? "yes" : "no");
}

static void
putDecimal(FILE* fp, const char* tag, fxBool locked, int v)
{
    fprintf(fp, "%s%s: %d\n", locked ? "&" : "", tag, v);
}

static void
putString(FILE* fp, const char* tag, fxBool locked, const char* v)
{
    fprintf(fp, "%s%s: %s\n", locked ? "&" : "", tag, v);
}

void
FaxMachineInfo::update(const fxStr& number)
{
    if (!changed)
	return;
    FILE* fp = openInfoFile(number, "w");
    if (fp) {
	putBoolean(fp, "supportsHighRes", isLocked(HIRES), supportsHighRes);
	putBoolean(fp, "supports2DEncoding", isLocked(G32D),supports2DEncoding);
	putBoolean(fp, "supportsPostScript", isLocked(PS), supportsPostScript);
	putBoolean(fp, "calledBefore", FALSE, calledBefore);
	putDecimal(fp, "maxPageWidth", isLocked(WD), maxPageWidth);
	putDecimal(fp, "maxPageLength", isLocked(LN), maxPageLength);
	putString(fp, "maxSignallingRate", isLocked(BR),
	    brnames[fxmin(maxSignallingRate, BR_14400)]);
	putString(fp, "remoteCSI", FALSE, csi);
	fclose(fp);
	changed = FALSE;
    } else
	syslog(LOG_ERR, "Can not save machine capabilities for \"%s\"",
	    (char*) number);
}
