#ifndef __AOC_QUEUE_H__
#define __AOC_QUEUE_H__

#include <glib.h>

typedef GArray aoc_GCharQueue;

gchar aoc_g_char_queue_pop(aoc_GCharQueue *q);
void aoc_g_char_queue_push(aoc_GCharQueue *q, gchar c);
aoc_GCharQueue * aoc_new_g_char_queue(void);
#endif
