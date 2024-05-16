#ifndef _REGISTERS_C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_GPRS 64


struct reg_t {
	uint8_t GPRS[NUM_GPRS];
	uint8_t SREG;
	uint8_t PC;
};

struct reg_t* init_reg() {
	struct reg_t* reg = (struct reg_t*)calloc(1, sizeof(struct reg_t));
	if (reg == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}
	return reg;
}

void kill_reg(struct reg_t* reg) {
	free(reg);
}

uint8_t get_reg_flag(struct reg_t* reg, char flag) {
	switch (flag) {
		case 'C':
			return (reg->SREG & 0b00010000) >> 4;
		case 'V':
			return (reg->SREG & 0b00001000) >> 3;
		case 'N':
			return (reg->SREG & 0b00000100) >> 2;
		case 'S':
			return (reg->SREG & 0b00000010) >> 1;
		case 'Z':
			return (reg->SREG & 0b00000001) >> 0;
		default:
			printf("Error: Invalid flag\n");
			exit(1);
	}
}

void set_reg_flag(struct reg_t* reg, char flag, uint8_t value) {
	if (value != 0 && value != 1) {
		printf("Error: Invalid value: %d\n", value);
		exit(1);
	}
	switch (flag) {
		case 'C':
			if (value == 1) {
				reg->SREG |= 0b00010000;
			} else {
				reg->SREG &= 0b11101111;
			}
			break;
		case 'V':
			if (value == 1) {
				reg->SREG |= 0b00001000;
			} else {
				reg->SREG &= 0b11110111;
			}
			break;
		case 'N':
			if (value == 1) {
				reg->SREG |= 0b00000100;
			} else {
				reg->SREG &= 0b11111011;
			}
			break;
		case 'S':
			if (value == 1) {
				reg->SREG |= 0b00000010;
			} else {
				reg->SREG &= 0b11111101;
			}
			break;
		case 'Z':
			if (value == 1) {
				reg->SREG |= 0b00000001;
			} else {
				reg->SREG &= 0b11111110;
			}
			break;
		default:
			printf("Error: Invalid flag\n");
			exit(1);
	}
}

void _pretty_print_SREG(struct reg_t* reg) {
	printf("C: %d\n", get_reg_flag(reg, 'C'));
	printf("V: %d\n", get_reg_flag(reg, 'V'));
	printf("N: %d\n", get_reg_flag(reg, 'N'));
	printf("S: %d\n", get_reg_flag(reg, 'S'));
	printf("Z: %d\n", get_reg_flag(reg, 'Z'));
}

struct reg_t* copy_reg(struct reg_t* reg) {
	struct reg_t* new_reg = init_reg();
	for (int i = 0; i < NUM_GPRS; i++) {
		new_reg->GPRS[i] = reg->GPRS[i];
	}
	new_reg->SREG = reg->SREG;
	new_reg->PC = reg->PC;
	return new_reg;
}

void pretty_print_reg(struct reg_t* reg) {
	printf("===============REGISTERS==============\n");
	printf("----------GPRS----------\n");
	int start = 0;
	int end = 0;
	for (int i = 0; i < NUM_GPRS; i++) {
		if (reg->GPRS[i] != 0) {
			if (start != end){
				printf("0x%02x - 0x%02x: 0x00\n", start, end);
			}
			printf("0x%02x: 0x%02x\n", i, reg->GPRS[i]);
			start = i + 1;
			end = i + 1;
		} else {
			end++;
		}
	}
	// print the remaining zeros
	if (start != end) {
		printf("0x%02x-0x%02x: 0x00\n", start, end);
	}

	printf("----------SREG----------\n");
	_pretty_print_SREG(reg);

	printf("----------PC----------\n");
	printf("0x%02x\n", reg->PC);
	printf("======================================\n");
}

void pretty_print_diff_reg(struct reg_t *old_reg, struct reg_t *new_reg) {
	printf("=============DIFF REGISTERS==========-\n");
	bool diff = false;
	for (int i = 0; i < NUM_GPRS; i++) {
		if (old_reg->GPRS[i] != new_reg->GPRS[i]) {
			if (!diff) {
				printf("----------GPRS----------\n");
			}
			diff = true;
			printf("0x%02x: 0x%02x -> 0x%02x\n", i, old_reg->GPRS[i], new_reg->GPRS[i]);
		}
	}
	if (old_reg->SREG != new_reg->SREG) {
		printf("----------SREG----------\n");
		diff = true;
		printf("SREG: 0x%02x -> 0x%02x\n", old_reg->SREG, new_reg->SREG);
	}

	if (!diff) {
		printf("No difference\n");
	}
	printf("======================================\n");
}

#endif
