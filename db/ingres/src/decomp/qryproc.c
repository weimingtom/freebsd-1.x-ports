#include <stdio.h>

#include <ingres.h>
#include <aux.h>
#include <tree.h>
#include <symbol.h>
#include "globs.h"
#include <pv.h>
#include <lock.h>
#include <resp.h>
#include "sccs.h"
#include <errors.h>

#define INGRES_GUTIL
#define INGRES_CTLMOD
#include "protos.h"

SCCSID(@(#)qryproc.c	8.4	12/18/85)

/* Control Module configuration information structure */

func_t	DeOvqpFn = {
	"DECOMP/OVQP",
	qryproc,
	de_init,
	de_rubproc,
	(char *) &De,
	sizeof(De),
	tTdecomp,
	100,
	'D',
	0,
};

/*ARGSUSED*/
int
qryproc(int pc, paramv_t *pv)
{
	register qtree_t	*root, *q;
	register int		i;
	int			mode, result_num, retr_uniq;
	extern long		Accuread, Accuwrite, Accusread;
	int			loc_qbuf[1+QBUFSIZ/sizeof(int)];
	resp_t			*rp;

#ifdef xDTR1
	if (tTf(50, 0)) {
		Accuread = 0;
		Accusread = 0;
		Accuwrite = 0;
	}
#endif

	De.de_qbuf = (char *)loc_qbuf;
	initbuf((char *)loc_qbuf, QBUFSIZ, QBUFFULL, derror);

	/* init various variables in decomp for start of this query */
	startdecomp();

	/* Read in query, range table and mode */
	if (pv[0].pv_type != PV_QTREE)
		syserr("qryproc: bad parameter");
	root = pv[0].pv_val.pv_qtree;
	/* a john fix (below)
	root->sym.value.sym_root.rootuser = TRUE; */
#ifdef xDTR1
	if (tTf(50, 4)) {
		printf("qryproc\n");
		treepr(root);
	}
#endif

	/* initialize qt parameters */
	mode = De.de_qmode = Qt.qt_qmode;
	De.de_resultvar = Qt.qt_resvar;

	/*
	** Initialize range table. This code should eventually
	** be changed to take advantage of the fact that all
	** the openrs are already done by the control module.
	*/

	for (i = 0; i < MAX_RANGES; i++) {
		if (Qt.qt_rangev[i].rngvdesc != NULL) {
			De.de_rangev[i].relnum = rnum_assign(Qt.qt_rangev[i].rngvdesc->d_r.r_id);

		}
	}
	/* Initialize relation descriptors */
	initdesc(mode);

	/* locate pointers to QLEND and TREE nodes */
	for (q = root->right; q->sym.type != QLEND; q = q->right)
		continue;
	De.de_qle = q;

	for (q = root->left; q->sym.type != TREE; q = q->left)
		continue;
	De.de_tr = q;


	/* map the complete tree */
	mapvar(root, 0);

	/* set logical locks */
	if (Lockrel)
		lockit(root, De.de_resultvar);

	/* If there is no result variable then this must be a retrieve to the terminal */
	De.de_qry_mode = De.de_resultvar < 0 ? mdRETTERM : mode;

	/* if the mode is retrieve_unique, then make a result rel */
	retr_uniq = mode == mdRET_UNI;
	if (retr_uniq) {
		mk_unique(root);
		mode = mdRETR;
	}

	/* get id of result relation */
	if (De.de_resultvar < 0)
		result_num = NORESULT;
	else
		result_num = De.de_rangev[De.de_resultvar].relnum;

	/* evaluate aggregates in query */
	aggregate(root);

	/* decompose and process aggregate free query */
	decomp(root, mode, result_num);

	/* If this is a retrieve unique, then retrieve results */
	if (retr_uniq)
		pr_unique(root, De.de_resultvar);

	if (mode != mdRETR)
		i = ACK;
	else
		i = NOACK;
	i = endovqp(i);

#ifdef xDTR1
	if (tTf(50, 1)) {
		printf("DECOMP read %ld pages,", Accuread);
		printf("%ld catalog pages,", Accusread);
		printf("wrote %ld pages\n", Accuwrite);
	}
#endif

	/* call update processor if batch mode */
	if (i == UPDATE) {
		initp();
		call_dbu(mdUPDATE, -1);
	}


	/* clean decomp */
	reinit();
	rp = getresp();
	if (i != UPDATE) {
		rp->resp_tups = De.ov_tupsfound;
	}

	return (0);
}
