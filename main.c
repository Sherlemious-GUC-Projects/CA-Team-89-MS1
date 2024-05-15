// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// local header files
#include "MA/data_memory.h"
#include "MA/instruction_memory.h"
#include "MA/register.h"


// definitions
#define ASM_PATH "asm.txt"


//global variables
uint8_t *data_mem;
uint16_t *inst_mem;
struct reg_t *reg;


// init all structs
struct PCB_t {
	uint8_t OPCode;
	uint8_t operand1;
	uint8_t operand2;
};


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


void read_asm() {
	// open the file and check if its there
	FILE *fp = fopen(ASM_PATH, "r");
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
				// parse the OPCode
				OPCode = parse_OPCode(token);
			} else if (j == 1) {
				// parse the first operand
				operand1 = (int)strtol(token, NULL, 16);
			} else if (j == 2) {
				// parse the second operand
				operand2 = (int)strtol(token, NULL, 16);
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

	// close the file
	fclose(fp);
}


uint16_t fetch() {
	// get the instruction at the PC
	uint16_t curr_inst = get_inst(inst_mem, reg->PC);
	
	// increment the PC
	reg->PC++;
	return curr_inst;
}


struct PCB_t decode(uint16_t inst) {
	// init the PCB
	struct PCB_t pcb;
	
	// get the OPCode
	pcb.OPCode = (inst & 0b1111000000000000) >> 12;
	
	// get the first operand
	pcb.operand1 = (inst & 0b0000111111000000) >> 6;
	
	// get the second operand
	pcb.operand2 = (inst & 0b0000000000111111);
	
	return pcb;
}


void execute(struct PCB_t pcb) {
	/*
		ADD
		SUB
		MUL
		LDI
		BEQZ
		AND
		OR 
		JR 
		SLC
		SRC
		LB 
		SB 
	*/
	// match each case and execute the instruction
	switch (pcb.OPCode) {
		case 0://ADD
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] + reg->GPRS[pcb.operand2];
			break;
		case 1://SUB
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] - reg->GPRS[pcb.operand2];
			break;
		case 2://MUL
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] * reg->GPRS[pcb.operand2];
			break;
		case 3://LDI
			reg->GPRS[pcb.operand1] = pcb.operand2;
			break;
		case 4://BEQZ
			if (reg->GPRS[pcb.operand1] == 0) {
				reg->PC = reg->PC + 1 + pcb.operand2;
			}
			break;
		case 5://AND
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] & reg->GPRS[pcb.operand2];
			break;
		case 6://OR
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] | reg->GPRS[pcb.operand2];
			break;
		case 7://JR
			reg->PC = reg->GPRS[pcb.operand1] || reg->GPRS[pcb.operand2];
			break;
		case 8://SLC
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] << pcb.operand2 | reg->GPRS[pcb.operand1] >> (8 - pcb.operand2);
			break;
		case 9://SRC
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] >> pcb.operand2 | reg->GPRS[pcb.operand1] << (8 - pcb.operand2);
			break;
		case 10://LB
			reg->GPRS[pcb.operand1] = get_data(data_mem, reg->GPRS[pcb.operand2]);
			break;
		case 11://SB
			set_data(data_mem, reg->GPRS[pcb.operand2], reg->GPRS[pcb.operand1]);
			break;
		default:
			printf("Error: Invalid OPCode\n");
			exit(1);
		}
}


int main() {
	// init all the MA
	data_mem = init_data_mem();
	inst_mem = init_inst_mem();
	reg = init_reg();

	// ~~~ BETA ~~~ //
	reg->GPRS[0] = 0x01;
	reg->GPRS[1] = 0x02;

	// read the assembly code into the memory
	read_asm();

	// fetch the instructions
	uint16_t curr_inst = fetch();

	// decode the instruction
	struct PCB_t pcb = decode(curr_inst);

	// execute the instruction
	execute(pcb);
	
	pretty_print_reg(reg);

	// kill all the MA
	kill_data_mem(data_mem);
	kill_inst_mem(inst_mem);
	kill_reg(reg);
	return 0;
}

