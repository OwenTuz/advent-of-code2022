#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "aoc_utils.h"

#define AOC_ARRAY_MAX 128

typedef struct {
    gchar *name;
    int size;
} File;

typedef struct Directory Directory;

struct Directory {
    gchar *name;
    Directory *parent;
    Directory *children[AOC_ARRAY_MAX];
    File *files[AOC_ARRAY_MAX];
};

Directory *new_directory(gchar* name, Directory *parent){
    Directory *dir = malloc(sizeof(Directory));
    if (!dir){
        printf("Error allocating memory for new directory %s\n", name);
        exit(1);
    }
    dir->name = strndup(name, strlen(name) + 1);
    dir->parent = parent;
    return dir;
}

Directory *cd(gchar *target, Directory *current_dir){
    if (strncmp(target, "..", 2) == 0){ // go up 1 to parent directory
        if (!current_dir->parent){
            printf("FATAL: Error trying to change directory from %s to parent\n", current_dir->name);
            exit(1);
         } else {
            return current_dir->parent;
         }
     }
     // otherwise, find appropriate directory in 'children' and go there
     if (current_dir->children[0] == NULL){
        printf("FATAL: Tried to change directory to %s but it hasn't been initialised yet\n", target);
        exit(1);
      } else {
        for (int i = 0; i < AOC_ARRAY_MAX; ++i){
            if (current_dir->children[i] == NULL) {
                printf("FATAL: Error traversing to directory %s from %s\n", target, current_dir->name);
                exit(1);
             }
             char *name = current_dir->children[i]->name;
             if (strncmp(target, name, strlen(target)) == 0) {
                return current_dir->children[i];
             }
        }
    }
    // we shouldn't get here
    printf("FATAL: Failed to cd to directory %s\n", target);
    exit(1);
}

void add_subdir(Directory *current_dir, gchar *name){
    int i = 0;
    for (; i <= AOC_ARRAY_MAX && current_dir->children[i] != NULL; ++i){
        if (strncmp(
                current_dir->children[i]->name,
                name,
                strlen(current_dir->children[i]->name) + 1
            ) == 0) {
            // dir already listed
            return;
        }
        if (i == AOC_ARRAY_MAX){
            printf("FATAL: Exceeded array bounds while trying to insert dir %s\n", name);
            exit(1);
        }
    }
    gchar *dirname = strndup(name, strlen(name) + 1);
    current_dir->children[i] = new_directory(dirname, current_dir);
    return;
}

void add_file(Directory *current_dir, gchar* name, int size){
    int i = 0;
    for (; i <= AOC_ARRAY_MAX && current_dir->files[i] != NULL; ++i){
        if (strncmp(
                current_dir->files[i]->name,
                name,
                strlen(current_dir->files[i]->name) + 1
            ) == 0){
            // file already listed
            return;
        }
        if (i == AOC_ARRAY_MAX){
            printf("FATAL: Exceeded array bounds while trying to insert file %s\n", name);
            exit(1);
        }
    }
    current_dir->files[i] = malloc(sizeof(File));
    current_dir->files[i]->name = strndup(name, strlen(name) + 1);
    current_dir->files[i]->size = size;
    return;
}

Directory *parse_tree(GArray *input){
    Directory *root = new_directory("/", NULL);
    Directory *current_dir = root;

    for (guint i = 1; i < input-> len; ++i){
        gchar *line = g_array_index(input, gchar *, i);
        char *token = strtok(line, " ");

        if (token[0] == '$') { // This is a command
            token = strtok(NULL, " ");
            if (token[0] == 'l') { // 'ls'
                continue;
            } else if (token[0] == 'c'){ // 'cd' 
                token = strtok(NULL, " ");
                current_dir = cd(token, current_dir);
                continue;
            } else {
                printf("FATAL: Expected command, got %s\n", token);
                exit(1);
            }
        }
        if (token[0] == 'd') { // directory
            token = strtok(NULL, " ");
            add_subdir(current_dir, token);
            continue;
        }
        if (isdigit(token[0])){
            int size = atoi(token);
            token = strtok(NULL, " ");
            add_file(current_dir, token, size);
            continue;
        }
    }
    return root;
}

int get_size(Directory *dir, int *total){
    for (int i=0; i <= AOC_ARRAY_MAX && dir->children[i] != NULL; ++i){
        get_size(dir->children[i], total);
    }
    for (int i=0; i <= AOC_ARRAY_MAX && dir->files[i] != NULL; ++i){
        *total += dir->files[i]->size;
    }
    return *total;
}

int get_size_less_than(Directory *dir, int max, int *total){
    for (int i=0; i <= AOC_ARRAY_MAX && dir->children[i] != NULL; ++i){
        int size = 0;
        size = get_size(dir->children[i], &size);
        if ( size <= max) {
            *total += size;
        }
        get_size_less_than(dir->children[i], max, total);
    }
    return *total;
}

int find_smallest_more_than(Directory *dir, int min, int *smallest){
    for (int i=0; i <= AOC_ARRAY_MAX && dir->children[i] != NULL; ++i){
        int size = 0;
        size = get_size(dir->children[i], &size);
        if ( size >= min && size < *smallest) {
            *smallest = size;
        }
        find_smallest_more_than(dir->children[i], min, smallest);
    }
    return *smallest;
}

int part1(Directory *root){
    int total = 0;
    int max = 100000;
    return get_size_less_than(root, max, &total);
}

int part2(Directory *root){
    int space_used = 0;
    int space_available = 70000000 - get_size(root, &space_used);
    int space_needed = 30000000 - space_available;
    int smallest_found = INT_MAX;
    return find_smallest_more_than(root, space_needed, &smallest_found);
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

    Directory *root = parse_tree(input);
    printf("Part 1: Answer is %d\n", part1(root));
    printf("Part 2: Answer is %d\n", part2(root));
    g_array_free(input, TRUE);
    return 0;
}
