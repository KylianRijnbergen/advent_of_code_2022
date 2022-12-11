#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"

#define MAX_ITEMS 100
#define MONKEY_COUNT 8
#define ROUNDS 10000
#define LCM (long long) (17 * 19 * 7 * 11 * 13 * 3 * 5 * 2)

typedef struct Monkey Monkey;

typedef struct Monkey {
	int monkey_id;
	long long inspects;
	long long* inventory;
	int num_items_in_inventory;
	long long test_value;
	int target_if_true;
	int target_if_false;
	int turns_taken;
} Monkey;

Monkey* monkey_create(int monkey_id, long long test_value, int target_if_true, int target_if_false);
void monkey_catch(Monkey* monkey, long long item);
void monkey_throw(Monkey* throwing_monkey, Monkey* catching_monkey);
void inspect(Monkey* monkeys[MONKEY_COUNT], int current_monkey);

int main(int argc, char* argv[]) {
	/* Initialize monkeys with empty inventories */
	Monkey* monkeys[MONKEY_COUNT];
	long long test_values[MONKEY_COUNT] = { 17, 19, 7, 11, 13, 3, 5, 2 };
	int target_true_values[MONKEY_COUNT] = { 2, 7, 4, 6, 6, 1, 5, 2 };
	int target_false_values[MONKEY_COUNT] = { 7, 0, 3, 4, 5, 0, 1, 3 };
	for (unsigned int i = 0; i < MONKEY_COUNT; i++) {
		monkeys[i] = monkey_create(i, test_values[i], target_true_values[i], target_false_values[i]);
	}

	/* Add items to their inventories */
	/* Monkey 0 has 4 items */
	monkey_catch(monkeys[0], 83);
	monkey_catch(monkeys[0], 97);
	monkey_catch(monkeys[0], 95);
	monkey_catch(monkeys[0], 67);

	/* Monkey 1 has 6 items */
	monkey_catch(monkeys[1], 71);
	monkey_catch(monkeys[1], 70);
	monkey_catch(monkeys[1], 79);
	monkey_catch(monkeys[1], 88);
	monkey_catch(monkeys[1], 56);
	monkey_catch(monkeys[1], 70);

	/* Monkey 2 has 8 items */
	monkey_catch(monkeys[2], 98);
	monkey_catch(monkeys[2], 51);
	monkey_catch(monkeys[2], 51);
	monkey_catch(monkeys[2], 63);
	monkey_catch(monkeys[2], 80);
	monkey_catch(monkeys[2], 85);
	monkey_catch(monkeys[2], 84);
	monkey_catch(monkeys[2], 95);

	/* Monkey 3 has 5 items */
	monkey_catch(monkeys[3], 77);
	monkey_catch(monkeys[3], 90);
	monkey_catch(monkeys[3], 82);
	monkey_catch(monkeys[3], 80);
	monkey_catch(monkeys[3], 79);

	/* Monkey 4 only has a single item */
	monkey_catch(monkeys[4], 68);

	/* Monkey 5 has 2 items */
	monkey_catch(monkeys[5], 60);
	monkey_catch(monkeys[5], 94);

	/* Monkey 6 has 3 items */
	monkey_catch(monkeys[6], 81);
	monkey_catch(monkeys[6], 51);
	monkey_catch(monkeys[6], 85);

	/* Monkey 7 has 7 items */
	monkey_catch(monkeys[7], 98);
	monkey_catch(monkeys[7], 81);
	monkey_catch(monkeys[7], 63);
	monkey_catch(monkeys[7], 65);
	monkey_catch(monkeys[7], 84);
	monkey_catch(monkeys[7], 71);
	monkey_catch(monkeys[7], 84);

	for (unsigned int round_id = 0; round_id < ROUNDS; round_id++) {
		for (int monkey_id = 0; monkey_id < MONKEY_COUNT; monkey_id++) {
			inspect(monkeys, monkey_id);
		}
	}

	long long inspect_values[MONKEY_COUNT];
	for (unsigned int i = 0; i < MONKEY_COUNT; i++) {
		inspect_values[i] = monkeys[i]->inspects;
	}

	quicksort_ll(inspect_values, MONKEY_COUNT);
	printf("The monkey business value is %lld\n", inspect_values[MONKEY_COUNT - 2] * inspect_values[MONKEY_COUNT - 1]);
	return 0;
}

Monkey* monkey_create(int monkey_id, long long test_value, int target_if_true, int target_if_false) {
	Monkey* monkey = malloc(sizeof(Monkey));
	monkey->monkey_id = monkey_id;
	monkey->inspects = 0;
	monkey->inventory = malloc(sizeof(long long) * MAX_ITEMS);
	monkey->num_items_in_inventory = 0;
	monkey->test_value = test_value;
	monkey->target_if_true = target_if_true;
	monkey->target_if_false = target_if_false;
	monkey->turns_taken = 0;
	return monkey;
}

void monkey_catch(Monkey* monkey, long long item) {
	monkey->inventory[monkey->num_items_in_inventory] = item % LCM;
	monkey->num_items_in_inventory++;
}

void monkey_throw(Monkey* throwing_monkey, Monkey* catching_monkey) {
	long long item_value = throwing_monkey->inventory[throwing_monkey->num_items_in_inventory - 1];
	throwing_monkey->num_items_in_inventory--;
	monkey_catch(catching_monkey, item_value);
}

void inspect(Monkey* monkeys[MONKEY_COUNT], int current_monkey) {
	Monkey* monkey = monkeys[current_monkey];
	/* Inspection and operation */
	int id = monkey->monkey_id;
	while (monkey->num_items_in_inventory > 0) {
		monkey->inspects++;
		if (id == 0) {
			monkey->inventory[monkey->num_items_in_inventory - 1] *= 19;
		}
		else if (id == 1) {
			monkey->inventory[monkey->num_items_in_inventory - 1] += 2;
		}
		else if (id == 2) {
			monkey->inventory[monkey->num_items_in_inventory - 1] += 7;
		}
		else if (id == 3) {
			monkey->inventory[monkey->num_items_in_inventory - 1] += 1;
		}
		else if (id == 4) {
			monkey->inventory[monkey->num_items_in_inventory - 1] *= 5;
		}
		else if (id == 5) {
			monkey->inventory[monkey->num_items_in_inventory - 1] += 5;
		}
		else if (id == 6) {
			monkey->inventory[monkey->num_items_in_inventory - 1] =
				monkey->inventory[monkey->num_items_in_inventory - 1]
				* monkey->inventory[monkey->num_items_in_inventory - 1];
		}
		else if (id == 7) {
			monkey->inventory[monkey->num_items_in_inventory - 1] += 3;
		}

		/* Testing worry level and throwing */
		if (monkey->inventory[monkey->num_items_in_inventory - 1] % monkey->test_value == 0) {
			monkey_throw(monkey, monkeys[monkey->target_if_true]);
		}
		else {
			monkey_throw(monkey, monkeys[monkey->target_if_false]);
		}
	}
}
