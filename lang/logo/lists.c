/*
 *      lists.c         logo list functions module              dvb
 *
 *	Copyright (C) 1993 by the Regents of the University of California
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *  
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *  
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "logo.h"
#include "globals.h"

NODE *bfable_arg(NODE *args)
{
    NODE *arg = car(args);

    while ((arg == NIL || arg == UNBOUND || arg == Null_Word ||
	    nodetype(arg) == ARRAY) && NOT_THROWING) {
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
    }
    return arg;
}

NODE *list_arg(NODE *args)
{
    NODE *arg = car(args);

    while (!(arg == NIL || is_list(arg)) && NOT_THROWING) {
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
    }
    return arg;
}

NODE *lbutfirst(NODE *args)
{
    NODE *val = UNBOUND, *arg;

    arg = bfable_arg(args);
    if (NOT_THROWING) {
	if (is_list(arg))
	    val = cdr(arg);
	else {
	    setcar(args, cnv_node_to_strnode(arg));
	    arg = car(args);
	    if (getstrlen(arg) > 1)
		val = make_strnode(getstrptr(arg) + 1,
			  getstrhead(arg),
			  getstrlen(arg) - 1,
			  nodetype(arg),
			  strnzcpy);
	    else
		val = Null_Word;
	}
    }
    return(val);
}

NODE *lbutlast(NODE *args)
{
    NODE *val = UNBOUND, *lastnode, *tnode, *arg;

    arg = bfable_arg(args);
    if (NOT_THROWING) {
	if (is_list(arg)) {
	    args = arg;
	    val = NIL;
	    while (cdr(args) != NIL) {
		tnode = cons(car(args), NIL);
		if (val == NIL) {
		    val = tnode;
		    lastnode = tnode;
		} else {
		    setcdr(lastnode, tnode);
		    lastnode = tnode;
		}
		args = cdr(args);
		if (check_throwing) break;
	    }
	} else {
	    setcar(args, cnv_node_to_strnode(arg));
	    arg = car(args);
	    if (getstrlen(arg) > 1)
		val = make_strnode(getstrptr(arg),
			  getstrhead(arg),
			  getstrlen(arg) - 1,
			  nodetype(arg),
			  strnzcpy);
	    else
		val = Null_Word;
	}
    }
    return(val);
}

NODE *lfirst(NODE *args)
{
    NODE *val = UNBOUND, *arg;

    if (nodetype(car(args)) == ARRAY) {
	return make_intnode((FIXNUM)getarrorg(car(args)));
    }
    arg = bfable_arg(args);
    if (NOT_THROWING) {
	if (is_list(arg))
	    val = car(arg);
	else {
	    setcar(args, cnv_node_to_strnode(arg));
	    arg = car(args);
	    val = make_strnode(getstrptr(arg), getstrhead(arg), 1,
			       nodetype(arg), strnzcpy);
	}
    }
    return(val);
}

NODE *lfirsts(NODE *args)
{
    NODE *val = UNBOUND, *arg, *argp, *tail;

    arg = list_arg(args);
    if (car(args) == NIL) return(NIL);
    if (NOT_THROWING) {
	val = cons(lfirst(arg), NIL);
	tail = val;
	for (argp = cdr(arg); argp != NIL; argp = cdr(argp)) {
	    setcdr(tail, cons(lfirst(argp), NIL));
	    tail = cdr(tail);
	    if (check_throwing) break;
	}
	if (stopping_flag == THROWING) {
	    gcref(val);
	    return UNBOUND;
	}
    }
    return(val);
}

NODE *lbfs(NODE *args)
{
    NODE *val = UNBOUND, *arg, *argp, *tail;

    arg = list_arg(args);
    if (car(args) == NIL) return(NIL);
    if (NOT_THROWING) {
	val = cons(lbutfirst(arg), NIL);
	tail = vref(val);
	for (argp = cdr(arg); argp != NIL; argp = cdr(argp)) {
	    setcdr(tail, cons(lbutfirst(argp), NIL));
	    tail = cdr(tail);
	    if (check_throwing) break;
	}
	if (stopping_flag == THROWING) {
	    gcref(val);
	    return UNBOUND;
	}
    }
    return(val);
}

NODE *llast(NODE *args)
{
    NODE *val = UNBOUND, *arg;

    arg = bfable_arg(args);
    if (NOT_THROWING) {
	if (is_list(arg)) {
	    args = arg;
	    while (cdr(args) != NIL) {
		args = cdr(args);
		if (check_throwing) break;
	    }
	    val = car(args);
	}
	else {
	    setcar(args, cnv_node_to_strnode(arg));
	    arg = car(args);
	    val = make_strnode(getstrptr(arg) + getstrlen(arg) - 1,
			       getstrhead(arg), 1, nodetype(arg), strnzcpy);
	}
    }
    return(val);
}

NODE *llist(NODE *args)
{
    return(args);
}

NODE *lemptyp(NODE *arg)
{
    return torf(car(arg) == NIL || car(arg) == Null_Word);
}

NODE *char_arg(NODE *args)
{
    NODE *arg = car(args), *val;

    val = cnv_node_to_strnode(arg);
    while ((val == UNBOUND || getstrlen(val) != 1) && NOT_THROWING) {
	gcref(val);
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
	val = cnv_node_to_strnode(arg);
    }
    setcar(args,val);
    return(val);
}

NODE *lascii(NODE *args)
{
    FIXNUM i;
    NODE *val = UNBOUND, *arg;

    arg = char_arg(args);
    if (NOT_THROWING) {
	i = (FIXNUM)clearparity(*getstrptr(arg)) & 0377;
	val = make_intnode(i);
    }
    return(val);
}

NODE *lbackslashedp(NODE *args)
{
    char i;
    NODE *arg;

    arg = char_arg(args);
    if (NOT_THROWING) {
	i = *getstrptr(arg);
	return torf(getparity(i));
    }
    return(UNBOUND);
}

NODE *lchar(NODE *args)
{
    NODE *val = UNBOUND, *arg;
    char c;

    arg = pos_int_arg(args);
    if (NOT_THROWING) {
	c = getint(arg);
	val = make_strnode(&c, (char *)NULL, 1,
		       (getparity(c) ? STRING : BACKSLASH_STRING), strnzcpy);
    }
    return(val);
}

NODE *lcount(NODE *args)
{
    int cnt = 0;
    NODE *arg;

    arg = car(args);
    if (arg != NIL && arg != Null_Word) {
	if (is_list(arg)) {
	    args = arg;
	    for (; args != NIL; cnt++) {
		args = cdr(args);
		if (check_throwing) break;
	    }
	} else if (nodetype(arg) == ARRAY) {
	    cnt = getarrdim(arg);
	} else {
	    setcar(args, cnv_node_to_strnode(arg));
	    cnt = getstrlen(car(args));
	}
    }
    return(make_intnode((FIXNUM)cnt));
}

NODE *lfput(NODE *args)
{
    NODE *lst, *arg;

    arg = car(args);
    lst = list_arg(cdr(args));
    if (NOT_THROWING)
	return cons(arg,lst);
    else
	return UNBOUND;
}

NODE *llput(NODE *args)
{
    NODE *lst, *arg, *val = UNBOUND, *lastnode = NIL, *tnode = NIL;

    arg = car(args);
    lst = list_arg(cdr(args));
    if (NOT_THROWING) {
	val = NIL;
	while (lst != NIL) {
	    tnode = cons(car(lst), NIL);
	    if (val == NIL) {
		val = tnode;
	    } else {
		setcdr(lastnode, tnode);
	    }
	    lastnode = tnode;
	    lst = cdr(lst);
	    if (check_throwing) break;
	}
	if (val == NIL)
	    val = cons(arg, NIL);
	else
	    setcdr(lastnode, cons(arg, NIL));
    }
    return(val);
}

NODE *string_arg(NODE *args)
{
    NODE *arg = car(args), *val;

    val = cnv_node_to_strnode(arg);
    while (val == UNBOUND && NOT_THROWING) {
	gcref(val);
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
	val = cnv_node_to_strnode(arg);
    }
    setcar(args,val);
    return(val);
}

NODE *lword(NODE *args)
{
    NODE *val = NIL, *arg = NIL, *tnode = NIL, *lastnode = NIL;
    int cnt = 0;
    NODETYPES str_type = STRING;

    if (args == NIL) return Null_Word;
    val = args;
    while (val != NIL && NOT_THROWING) {
	arg = string_arg(val);
	val = cdr(val);
	if (NOT_THROWING) {
	    if (backslashed(arg))
		str_type = VBAR_STRING;
	    cnt += getstrlen(arg);
	}
    }
    if (NOT_THROWING)
	val = make_strnode((char *)args, (char *)NULL,
			   cnt, str_type, word_strnzcpy); /* kludge */
    else
	val = UNBOUND;
    return(val);
}

