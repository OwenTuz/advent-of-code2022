#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdint.h>
#include "aoc_utils.h"

typedef void(*Operation)(uint64_t *item, const uint64_t *operand);

typedef struct Monkey {
    uint64_t id;
    uint64_t next_id_if_true;
    uint64_t next_id_if_false;

    GQueue *items;

    Operation opt;
    uint64_t operand; // the value to which an item's worry level is
                      // added or multiplied
                      // special case: if 0, operand is 'self' (eg 'old * old')
    uint64_t test_divisor;

    uint64_t inspected;
} Monkey;

Monkey *monkey(
    uint64_t id, uint64_t next_true, uint64_t next_false,
    Operation opt, uint64_t operand, uint64_t test_divisor
){
    Monkey *monkey = malloc(sizeof(Monkey));
    monkey->id = id;
    monkey->next_id_if_true = next_true;
    monkey->next_id_if_false = next_false;
    monkey->items = g_queue_new();
    monkey->opt = opt;
    monkey->operand = operand;
    monkey->test_divisor = test_divisor;
    monkey->inspected = 0;
    return monkey;
}

void monkey_free(Monkey *monkey){
    g_queue_free_full(monkey->items, free);
    free(monkey);
    return;
}

void M_Mul(uint64_t *item, const uint64_t *operand){
    if (*operand == 0) {
        *item = *item * *item;
    } else {
        *item = *operand * *item;
    }
    return;
}

void M_Add(uint64_t *item, const uint64_t *operand){
    *item = *item + *operand;
    return;
}

void inspect_and_get_bored(Monkey *monkey, uint64_t *item){
    monkey->opt(item, &monkey->operand);
    *item = *item / 3;
    monkey->inspected +=1;
}

void inspect(Monkey *monkey, uint64_t *item){
    monkey->opt(item, &monkey->operand);
    // magic number here is the GCD of all the divisors used by the monkeys
    // they're all prime so we can get it the easy way
    *item = *item % (7 * 2 * 19 * 3 * 13 * 17 * 11 * 5);
    monkey->inspected +=1;
}

void test_and_throw(GArray *monkeys, Monkey *monkey, uint64_t *item){
    uint64_t recv_id;
    if (*item % monkey->test_divisor == 0){
        recv_id = monkey->next_id_if_true;
    } else {
        recv_id = monkey->next_id_if_false;
    }
    Monkey *m_recv = g_array_index(monkeys, Monkey *, recv_id);
    g_queue_push_tail(m_recv->items, item);
    return;
}

void m_add_item(Monkey *monkey, uint64_t item){
    uint64_t *x = malloc(sizeof(uint64_t));
    *x = item;
    g_queue_push_tail(monkey->items, x);
    return;
}

uint64_t part1(GArray *monkeys){
    for (uint64_t i = 0; i < 20; ++i){
        for (guint m = 0; m < monkeys->len; ++m){
            Monkey *monkey = g_array_index(monkeys, Monkey *, m);
            for (;;){
                uint64_t *item = g_queue_pop_head(monkey->items);
                if (!item){ // empty queue
                    break;
                }
                inspect_and_get_bored(monkey, item);
                test_and_throw(monkeys, monkey, item);
            }
        }
    }
    uint64_t most_inspected = 0;
    uint64_t second_most_inspected = 0;
    for (guint i = 0; i < monkeys->len; ++i){
        Monkey *m = g_array_index(monkeys, Monkey *, i);
        if (m->inspected > most_inspected) {
            second_most_inspected = most_inspected;
            most_inspected = m->inspected;
        } else if (m->inspected > second_most_inspected){
            second_most_inspected = m->inspected;
        }
    }
    return most_inspected * second_most_inspected;
}

