/*
 * Copyright 1993 The University of Newcastle upon Tyne
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose other than its commercial exploitation
 * is hereby granted without fee, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of The University of Newcastle upon Tyne not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. The University of
 * Newcastle upon Tyne makes no representations about the suitability of
 * this software for any purpose. It is provided "as is" without express
 * or implied warranty.
 * 
 * THE UNIVERSITY OF NEWCASTLE UPON TYNE DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF
 * NEWCASTLE UPON TYNE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * 
 * Author:  Jim Wight (j.k.wight@newcastle.ac.uk)
 *          Department of Computing Science
 *          University of Newcastle upon Tyne, UK
 */

#ifndef _AxeiiUndo_h
#define _AxeiiUndo_h

#include <X11/Intrinsic.h>
#include <X11/Xaw/AsciiText.h>

#define INSERT   1
#define DELETE   2
#define DELETING 3
#define REPLACE  4

typedef struct _UndoRecord {
    struct _UndoRecord *prev;
    XawTextPosition    begin;
    XawTextPosition    end;
    int                undoType;
    char               *data;
    void               (*complete)(); 
    struct _UndoRecord *next;
} UndoRecord;

extern void InitialiseUndo();
extern void UndoHook();
extern UndoRecord *LinkNewUndoRecord();
extern void UnlinkUndoRecord();
extern void NullAction();
extern void UndoAction();
extern void InsertUndo();
extern void ReplaceUndo();
extern void ResetUndo();

#endif /* _AxeiiUndo_h */
