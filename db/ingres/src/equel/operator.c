#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "constants.h"
#include "globals.h"
#include "y.tab.h"
#include "sccs.h"

#define INGRES_GUTIL
#include "protos.h"

SCCSID(@(#)operator.c	8.1	12/31/84)


/*
**  OPERATOR -- process a token starting with an operator
**
**	Processes operators, strings, comments, and 
**	floating constants without a leading 0.
**
**	Parameters:
**		chr - first character of token {equel_cmap (chr) == OPATR}
**
**	Returns:
**		NUMBER or STRING token, or operator token.
**		CONTINUE on error.
**
**	Side Effects:
**		Adds a node to the Symbol space, and returns adress
**		in "yylval".
**		Opcode is set to the opcode of the operator.
**		May backup a character.
*/
int
operator(char chr)
{
	register struct optab	*op;
	char			opbuf [3];

	opbuf [0] = chr;
	opbuf [1] = getch();
	opbuf [2] = '\0';

	if (opbuf [0] == '.' && equel_cmap(opbuf[1]) == NUMBR) {
		/* floating mantissa w/o leading 0 */
		backup(opbuf [1]);
		return (number(opbuf [0]));
	}
	if (equel_cmap(opbuf[1]) != OPATR) {
		backup(opbuf [1]);
		opbuf [1] = '\0';
	}
	/* operator has been reduced to its smallest 
	 * possible length, now try to find it in the
	 * operator table [tokens.y]
	 */
	for ( ; ; ) {
		for (op = Optab; op->op_term; op++)
			if (strcmp(op->op_term, opbuf) == 0)
				break;
		if (!op->op_term && opbuf [1]) {
			/* reduce a 2 char operator to 1 char,
			 * and re-search
			 */
			backup(opbuf[1]);
			opbuf [1] = '\0';
			continue;
		}
		break;
	}
	if (op->op_term) {
		/* string quotes ? */
		if (op->op_token == Tokens.sp_quote)
			return (string(op));

		/* comment indicator ? */
		if (op->op_token == Tokens.sp_bgncmnt)
			return (comment());

		/* {strcmp(opbuf, op->op_term) == 0} */
		Opcode = op->op_code;
		yylval.u_dn = addsym(op->op_term);
		return (op->op_token);
	}
	yysemerr("bad operator", opbuf);

	/* operator not found, skip token and try again */
	return (CONTINUE);
}
