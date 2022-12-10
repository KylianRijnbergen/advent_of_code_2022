#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define REGISTER_HISTORY 6

typedef struct Cpu Cpu;

typedef struct Cpu {
	int reg;
	int* register_history;
	int clock;
} Cpu;


Cpu* build_cpu(void);
void tick(Cpu* cpu, int tick_count);
void noop(Cpu* cpu);
void addx(Cpu* cpu, int count);
void save_register(Cpu* cpu);
void update_registers(Cpu* cpu);
void parse_line(Cpu* cpu, char* line);
int parse_buffer(char* buff);
bool sprite_visible(int register_x, int current_pixel);
void draw_pixel(char character);

int main(int argc, char* argv[]) {
	Cpu* cpu = build_cpu();
	
	FILE* file;
	file = fopen("input.txt", "r");
	
	const int MAX_LINE_LENGTH = 15;
	char line[MAX_LINE_LENGTH];
	
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		parse_line(cpu, line);
	}
	return 0;
}

Cpu* build_cpu(void) {
	Cpu* cpu = malloc(sizeof(Cpu));
	cpu->reg = 1;
	cpu->register_history = malloc(sizeof(int) * REGISTER_HISTORY);
	for (unsigned int i = 0; i < REGISTER_HISTORY; i++) {
		cpu->register_history[i] = 0;
	}
	cpu->clock = 0;
	return cpu;
}

void tick(Cpu* cpu, int tick_count) {
	for (unsigned int i = 0; i < tick_count; i++) {
		char c;
		if (cpu->clock % 40 == 0 && cpu->clock > 0) {
			printf("\n");
		}
		
		if (sprite_visible(cpu->reg, cpu->clock % 40)) {
			c = '#';
		}
		else {
			c = '.';
		}
		draw_pixel(c);
		cpu->clock += 1;
		update_registers(cpu);
	}
}

void noop(Cpu* cpu) {
	tick(cpu, 1);
}

void addx(Cpu* cpu, int count) {
	tick(cpu, 2);
	cpu->reg += count;
}

void save_register(Cpu* cpu) {
	cpu->register_history[cpu->clock / 40] = cpu->reg;
}

void update_registers(Cpu* cpu) {
	if (cpu->clock % 40 == 20) {
		save_register(cpu);
	}
}

void parse_line(Cpu* cpu, char* line) {
	if (line[0] == 'a') {
		const int BUFF_SIZE = 5;
		char count_buffer[BUFF_SIZE];
		for (int characterindex = 0; characterindex < BUFF_SIZE; characterindex++) {
			count_buffer[characterindex] = line[characterindex + BUFF_SIZE];
		}
		int operation_value = parse_buffer(count_buffer);
		addx(cpu, operation_value);
	}
	else if (line[0] == 'n') {
		noop(cpu);
	}
}

int parse_buffer(char* buff) {
	char* newline_index;
	newline_index = strchr(buff, '\n');
	*newline_index = '\0';
	return atoi(buff);
}

bool sprite_visible(int register_x, int current_pixel) {
	return abs(current_pixel - register_x) <= 1;
}

void draw_pixel(char character) {
	putchar(character);
}