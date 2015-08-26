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


const int  NUMBER_OF_CURSORS_IN_STORE = 10;

/** initalize and allocate a new empty cursor list. 
 * @param[in,out] cursor_list the cursor list to initialize.
 * */
void mtc_list_init(multi_cursor_list_T * clist) {
    clist->sorted = FALSE;
    ga_init2( &clist->cursors, sizeof(pos_T),NUMBER_OF_CURSORS_IN_STORE);
}

/** delete all cursors contained in the cursor list. 
 */
void mtc_list_free(multi_cursor_list_T *clist) {
    ga_clear(&clist->cursors);
    clist->sorted = FALSE;
}


/** 3 way compare of cursors positions. */
static int cursor_cmp(pos_T *p1, pos_T *p2) {
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
    } 
    
    if (pos1 > pos2)  {
	return 1; } else {
	    return -1;
	}
}

/** sort cursor in the cursor list. */
void mtc_list_sort(multi_cursor_list_T *clist) {
    printf("Sorting cursor list (%3d cursors)\n",clist->cursors.ga_len);

    if (clist->cursors.ga_data) {
    qsort(clist->cursors.ga_data,clist->cursors.ga_len,sizeof(pos_T),cursor_cmp);

    clist->sorted = TRUE;
    }

}

/** checks if the cursor is already in the cursor list.
 * @param clist the cursor list to look at
 * @param pos the curser to check if it is in \c clist
 * @retval 0 if \c pos is NOT in the list.
 * @retval 1 if \c pos is in the list
 * */
int mtc_check_if_exists(multi_cursor_list_T *clist,
		 pos_T * pos) {
    int i;
    pos_T * cursor_array;

    cursor_array = (pos_T *) clist->cursors.ga_data;

    for (i=0;i< clist->cursors.ga_len;i++) {
	if  (cursor_cmp( &cursor_array[i] ,pos) == 0 ) {
	    return 1;
	}
    }
    return 0;
}


/** append a new cursor into the cursor list.
 * @param clist the cursor list to edit
 * @param pos   the item to add into the list.
 * The cursor \c pos is copied into the list, so it can be safely deallocated .  
 *
 * @retval OK if  pos has been added to the cursor list
 * @retval FAIL if \c pos were not unique.
 *
 * 
 * 
 * */
int mtc_add_pos(multi_cursor_list_T *clist,
		 pos_T * pos) {
    int status = OK;
    int retval;
    
    clist->sorted = FALSE;

    if (mtc_check_if_exists(clist,pos) != 0) {
	printf("Error: cursor %d,%d is already existing\n",pos->lnum,pos->col);
	return FAIL;
    }	

    if (clist->cursors.ga_len == clist->cursors.ga_maxlen) {
	printf("Cursor list need growing");
	status = ga_grow(&clist->cursors,NUMBER_OF_CURSORS_IN_STORE);
    }

    if (status == OK) {
	pos_T * cursor_array;

	cursor_array = (pos_T*) clist->cursors.ga_data;

	cursor_array[clist->cursors.ga_len] = *pos;
	clist->cursors.ga_len= clist->cursors.ga_len + 1;
    } else {
	/* TODO: handle growing error*/
    }

    return retval;
}



void mtc_print_all(multi_cursor_list_T *clist) {
    int i;
    pos_T * cursor_array;

    cursor_array = (pos_T *) clist->cursors.ga_data;

    for (i=0;i< clist->cursors.ga_len;i++) {
	printf("%03d: (%3d,%3d)\n",i,cursor_array[i].lnum,cursor_array[i].col);
    }
}


#endif /* FEAT_MULTICURSOR */
