/*
 * GDI bit-blit operations
 *
 * Copyright 1993 Alexandre Julliard
 */

static char Copyright[] = "Copyright  Alexandre Julliard, 1993";

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#include "gdi.h"

extern const int DC_XROPfunction[];

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))


/***********************************************************************
 *           PatBlt    (GDI.29)
 */
BOOL PatBlt( HDC hdc, short left, short top,
	     short width, short height, DWORD rop)
{
    int x1, x2, y1, y2;
    
    DC * dc = (DC *) GDI_GetObjPtr( hdc, DC_MAGIC );
    if (!dc) return FALSE;
#ifdef DEBUG_GDI
    printf( "PatBlt: %d %d,%d %dx%d %06x\n",
	    hdc, left, top, width, height, rop );
#endif

    rop >>= 16;
    if (!DC_SetupGCForBrush( dc )) rop &= 0x0f;
    else rop = (rop & 0x03) | ((rop >> 4) & 0x0c);
    XSetFunction( XT_display, dc->u.x.gc, DC_XROPfunction[rop] );

    x1 = dc->w.DCOrgX + XLPTODP( dc, left );
    x2 = dc->w.DCOrgX + XLPTODP( dc, left + width );
    y1 = dc->w.DCOrgY + YLPTODP( dc, top );
    y2 = dc->w.DCOrgY + YLPTODP( dc, top + height );
    XFillRectangle( XT_display, dc->u.x.drawable, dc->u.x.gc,
		   MIN(x1,x2), MIN(y1,y2), abs(x2-x1), abs(y2-y1) );
    return TRUE;
}


/***********************************************************************
 *           BitBlt    (GDI.34)
 */
BOOL BitBlt( HDC hdcDest, short xDest, short yDest, short width, short height,
	     HDC hdcSrc, short xSrc, short ySrc, DWORD rop )
{
    int xs1, xs2, ys1, ys2;
    int xd1, xd2, yd1, yd2;
    DC *dcDest, *dcSrc;

#ifdef DEBUG_GDI    
    printf( "BitBlt: %d %d,%d %dx%d %d %d,%d %08x\n",
	   hdcDest, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, rop );
#endif

    if ((rop & 0xcc0000) == ((rop & 0x330000) << 2))
	return PatBlt( hdcDest, xDest, yDest, width, height, rop );

    rop >>= 16;
    if ((rop & 0x0f) != (rop >> 4))
    {
	printf( "BitBlt: Unimplemented ROP %02x\n", rop );
	return FALSE;
    }
    
    dcDest = (DC *) GDI_GetObjPtr( hdcDest, DC_MAGIC );
    if (!dcDest) return FALSE;
    dcSrc = (DC *) GDI_GetObjPtr( hdcSrc, DC_MAGIC );
    if (!dcSrc) return FALSE;

    xs1 = dcSrc->w.DCOrgX + XLPTODP( dcSrc, xSrc );
    xs2 = dcSrc->w.DCOrgX + XLPTODP( dcSrc, xSrc + width );
    ys1 = dcSrc->w.DCOrgY + YLPTODP( dcSrc, ySrc );
    ys2 = dcSrc->w.DCOrgY + YLPTODP( dcSrc, ySrc + height );
    xd1 = dcDest->w.DCOrgX + XLPTODP( dcDest, xDest );
    xd2 = dcDest->w.DCOrgX + XLPTODP( dcDest, xDest + width );
    yd1 = dcDest->w.DCOrgY + YLPTODP( dcDest, yDest );
    yd2 = dcDest->w.DCOrgY + YLPTODP( dcDest, yDest + height );

    if ((abs(xs2-xs1) != abs(xd2-xd1)) || (abs(ys2-ys1) != abs(yd2-yd1)))
	return FALSE;  /* Should call StretchBlt here */
    
    DC_SetupGCForText( dcDest );
    XSetFunction( XT_display, dcDest->u.x.gc, DC_XROPfunction[rop & 0x0f] );
    if (dcSrc->w.bitsPerPixel == dcDest->w.bitsPerPixel)
    {
	XCopyArea( XT_display, dcSrc->u.x.drawable,
		   dcDest->u.x.drawable, dcDest->u.x.gc,
		   MIN(xs1,xs2), MIN(ys1,ys2), abs(xs2-xs1), abs(ys2-ys1),
		   MIN(xd1,xd2), MIN(yd1,yd2) );
    }
    else
    {
	if (dcSrc->w.bitsPerPixel != 1) return FALSE;
	XCopyPlane( XT_display, dcSrc->u.x.drawable,
		    dcDest->u.x.drawable, dcDest->u.x.gc,
		    MIN(xs1,xs2), MIN(ys1,ys2), abs(xs2-xs1), abs(ys2-ys1),
		    MIN(xd1,xd2), MIN(yd1,yd2), 1 );
    }
    return TRUE;
}



