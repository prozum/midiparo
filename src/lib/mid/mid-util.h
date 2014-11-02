/** @file mid-util.h */

#ifndef __MIDIUTIL_H__
#define __MIDIUTIL_H__

#include "mid.h"

void print_header(mid_t *mid);
void print_tracks(mid_t *mid);
void print_event(mid_t *mid, unsigned int t, unsigned int e);

#endif