NODE *lsentence(NODE *args)
{
    NODE *tnode = NIL, *lastnode = NIL, *val = NIL, *arg = NIL;

    while (args != NIL && NOT_THROWING) {
	arg = car(args);
	while (nodetype(arg) == ARRAY && NOT_THROWING) {
	    setcar(args, err_logo(BAD_DATA, arg));
	    arg = car(args);
	}
	args = cdr(args);
	if (stopping_flag == THROWING) break;
	if (is_list(arg)) {
	    while (arg != NIL && NOT_THROWING) {
		tnode = cons(car(arg), NIL);
		arg = cdr(arg);
		if (val == NIL) val = tnode;
		else setcdr(lastnode, tnode);
		lastnode = tnode;
	    }
	} else {
	    tnode = cons(arg, NIL);
	    if (val == NIL) val = tnode;
	    else setcdr(lastnode, tnode);
	    lastnode = tnode;
	}
    }
    if (stopping_flag == THROWING) {
	gcref(val);
	return UNBOUND;
    }
    return(val);
}

NODE *lwordp(NODE *arg)
{
    arg = car(arg);
    return torf(arg != UNBOUND && !aggregate(arg));
}

NODE *llistp(NODE *arg)
{
    arg = car(arg);
    return torf(is_list(arg));
}

