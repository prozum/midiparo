#ifndef __MID_H
#define __MID_H

#include <mid/mid.h>

#include <gtk/gtk.h>

enum
{
    COLUMN_MSG,
    COLUMN_BYTE_1,
    COLUMN_BYTE_2,
    COLUMN_CHAN,
    COLUMN_DELTA,
    COLUMN_DATA,
    NUM_COLUMNS
};


int mid_import(GFile *mid_gfile, GtkWidget *notebook);
int mid_export(mid_t *mid);

#endif
