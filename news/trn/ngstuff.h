/* $Id: ngstuff.h,v 1.2 1993/07/26 19:12:55 nate Exp $
 */
/* This software is Copyright 1991 by Stan Barber. 
 *
 * Permission is hereby granted to copy, reproduce, redistribute or otherwise
 * use this software as long as: there is no monetary profit gained
 * specifically from the use or reproduction of this software, it is not
 * sold, rented, traded or otherwise marketed, and this copyright notice is
 * included prominently in any copy made. 
 *
 * The authors make no claims as to the fitness or correctness of this software
 * for any use whatsoever, and it is provided as is. Any use of this software
 * is at the user's own risk. 
 */

#define NN_NORM 0
#define NN_INP 1
#define NN_REREAD 2
#define NN_ASK 3

EXT bool one_command INIT(FALSE);	/* no ':' processing in perform() */

void	ngstuff_init _((void));
int	escapade _((void));
int	switcheroo _((void));
int	numnum _((void));
int	perform _((char*,int));
int	use_selected _((void));
