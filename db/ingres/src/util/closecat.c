#include <ingres.h>
#include <aux.h>
#include <access.h>
#include "sccs.h"

#include "protos.h"

SCCSID(@(#)closecat.c	8.1	12/31/84)

/*
**  CLOSECATALOG -- close system catalog
**
**	This routine closes the sysetm relations opened by calls
**	to opencatalog.
**
**	The 'Desxx' struct defines which relations will be closed
**	in this manner and is defined in .../source/aux.h.
**
**	The actual desxx structure definition is in the file
**	
**		catalog_desc.c
**
**	which defines which relations can be cached and if any
**	alias descriptors exist for the relations. That file
**	can be redefined to include various caching.
**
**	Parameters:
**		really - whether to actually close the relations
**			or just update and flush them.
**
**	Returns:
**		none
**
**	Side Effects:
**		A relation is (may be) closed and its pages flushed
**
**	Trace Flags:
**		none
*/

void
closecatalog(bool really)
{
	register struct desxx	*p;
	extern struct desxx	Desxx[];
	extern long		CmOfiles;

#ifdef DEBUG
printf("%d: real closecatalog\n", getpid());
#endif
	for (p = Desxx; p->cach_relname; p++) {
		if (really && !p->cach_alias) {
			CmOfiles &= ~(1 << p->cach_desc->d_fd);
			if (closer(p->cach_desc) < 0) {
				syserr("closecat %s", p->cach_relname);
			}
		} else {
			if (noclose(p->cach_desc) < 0) {
				syserr("closecat %s", p->cach_relname);
			}
			if (really) {
				p->cach_desc->d_opened = 0;
			}
		}
	}
}
