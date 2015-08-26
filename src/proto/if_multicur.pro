/* if_multicur.pro */
void mtc_list_init(multi_cursor_list_T *cursor_list);
void mtc_list_free(multi_cursor_list_T *clist);
int  mtc_check_if_exists(multi_cursor_list_T *clist,pos_T * pos);
int mtc_add_pos(multi_cursor_list_T *clist, pos_T * pos);

void mtc_print_all(multi_cursor_list_T *clist);

/* vim: set ft=c : */
