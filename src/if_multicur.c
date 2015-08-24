/* vi:set ts=8 sts=4 sw=4:
 *
 * VIM - Vi IMproved    by Bram Moolenaar
 *
 * Do ":help uganda"  in Vim to read copying and usage conditions.
 * Do ":help credits" in Vim to see a list of people who contributed.
 * See README.txt for an overview of the Vim source code.
 */
/*
 * multi-cursor extensions by Adrien Pre
 *
 * Copyright (c) 2015 Adrien Pre <adrienpre+vim@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */


#include "vim.h"
#include <stdio.h> // printf (debug)

#if defined(FEAT_MULTICURSOR)

#include "if_multicur.h"
#include <stdlib.h> /* for qsort */

void mtc_list_init(multi_cursor_list_T * clist) {
    clist->first = NULL;
    clist->last = NULL;
    clist->number_of_cursors = 0;
    clist->sorted = 0;
}

/** delete all cursors contained in the cursor list. 
 */
void mtc_list_free(multi_cursor_list_T *clist) {
     mtc_pos_T * cursor, * next_cursor;

    cursor = clist->first;

     while (cursor) {
	 next_cursor = cursor->next;
	 vim_free(cursor);
	 cursor = next_cursor;
     }

     mtc_list_init(clist);
}


/** 3 way compare of cursors positions. */
static int cursor_cmp(mtc_pos_T *p1, mtc_pos_T *p2) {
    int pos1;
    int pos2;

    pos1= p1->lnum + p1->col;
    pos2= p2->lnum + p2->col;
#ifdef FEAT_VIRTUALEDIT
    pos1 += p1->coladd;
    pos2 += p2->coladd;
#endif

    if (pos1 == pos2) {
	return 0;
    } else {
	return pos1 < pos2;
    }
}

/** sort cursor in the cursor list. */
void mtc_list_sort(multi_cursor_list_T *clist) {
    printf("Sorting cursor list (%3d cursors)\n",clist->number_of_cursors);

}

/** add a new cursor at the end of the cursor list. */
void mtc_add_cursor(multi_cursor_list_T *clist,
	linenr_T lnum,
	colnr_T col,
	colnr_T coladd) {

    mtc_pos_T * new_cursor = NULL;
    
    new_cursor = (mtc_pos_T*) alloc(sizeof(mtc_pos_T));
    if (new_cursor) {
	vim_memset(new_cursor,0,sizeof(mtc_pos_T));
	new_cursor->prev = clist->last;

	new_cursor->lnum = lnum;
	new_cursor->col = col;
#ifdef FEAT_VIRTUALEDIT	
	new_cursor->coladd = coladd;
#endif
	clist->last = new_cursor;
	clist->number_of_cursors = clist->number_of_cursors +1;
	clist->sorted = FALSE; /* invalidate sorted flag to force re-sorting*/
    }
}


#endif /* FEAT_MULTICURSOR */
