#ifndef PARSE_ASM_H
#define PARSE_ASM_H

// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t parse_OPCode(char *token) {
	if (strcmp(token, "ADD") == 0) {
		return 0b0000;
	} else if (strcmp(token, "SUB") == 0) {
		return 0b0001;
	} else if (strcmp(token, "MUL") == 0) {
		return 0b0010;
	} else if (strcmp(token, "LDI") == 0) {
		return 0b0011;
	} else if (strcmp(token, "BEQZ") == 0) {
		return 0b0100;
	} else if (strcmp(token, "AND") == 0) {
		return 0b0101;
	} else if (strcmp(token, "OR") == 0) {
		return 0b0110;
	} else if (strcmp(token, "JR") == 0) {
		return 0b0111;
	} else if (strcmp(token, "SLC") == 0) {
		return 0b1000;
	} else if (strcmp(token, "SRC") == 0) {
		return 0b1001;
	} else if (strcmp(token, "LB") == 0) {
		return 0b1010;
	} else if (strcmp(token, "SB") == 0) {
		return 0b1011;
	} else {
		printf("Error: Invalid OPCode: %s\n", token);
		exit(1);
	}
}


void read_asm(uint16_t *inst_mem, char *path) {
	// open the file and check if its there
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Error: File not found\n");
		exit(1);
	}

	// read the file line by line
	char line[128];
	int i = 0;
	while (fgets(line, sizeof(line), fp)) {
		// remove the newline character
		line[strcspn(line, "\n")] = 0;

		// init the vars that will be stored
		uint16_t inst = 0; // the total lenght of the inst is 16 bits
		uint8_t OPCode = 0; // the opcode is only four bits so we will be using LSB
		uint8_t operand1 = 0; // the operand is only 6 bits so we will be using LSB
		uint8_t operand2 = 0; // the operand is only 6 bits so we will be using LSB

		// split the line into tokens
		char *token = strtok(line, " ");
		int j = 0;
		while (token != NULL) {
			if (j == 0) {
				// parse the OPCode (error handling is done in the function)
				OPCode = parse_OPCode(token);
			} else if (j == 1) {
				// check if the operand is a valid hex number
				if (strspn(token, "0123456789abcdef") != strlen(token)) {
					printf("Error: Invalid operand\n");
					exit(1);
				}

				// parse the first operand
				operand1 = (int)strtol(token, NULL, 16);

				// check if the operand is within the range of 6 bits
				if (operand1 > 0b111111) {
					printf("Error: Operand out of range\n");
					exit(1);
				}
			} else if (j == 2) {
				// check if the operand is a valid hex number
				if (strspn(token, "0123456789abcdef") != strlen(token)) {
					printf("Error: Invalid operand\n");
					exit(1);
				}
				// parse the second operand
				operand2 = (int)strtol(token, NULL, 16);

				// check if the operand is within the range of 6 bits
				if (operand2 > 0b111111) {
					printf("Error: Operand out of range\n");
					exit(1);
				}
			} else {
				printf("Error: Too many operands\n");
				exit(1);
			}
			
			// get the next token
			token = strtok(NULL, " ");
			j++;
		}

		// create the instruction
		inst = (OPCode << 12) | (operand1 << 6) | operand2;

		// store the instruction in the memory
		set_inst(inst_mem, i, inst);

		// increment the counter
		i++;
	}

	// set the last instruction to have an opcode of 0b1111
	set_inst(inst_mem, i, 0xF000);

	// close the file
	fclose(fp);
}


#endif