NODE *lnumberp(NODE *arg)
{
    setcar(arg, cnv_node_to_numnode(car(arg)));
    return torf(car(arg) != UNBOUND);
}

NODE *larrayp(NODE *arg)
{
    return torf(nodetype(car(arg)) == ARRAY);
}

NODE *memberp_help(NODE *args, BOOLEAN notp)
{
    NODE *obj1, *obj2, *val;
    int leng;
    int caseig = (compare_node(valnode__caseobj(Caseignoredp),
			       True, TRUE) == 0);

    val = False;
    obj1 = car(args);
    obj2 = cadr(args);
    if (is_list(obj2)) {
	while (obj2 != NIL && NOT_THROWING) {
	    if (equalp_help(obj1, car(obj2), caseig))
		return (notp ? obj2 : True);
	    obj2 = cdr(obj2);
	    if (check_throwing) break;
	}
	return (notp ? NIL : False);
    }
    else if (nodetype(obj2) == ARRAY) {
	int len = getarrdim(obj2);
	NODE **data = getarrptr(obj2);

	if (notp)
	    err_logo(BAD_DATA_UNREC,obj2);
	while (--len >= 0 && NOT_THROWING) {
	    if (equalp_help(obj1, *data++, caseig)) return True;
	}
	return False;
    } else {
	NODE *tmp;
	int i;

	if (aggregate(obj1)) return (notp ? Null_Word : False);
	setcar (cdr(args), cnv_node_to_strnode(obj2));
	obj2 = cadr(args);
	setcar (args, cnv_node_to_strnode(obj1));
	obj1 = car(args);
	tmp = NIL;
	if (obj1 != UNBOUND && obj2 != UNBOUND &&
	    getstrlen(obj1) <= getstrlen(obj2)) {
	    leng = getstrlen(obj2) - getstrlen(obj1);
	    setcar(cdr(args),make_strnode(getstrptr(obj2), getstrhead(obj2),
					  getstrlen(obj1), nodetype(obj2),
					  strnzcpy));
	    tmp = cadr(args);
	    for (i = 0; i <= leng; i++) {
		if (equalp_help(obj1, tmp, caseig)) {
		    if (notp) {
			setstrlen(tmp,leng+getstrlen(obj1)-i);
			return tmp;
		    } else return True;
		}
		setstrptr(tmp, getstrptr(tmp) + 1);
	    }
	}
	return (notp ? Null_Word : False);
    }
}

NODE *lmemberp(NODE *args)
{
    return(memberp_help(args, FALSE));
}

NODE *lmember(NODE *args)
{
    return(memberp_help(args, TRUE));
}

NODE *integer_arg(NODE *args)
{
    NODE *arg = car(args), *val;

    val = cnv_node_to_numnode(arg);
    while (nodetype(val) != INT && NOT_THROWING) {
	gcref(val);
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
	val = cnv_node_to_numnode(arg);
    }
    setcar(args,val);
    if (nodetype(val) == INT) return(val);
    return UNBOUND;
}

FIXNUM int_arg(NODE *args)
{
    NODE *arg =integer_arg(args);

    if (NOT_THROWING) return getint(arg);
    return 0;
}

NODE *litem(NODE *args)
{
    int i;
    NODE *obj, *val;

    val = integer_arg(args);
    obj = cadr(args);
    while ((obj == NIL || obj == Null_Word) && NOT_THROWING) {
	setcar(cdr(args), err_logo(BAD_DATA, obj));
	obj = cadr(args);
    }
    if (NOT_THROWING) {
	i = getint(val);
	if (is_list(obj)) {
	    if (i <= 0) {
		err_logo(BAD_DATA_UNREC, val);
		return UNBOUND;
	    }
	    while (--i > 0) {
		obj = cdr(obj);
		if (obj == NIL) {
		    err_logo(BAD_DATA_UNREC, val);
		    return UNBOUND;
		}
	    }
	    return car(obj);
	}
	else if (nodetype(obj) == ARRAY) {
	    i -= getarrorg(obj);
	    if (i < 0 || i >= getarrdim(obj)) {
		err_logo(BAD_DATA_UNREC, val);
		return UNBOUND;
	    }
	    return (getarrptr(obj))[i];
	}
	else {
	    if (i <= 0) {
		err_logo(BAD_DATA_UNREC, val);
		return UNBOUND;
	    }
	    setcar (cdr(args), cnv_node_to_strnode(obj));
	    obj = cadr(args);
	    if (i > getstrlen(obj)) {
		err_logo(BAD_DATA_UNREC, val);
		return UNBOUND;
	    }
	    return make_strnode(getstrptr(obj) + i - 1, getstrhead(obj),
				1, nodetype(obj), strnzcpy);
	}
    }
    return(UNBOUND);
}

