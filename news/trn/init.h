/* $Id: init.h,v 1.2 1993/07/26 19:12:30 nate Exp $
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

EXT char *lockname INIT(nullstr);
EXT long our_pid;

bool	initialize _((int,char**));
void	lock_check _((void));
void	newsnews_check _((void));
