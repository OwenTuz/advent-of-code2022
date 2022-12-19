#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "aoc_queue.h"

void aoc_g_char_queue_push(aoc_GCharQueue *queue, gchar item){
    g_array_append_val(queue, item);
}

gchar aoc_g_char_queue_pop(aoc_GCharQueue *queue){
    if(queue-> len == 0){
        printf("Tried to pop from empty queue!");
        exit(1);
    }
    gchar c = g_array_index(queue, gchar, queue->len - 1);
    g_array_remove_index(queue, queue->len - 1);
    return c;
}

aoc_GCharQueue * aoc_new_g_char_queue(void){
    GArray * queue = g_array_new(TRUE, FALSE, sizeof(gchar));
    return queue;
}