int circular(NODE *arr, NODE *new)
{
    if (new == NIL) return(0);
    else if (nodetype(new) == ARRAY) {
	int i = getarrdim(new);
	NODE **p = getarrptr(new);

	if (new == arr) return(1);
	while (--i >= 0) {
	    if (circular(arr,*p++)) return(1);
	}
	return(0);
    } else if (is_list(new)) {
	while (new != NIL) {
	    if (circular(arr,car(new))) return(1);
	    new = cdr(new);
	}
	return(0);
    } else return(0);
}

NODE *setitem_helper(NODE *args, BOOLEAN safe)
{
    int i;
    NODE *obj, *val, *cont;

    val = integer_arg(args);
    obj = cadr(args);
    while (nodetype(obj) != ARRAY && NOT_THROWING) {
	setcar(cdr(args), err_logo(BAD_DATA, obj));
	obj = cadr(args);
    }
    cont = car(cddr(args));
    if (NOT_THROWING) {
	i = getint(val);
	if (safe) {
	    while (circular(obj,cont) && NOT_THROWING) {
		setcar(cddr(args), err_logo(BAD_DATA, cont));
		cont = car(cddr(args));
	    }
	}
	if (NOT_THROWING) {
	    i -= getarrorg(obj);
	    if (i < 0 || i >= getarrdim(obj))
		err_logo(BAD_DATA_UNREC, val);
	    else {
		val = (getarrptr(obj))[i];
		(getarrptr(obj))[i] = vref(cont);
		deref(val);
	    }
	}
    }
    return(UNBOUND);
}

NODE *lsetitem(NODE *args) {
    return setitem_helper(args, TRUE);
}

NODE *l_setitem(NODE *args) {
    return setitem_helper(args, FALSE);
}

NODE *larray(NODE *args)
{
    NODE *arg;
    int d, o;

    arg = pos_int_arg(args);
    if (cdr(args) != NIL) o = int_arg(cdr(args));
    else o = 1;

    if (NOT_THROWING) {
	d = getint(arg);
	arg = make_array(d);
	setarrorg(arg,o);
	return arg;
    }
    return UNBOUND;
}

FLONUM float_arg(NODE *args)
{
    NODE *arg = car(args), *val;

    val = cnv_node_to_numnode(arg);
    while (!is_number(val) && NOT_THROWING) {
	gcref(val);
	setcar(args, err_logo(BAD_DATA, arg));
	arg = car(args);
	val = cnv_node_to_numnode(arg);
    }
    setcar(args,val);
    if (nodetype(val) == FLOAT) return getfloat(val);
    if (nodetype(val) == INT) return (FLONUM)getint(val);
    return 0.0;
}

NODE *lform(NODE *args)
{
    FLONUM number;
    int width, precision;
    char result[100];
    char format[20];

    number = float_arg(args);
    width = (int)int_arg(cdr(args));
    if (width < 0) {
	print_stringptr = format;
	print_stringlen = 20;
	ndprintf((FILE *)NULL,"%p\n",string_arg(cddr(args)));
	*print_stringptr = '\0';
    } else
	precision = (int)int_arg(cddr(args));
    if (NOT_THROWING) {
	if (width >= 100) width = 99;
	if (width < 0)
	    sprintf(result,format,number);
	else
	    sprintf(result,"%*.*f",width,precision,number);
	return(make_strnode(result, (char *)NULL, (int)strlen(result),
			    STRING, strnzcpy));
    }
    return(UNBOUND);
}

NODE *l_setfirst(NODE *args)
{
    NODE *list, *newval;

    list = car(args);
    newval = cadr(args);
    while (NOT_THROWING && (list == NIL || !is_list(list))) {
	setcar(args, err_logo(BAD_DATA,list));
	list = car(args);
    }
    setcar(list,newval);
    return(UNBOUND);
}

NODE *l_setbf(NODE *args)
{
    NODE *list, *newval;

    list = car(args);
    newval = cadr(args);
    while (NOT_THROWING && (list == NIL || !is_list(list))) {
	setcar(args, err_logo(BAD_DATA,list));
	list = car(args);
    }
    setcdr(list,newval);
    return(UNBOUND);
}