/***********************************************************************
 *           StretchBlt    (GDI.35)
 * 
 * 	o StretchBlt is CPU intensive so we only call it if we have
 *        to.  Checks are made to see if we can call BitBlt instead.
 *
 * 	o the stretching is slowish, some integer interpolation would
 *        speed it up.
 *
 *      o a this point stretch mode isn't used when compressing
 *        bitmaps this results in a color copy all the time
 *
 */
BOOL StretchBlt( HDC hdcDest, short xDest, short yDest, short widthDest, short heightDest,
               HDC hdcSrc, short xSrc, short ySrc, short widthSrc, short heightSrc, DWORD rop )
{
    int xs1, xs2, ys1, ys2;
    int xd1, xd2, yd1, yd2;
    DC *dcDest, *dcSrc;
    XImage *sxi, *dxi;
    register int x, y; 
    float deltax, deltay, sourcex, sourcey;

#ifdef DEBUG_GDI     
    printf( "StretchBlt: %d %d,%d %dx%d %d %d,%d %dx%d %08x\n",
           hdcDest, xDest, yDest, widthDest, heightDest, hdcSrc, xSrc, 
           ySrc, widthSrc, heightSrc, rop );
    printf("StretchMode is %x\n", 
	((DC *)GDI_GetObjPtr(hdcDest, DC_MAGIC))->w.stretchBltMode);
#endif 

    if ((rop & 0xcc0000) == ((rop & 0x330000) << 2))
        return PatBlt( hdcDest, xDest, yDest, widthDest, heightDest, rop );

    /* don't stretch the bitmap unless we have to; if we don't,
     * call BitBlt for a performance boost
     */

    if (widthSrc == widthDest && heightSrc == heightDest) {
	return BitBlt(hdcDest, xDest, yDest, widthSrc, heightSrc,
               hdcSrc, xSrc, ySrc, rop);
    }

    rop >>= 16;
    if ((rop & 0x0f) != (rop >> 4))
    {
        printf( "StretchBlt: Unimplemented ROP %02x\n", rop );
        return FALSE;
    }

    dcDest = (DC *) GDI_GetObjPtr( hdcDest, DC_MAGIC );
    if (!dcDest) return FALSE;
    dcSrc = (DC *) GDI_GetObjPtr( hdcSrc, DC_MAGIC );
    if (!dcSrc) return FALSE;

    xs1 = dcSrc->w.DCOrgX + XLPTODP( dcSrc, xSrc );
    xs2 = dcSrc->w.DCOrgX + XLPTODP( dcSrc, xSrc + widthSrc );
    ys1 = dcSrc->w.DCOrgY + YLPTODP( dcSrc, ySrc );
    ys2 = dcSrc->w.DCOrgY + YLPTODP( dcSrc, ySrc + heightSrc );
    xd1 = dcDest->w.DCOrgX + XLPTODP( dcDest, xDest );
    xd2 = dcDest->w.DCOrgX + XLPTODP( dcDest, xDest + widthDest );
    yd1 = dcDest->w.DCOrgY + YLPTODP( dcDest, yDest );
    yd2 = dcDest->w.DCOrgY + YLPTODP( dcDest, yDest + heightDest );


    /* get a source and destination image so we can manipulate
     * the pixels
     */

    sxi = XGetImage(display, dcSrc->u.x.drawable, xs1, ys1, 
	     widthSrc, heightSrc, AllPlanes, ZPixmap);
    dxi = XCreateImage(display, DefaultVisualOfScreen(screen),
	  		    DefaultDepthOfScreen(screen), ZPixmap, 
			    0, NULL, widthDest, heightDest,
			    32, 0);
    dxi->data = malloc(dxi->bytes_per_line * heightDest);


     /* the actual stretching is done here, we'll try to use
      * some interolation to get some speed out of it in
      * the future
      */

    deltax = (float)widthSrc/widthDest;
    deltay = (float)heightSrc/heightDest;

    for (x=0, sourcex=0.0; x<widthDest; x++, sourcex+=deltax) 
        for (y=0, sourcey=0.0; y<heightDest; y++, sourcey+=deltay)
	    XPutPixel(dxi, x, y, XGetPixel(sxi, (int)sourcex, (int)sourcey));

    DC_SetupGCForText(dcDest);
    XSetFunction(display, dcDest->u.x.gc, DC_XROPfunction[rop & 0x0f]);
    XPutImage(display, dcDest->u.x.drawable, dcDest->u.x.gc,
	 	dxi, 0, 0, MIN(xd1,xd2), MIN(yd1,yd2), 
		widthDest, heightDest);

    /* now free the images we created */

    XDestroyImage(sxi);
    XDestroyImage(dxi);

    return TRUE;
}
