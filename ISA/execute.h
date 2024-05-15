#ifndef EXECUTE_H
#define EXECUTE_H

// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint16_t concat(uint8_t a, uint8_t b) {
	return (a << 8) | b;
}


void handle_addition(struct PCB_t pcb, struct reg_t *reg) {
	// init some vars
	uint32_t mask = 0x000000ff;
	uint32_t expanded_a;
	uint32_t expanded_b;

	// check for the carry flag
	expanded_a = (reg->GPRS[pcb.operand1] & mask);
	expanded_b = (reg->GPRS[pcb.operand2] & mask);
	if ((expanded_a + expanded_b) > mask) {
		set_reg_flag(reg, 'C', 1);
	} else {
		set_reg_flag(reg, 'C', 0);
	}

	// Do the Addition
	reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] + reg->GPRS[pcb.operand2];
}


void execute(struct PCB_t pcb, struct reg_t *reg, uint8_t *data_mem) {
	// match each case and execute the instruction
	switch (pcb.OPCode) {
		case 0://ADD
			handle_addition(pcb, reg);
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
			reg->PC = concat(reg->GPRS[pcb.operand1], reg->GPRS[pcb.operand2]);
			break;
		case 8://SLC
			reg->GPRS[pcb.operand1] = reg->GPRS[pcb.operand1] << pcb.operand2; //| reg->GPRS[pcb.operand1] >> (8 - pcb.operand2);
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


#endif
