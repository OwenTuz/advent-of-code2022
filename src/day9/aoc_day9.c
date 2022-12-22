#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <glib.h>
#include "aoc_utils.h"

typedef struct Point {
    int x;
    int y;
} Point;

bool is_touching(Point head, Point tail){
    return (tail.y == head.y || tail.y == head.y - 1 || tail.y == head.y + 1) &&
           (tail.x == head.x || tail.x == head.x - 1 || tail.x == head.x + 1);
}

void move_tail(const Point *head, Point *tail){
    // don't adjust in the switch statement: this way, if there's any issue
    // we log the original value before adjustment
    int x_adjust = 0;
    int y_adjust = 0;
    switch (head->x - tail->x){
        case 0:
            break;
        case 1:
        case 2:
            x_adjust = 1;
            break;
        case -1:
        case -2:
            x_adjust = -1;
            break;
        default:
            printf(
                "Error moving tail. Tail: %d, %d / Head: %d, %d\n",
                tail->x, tail->y, head->x, head->y
            );
            exit(1);
    }

    switch (head->y - tail->y){
        case 0:
            break;
        case 1:
        case 2:
            y_adjust = 1;
            break;
        case -1:
        case -2:
            y_adjust = -1;
            break;
        default:
            printf(
                "Error moving tail. Tail: %d, %d / Head: %d, %d\n",
                tail->x, tail->y, head->x, head->y
            );
            exit(2);
    }
    tail->x += x_adjust;
    tail->y += y_adjust;

}

void move(gchar *inst, Point *head, GArray *tail_nodes, GHashTable *visited){
    char *save = NULL;

    gchar *dir = strtok_r(inst, " ", &save);
    int steps = atoi(strtok_r(NULL, " ", &save));

    if (steps == 0) {
        // this assumes the 0 is an error from atoi() and not an instruction
        // to move 0 steps
        printf("Error parsing number of steps: %s\n", inst);
        exit(1);
    }
    for (; steps > 0; steps--){
        switch (dir[0]){
            case 'L':
                head->x -= 1;
                break;
            case 'R':
                head->x += 1;
                break;
            case 'D':
                head->y -= 1;
                break;
            case 'U':
                head->y += 1;
                break;
            default:
                printf("Error parsing move instruction %s, unknown direction %c\n", inst, dir[0]);
                exit(1);
        }
        Point *next_head = head;
        for (guint i = 0; i < tail_nodes->len; ++i){
            Point *tail = &g_array_index(tail_nodes, Point, i);
            if (! is_touching(*next_head, *tail)) {
                move_tail(next_head, tail);
                assert(is_touching(*next_head, *tail));
                if (i == tail_nodes-> len - 1) {
                    char *key = malloc(32); // slightly magic number
                    snprintf(key, 32, "%d,%d", tail->x, tail->y);
                    g_hash_table_add(visited, key);
                }
            }
            next_head = tail;
        }
    }
}

// glib requires this interface when iterating a hashtable but here, we don't
// need two out of three args, so let's stop clang complaining
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void hashtable_count(gpointer key, gpointer value, gpointer user_data){
    int *counter = (int*) user_data;
    *counter += 1;
    return;
}
#pragma clang diagnostic pop

int part1(const GArray *input){
    Point head = { 0, 0 };
    Point tail = { 0, 0 };

    GArray *tails = g_array_new(TRUE, FALSE, sizeof(Point));
    g_array_append_val(tails, tail);

    GHashTable *visited = g_hash_table_new_full(
        g_str_hash, g_str_equal, free, NULL
    );
    g_hash_table_add(visited, strndup("0,0", 4));

    for (guint i = 0; i < input->len; ++i){
        gchar *line = g_array_index(input, gchar *, i);
        gchar *copied_line = strndup(line, strlen(line) + 1);
        move(copied_line, &head, tails, visited);
        free(copied_line);
    }
    int count = 0;
    g_array_free(tails, TRUE);
    g_hash_table_foreach(visited, hashtable_count, &count);
    g_hash_table_destroy(visited);
    return count;
}

int part2(const GArray *input){
    Point head = { 0, 0 };

    GArray *tails = g_array_new(TRUE, FALSE, sizeof(Point));
    for (guint i = 0; i < 9; ++i){
        Point tail = { 0, 0 };
        g_array_append_val(tails, tail);
    }

    GHashTable *visited = g_hash_table_new_full(
        g_str_hash, g_str_equal, free, NULL
    );
    g_hash_table_add(visited, strndup("0,0", 4));

    for (guint i = 0; i < input->len; ++i){
        gchar *line = g_array_index(input, gchar *, i);
        gchar *copied_line = strndup(line, strlen(line) + 1);
        move(copied_line, &head, tails, visited);
        free(copied_line);
    }
    g_array_free(tails, TRUE);
    int count = 0;
    g_hash_table_foreach(visited, hashtable_count, &count);
    g_hash_table_destroy(visited);
    return count;
}

int main(int argc, char **argv) {
    char *filename;
    if (argc > 1) {
        filename = g_strdup(argv[1]);
    } else {
        filename = g_strdup("./input");
    }
    GArray *input = get_input_lines(filename);

    g_free(filename);
    if (!input) exit(1);

    printf("Part 1: Answer is %d\n", part1(input));
    printf("Part 2: Answer is %d\n", part2(input));
    g_array_free(input, TRUE);
    return 0;
}
