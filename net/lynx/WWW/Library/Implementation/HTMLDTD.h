/*                                               The HTML DTD -- software interface in libwww
                              HTML DTD - SOFTWARE INTERFACE
                                             
   SGML purists should excuse the use of the term "DTD" in this file to represent
   DTD-related information which is not exactly a DTD itself.
   
   The C modular structure doesn't work very well here, as the dtd is partly in the .h and
   partly in the .c which are not very independent.  Tant pis.
   
 */
#ifndef HTMLDTD_H
#define HTMLDTD_H

#include "HTUtils.h"
#include "SGML.h"

/*

Element Numbers

 */

/*

   Must Match all tables by element! These include tables in HTMLDTD.c and code in HTML.c
   .
   
 */
typedef enum _HTMLElement {
        HTML_A,         HTML_ADDRESS,
        HTML_B,         HTML_BLOCKQUOTE,
	HTML_BR,
        HTML_CITE,      HTML_CODE,      HTML_COMMENT,
        HTML_DD,        HTML_DFN,       HTML_DIR,
        HTML_DL,        HTML_DLC,       HTML_DT,
        HTML_EM, 	HTML_FORM,
        HTML_H1,        HTML_H2,        HTML_H3,
        HTML_H4,        HTML_H5,        HTML_H6,        HTML_H7,
        HTML_HR,
        HTML_I,         HTML_IMG,	HTML_INPUT,     HTML_ISINDEX,
        HTML_KBD,
        HTML_LI,        HTML_LINK,      HTML_LISTING,
        HTML_MENU,	HTML_NEXTID,    
        HTML_OL,        HTML_OPTION,	HTML_P,         
	HTML_PLAINTEXT, HTML_PRE,
        HTML_SAMP,      HTML_SELECT,	HTML_STRONG,	HTML_TEXTAREA,
        HTML_TITLE,     HTML_TT,
        HTML_U,         HTML_UL,
        HTML_VAR,       HTML_XMP } HTMLElement;

#define HTML_ELEMENTS 49

/*

Attribute numbers

 */

/*

   Identifier is HTML_<element>_<attribute>. These must match the tables in HTML.c!
   
 */
#define HTML_A_HREF             0
#define HTML_A_NAME             1
#define HTML_A_TITLE            2
#define HTML_A_TYPE             3
#define HTML_A_URN              4
#define HTML_A_ATTRIBUTES       5

#define DL_COMPACT 0

#define HTML_IMG_ALT		0
#define HTML_IMG_SRC            1
#define HTML_IMG_ATTRIBUTES     2

#define HTML_LINK_HREF          0
#define HTML_LINK_REL           1
#define HTML_LINK_REV           2
#define HTML_LINK_ATTRIBUTES    3

#define HTML_FORM_ACTION	0
#define HTML_FORM_METHOD	1
#define HTML_FORM_ATTRIBUTES    2

#define HTML_SELECT_MULTIPLE	0
#define HTML_SELECT_NAME	1
#define HTML_SELECT_ATTRIBUTES	2

#define HTML_OPTION_SELECTED	0
#define HTML_OPTION_ATTRIBUTES	1

#define HTML_TEXTAREA_COLS	0
#define HTML_TEXTAREA_NAME	1
#define HTML_TEXTAREA_ROWS	2
#define HTML_TEXTAREA_ATTRIBUTES 3

#define HTML_INPUT_CHECKED	0
#define HTML_INPUT_MAXLENGTH	1
#define HTML_INPUT_NAME		2
#define HTML_INPUT_SIZE		3
#define HTML_INPUT_TYPE		4
#define HTML_INPUT_VALUE	5
#define HTML_INPUT_ATTRIBUTES	6

#define NEXTID_N 0

extern CONST SGML_dtd HTML_dtd;


/*

Start anchor element

   It is kinda convenient to have a particulr routine for starting an anchor element, as
   everything else for HTML is simple anyway.
   
  ON ENTRY
  
   targetstream poinst to a structured stream object.
   
   name and href point to attribute strings or are NULL if the attribute is to be omitted.
   
 */
extern void HTStartAnchor PARAMS((
                HTStructured * targetstream,
                CONST char *    name,
                CONST char *    href));


#endif /* HTMLDTD_H */

/*

   End of module definition  */
