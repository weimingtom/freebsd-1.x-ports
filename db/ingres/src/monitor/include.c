#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "monitor.h"
#include <ingres.h>
#include <aux.h>
#include "sccs.h"

#define INGRES_GUTIL
#include "protos.h"

SCCSID(@(#)include.c	8.1	12/31/84)



/*
**  INCLUDE FILE
**
**	A file name, which must follow the \i, is read and inserted
**	into the text stream at this location.  It may include all of
**	the standard control functions.  Includes may be nested.
**
**	If the parameter is 0, the file name is taken from the input;
**	otherwise it is taken directly from the parameter.  In this
**	mode, errors are not printed.
**
**	Prompts are turned off during the include.
*/

void
include(char *filename)
{
	int			savendf;
	FILE			*saveinp;
	register char		*f;
	register FILE		*b;

	f = filename;
	if (f == 0)               /* stands for NULL pointer */
		f = getfilenm();
	if (strcmp(f, "-") == 0) {
		/* read keyboard */
		b = stdin;
	} else if (*f == 0) {
		/* back up one level (EOF on next read) */
		GiveEof = TRUE;
		return;
	} else {
		/* read file */
		if ((b = fopen(f, "r")) == NULL) {
			if (filename == 0)
				printf("Cannot open \"%s\"\n", f);
			return;
		}
	}

	/* check for too deep */
	if (Idepth >= 5) {
		printf("Include nested too deep\n");
		if (b)
			fclose(b);
		return;
	}
	Idepth++;

	/* get input from alternate file */
	savendf = Nodayfile;
	if (b == stdin) {
		Nodayfile = Userdflag;
		prompt("<<input>>");
	}
	else
		Nodayfile = -1;
	saveinp = Input;
	Input = b;
	monitor(TRUE);

	/* done -- restore old file */
	Input = saveinp;
	Nodayfile = savendf;
	Idepth--;
}
