#ifndef __MIDIOGRE_SEARCH_H__
#define __MIDIOGRE_SEARCH_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

gint search_songs(MidiogreApp *app);

int search_handler(void *a, int argc, char **argv, char **col_name);

gint read_instr_classes(MidiogreApp *app);

#endif