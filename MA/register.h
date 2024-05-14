#ifndef _REGISTERS_C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define NUM_GPRS 64


struct reg_t {
	uint8_t GPRS[NUM_GPRS];
	uint8_t SREG;
	uint8_t PC;
};

struct reg_t* init_reg() {
	struct reg_t* reg = (struct reg_t*)malloc(sizeof(struct reg_t));
	if (reg == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}
	return reg;
}

void kill_reg(struct reg_t* reg) {
	free(reg);
}

void pretty_print_reg(struct reg_t* reg) {
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
		printf("0x%04x-0x%04x: 0x0000\n", start, end);
	}

	printf("----------SREG----------\n");
	printf("0x%02x\n", reg->SREG);

	printf("----------PC----------\n");
	printf("0x%02x\n", reg->PC);
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

#endif
