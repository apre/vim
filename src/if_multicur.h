/* vi:set ts=8 sts=4 sw=4 noet:
 *
 * VIM - Vi IMproved	by Bram Moolenaar
 *
 * Do ":help uganda"  in Vim to read copying and usage conditions.
 * Do ":help credits" in Vim to see a list of people who contributed.
 * See README.txt for an overview of the Vim source code.
 */

/* multi-cursor extention by Adrien Pre.
 *
 * Copyright (c) 2015 Adrien Pre <adrienpre+vim@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

/** \file
 * \brief This file contains all stuffs related to the multi-cursor feature.
 *
 * */
#if defined(FEAT_MULTICURSOR) && !defined(FEAT_MULTI_CURSOR_INCLUDED)
#define FEAT_MULTI_CURSOR_INCLUDED

#include "vim.h"

/** "virtual" cursor.
 */
typedef struct mtc_pos_s
{
    linenr_T	lnum;	/* line number */
    colnr_T	col;	/* column number */
#ifdef FEAT_VIRTUALEDIT
    colnr_T	coladd;
#endif

    struct mtc_pos_s *next;
    struct mtc_pos_s *prev;

} mtc_pos_T;

/** multi-cursor list.
 *
 * The list of cursor is attached and specific to each window.
 * It is a double-linked list to allow normal and reverse iteration.
 * */


typedef struct {

    long number_of_cursors; /* number of cursor contained in the list. */
    int sorted; /* indicates if the list is sorted. */
 
    mtc_pos_T * first; /* first cursor of the list */
    mtc_pos_T * last; /* last cursor of the list */

} multi_cursor_list_T;

/** initalize a new empty cursor list. 
 * @param[in,out] cursor_list the cursor list to initialize.
 * */
void mtc_list_init(multi_cursor_list_T *cursor_list);

#endif