uint64_t part2(GArray *monkeys){
    for (uint64_t i = 0; i < 10000; ++i){
        for (guint m = 0; m < monkeys->len; ++m){
            Monkey *monkey = g_array_index(monkeys, Monkey *, m);
            for (;;){
                uint64_t *item = g_queue_pop_head(monkey->items);
                if (!item){ // empty queue
                    break;
                }
                inspect(monkey, item);
                test_and_throw(monkeys, monkey, item);
            }
        }
    }
    uint64_t most_inspected = 0;
    uint64_t second_most_inspected = 0;
    for (guint i = 0; i < monkeys->len; ++i){
        Monkey *m = g_array_index(monkeys, Monkey *, i);
        if (m->inspected > most_inspected) {
            second_most_inspected = most_inspected;
            most_inspected = m->inspected;
        } else if (m->inspected > second_most_inspected){
            second_most_inspected = m->inspected;
        }
    }
    return most_inspected * second_most_inspected;
}

GArray *setup_monkeys(void){
    // Today is the day I skip parsing and do the input by hand
    // it's still painful

    GArray *monkeys = g_array_new(TRUE, FALSE, sizeof(Monkey *));

    Monkey *m0 = monkey(0, 6, 2, M_Mul, 19, 7);
    m_add_item(m0, 59);
    m_add_item(m0, 74);
    m_add_item(m0, 65);
    m_add_item(m0, 86);
    g_array_append_val(monkeys, m0);

    Monkey *m1 = monkey(1, 2, 0, M_Add, 1, 2);
    m_add_item(m1, 62);
    m_add_item(m1, 84);
    m_add_item(m1, 72);
    m_add_item(m1, 91);
    m_add_item(m1, 68);
    m_add_item(m1, 78);
    m_add_item(m1, 51);
    g_array_append_val(monkeys, m1);

    Monkey *m2 = monkey(2, 6, 5, M_Add, 8, 19);
    m_add_item(m2, 78);
    m_add_item(m2, 84);
    m_add_item(m2, 96);
    g_array_append_val(monkeys, m2);

    Monkey *m3 = monkey(3, 1, 0, M_Mul, 0, 3);
    m_add_item(m3, 97);
    m_add_item(m3, 86);
    g_array_append_val(monkeys, m3);

    Monkey *m4 = monkey(4, 3, 1, M_Add, 6, 13);
    m_add_item(m4, 50);
    g_array_append_val(monkeys, m4);

    Monkey *m5 = monkey(5, 4, 7, M_Mul, 17, 11);
    m_add_item(m5, 73);
    m_add_item(m5, 65);
    m_add_item(m5, 69);
    m_add_item(m5, 65);
    m_add_item(m5, 51);
    g_array_append_val(monkeys, m5);

    Monkey *m6 = monkey(6, 5, 7, M_Add, 5, 5);
    m_add_item(m6, 69);
    m_add_item(m6, 82);
    m_add_item(m6, 97);
    m_add_item(m6, 93);
    m_add_item(m6, 82);
    m_add_item(m6, 84);
    m_add_item(m6, 58);
    m_add_item(m6, 63);
    g_array_append_val(monkeys, m6);

    Monkey *m7 = monkey(7, 3, 4, M_Add, 3, 17);
    m_add_item(m7, 81);
    m_add_item(m7, 78);
    m_add_item(m7, 82);
    m_add_item(m7, 76);
    m_add_item(m7, 79);
    m_add_item(m7, 80);
    g_array_append_val(monkeys, m7);
    return monkeys;
}

int main(void) {

    GArray *monkeys_pt1 = setup_monkeys();
    GArray *monkeys_pt2 = setup_monkeys();

    printf("Part 1: Answer is %llu\n", part1(monkeys_pt1));
    printf("Part 2: Answer is %llu\n", part2(monkeys_pt2));

    for (guint i = 0; i < monkeys_pt1->len; ++i){
        Monkey *m = g_array_index(monkeys_pt1, Monkey *, i);
        monkey_free(m);
        m = g_array_index(monkeys_pt2, Monkey *, i);
        monkey_free(m);
    }
    g_array_free(monkeys_pt1, TRUE);
    g_array_free(monkeys_pt2, TRUE);
    return 0;
}
