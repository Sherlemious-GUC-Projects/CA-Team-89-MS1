#ifndef EXECUTE_H
#define EXECUTE_H

// global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint16_t concat(uint8_t a, uint8_t b) {
	return (a << 8) | b;
}


void handle_carry(struct PCB_t pcb, struct reg_t *reg) {
	// check the OPCode
	if (pcb.OPCode != 0x0) {
		printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
		exit(1);
	}

	// init the mask
	uint32_t mask = 0x000000ff;

	// expand the Rs
	uint32_t expanded_R1 = (pcb.R1 & mask);
	uint32_t expanded_R2 = (pcb.R2 & mask);

	// check for the carry flag
	bool carry_flag = ((expanded_R1 + expanded_R2) > mask);

	// set the carry flag
	set_reg_flag(reg, 'C', carry_flag);
}


void handle_overflow(struct PCB_t pcb, struct reg_t *reg) {
	// check the OPCode
	if (pcb.OPCode == 0x0) { // if the OPCode is ADD
		// init the mask
		uint8_t mask = 0x80;

		// fetch the results (quickly)
		uint8_t resultant = reg->GPRS[pcb.operand1];

		// check if the two Rs have the same sign
		bool same_sign = ((pcb.R1 & mask) == (pcb.R2 & mask));

		// check if the result has the same sign as the Rs
		bool result_sign = ((resultant & mask) == (pcb.R1 & mask));

		// check for overflow
		bool overflow = (same_sign && !result_sign);

		// set the overflow flag
		set_reg_flag(reg, 'V', overflow);
	} else if (pcb.OPCode == 0x1) { // if the OPCode is SUB
		// init the mask
		uint8_t mask = 0x80;

		// fetch the results (quickly)
		uint8_t resultant = reg->GPRS[pcb.operand1];

		// check if the two Rs have different signs
		bool diff_sign = ((pcb.R1 & mask) != (pcb.R2 & mask));


		// check if the result has the same sign as R2
		bool result_sign = ((resultant & mask) == (pcb.R2 & mask));

		// check for overflow
		bool overflow = (diff_sign && result_sign);

		// set the overflow flag
		set_reg_flag(reg, 'V', overflow);
	}else{
		printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
		exit(1);
	
	}
}

void handle_negative(struct PCB_t pcb, struct reg_t *reg) {
	// check the OPCode
	if (
		pcb.OPCode == 0x0 ||
		pcb.OPCode == 0x1 ||
		pcb.OPCode == 0x2 ||
		pcb.OPCode == 0x5 ||
		pcb.OPCode == 0x6 ||
		pcb.OPCode == 0x8 ||
		pcb.OPCode == 0x9
	) { // if the OPCode is ADD or SUB or MUL or AND or OR or SLC or SRC
		// init the mask
		uint8_t mask = 0x80;

		// fetch the results (quickly)
		uint8_t resultant = reg->GPRS[pcb.operand1];

		// check if the result is negative
		bool negative = (resultant & mask);

		// set the negative flag
		set_reg_flag(reg, 'N', negative);
	} else {
		printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
		exit(1);
	}
}


void handle_sign(struct PCB_t pcb, struct reg_t *reg) {
	// check the OPCode
	if (
		pcb.OPCode == 0x0 ||
		pcb.OPCode == 0x1 
	) { // if the OPCode is ADD or SUB 
		// check if the result is zero
		bool sign = get_reg_flag(reg, 'N') ^ get_reg_flag(reg, 'V');

		// set the sign flag
		set_reg_flag(reg, 'S', sign);
	} else {
		printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
		exit(1);
	}
}


void handle_zero(struct PCB_t pcb, struct reg_t *reg) {
	// check the OPCode
	if (
		pcb.OPCode == 0x0 ||
		pcb.OPCode == 0x1 ||
		pcb.OPCode == 0x2 ||
		pcb.OPCode == 0x5 ||
		pcb.OPCode == 0x6 ||
		pcb.OPCode == 0x8 ||
		pcb.OPCode == 0x9
	) { // if the OPCode is ADD or SUB or MUL or AND or OR or SLC or SRC
		// fetch the results (quickly)
		uint8_t resultant = reg->GPRS[pcb.operand1];

		// check if the result is zero
		bool zero = (resultant == 0);

		// set the zero flag
		set_reg_flag(reg, 'Z', zero);
	} else {
		printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
		exit(1);
	}
}


void execute(struct PCB_t pcb, struct reg_t *reg, uint8_t *data_mem) {
	// match each case and execute the instruction
	switch (pcb.OPCode) {
		case 0://ADD
			// Do the Addition
			reg->GPRS[pcb.operand1] = pcb.R1 + pcb.R2;

			// Handle the carry bit
			handle_carry(pcb, reg);

			// handle the overflow bit
			handle_overflow(pcb, reg);

			// handle the negative bit
			handle_negative(pcb, reg);

			// handle the sign bit
			handle_sign(pcb, reg);

			// handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 1://SUB
			// Do the Subtraction
			reg->GPRS[pcb.operand1] = pcb.R1 - pcb.R2;

			// Handle the overflow bit
			handle_overflow(pcb, reg);

			// Handle the negative bit
			handle_negative(pcb, reg);

			// Handle the sign bit
			handle_sign(pcb, reg);

			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 2://MUL
			// Do the Multiplication
			reg->GPRS[pcb.operand1] = pcb.R1 * pcb.R2;

			// Handle the negative bit
			handle_negative(pcb, reg);

			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 3://LDI
			// Do the Load Immediate
			reg->GPRS[pcb.operand1] = pcb.IMM;
			break;
		case 4://BEQZ
			// Do the Branch if Equal to Zero
			if (pcb.R1 == 0) {
				reg->PC = reg->PC + 1 + pcb.IMM;
			}
			break;
		case 5://AND
			// Do the AND
			reg->GPRS[pcb.operand1] = pcb.R1 & pcb.R2;

			// Handle the negative bit
			handle_negative(pcb, reg);

			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 6://OR
			// Do the OR
			reg->GPRS[pcb.operand1] = pcb.R1 | pcb.R2;

			// Handle the negative bit
			handle_negative(pcb, reg);
			
			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 7://JR
			// Do the Jump Register
			reg->PC = concat(pcb.R1, pcb.R2);
			break;
		case 8://SLC
			// Do the Shift Left Circular
			reg->GPRS[pcb.operand1] = pcb.R1 << pcb.IMM | pcb.R1 >> (8 - pcb.IMM);

			// Handle the negative bit
			handle_negative(pcb, reg);

			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 9://SRC
			// Do the Shift Right Circular
			reg->GPRS[pcb.operand1] = pcb.R1 >> pcb.IMM | pcb.R1 << (8 - pcb.IMM);

			// Handle the negative bit
			handle_negative(pcb, reg);

			// Handle the zero bit
			handle_zero(pcb, reg);
			break;
		case 10://LB
			// Do the Load Byte
			reg->GPRS[pcb.operand1] = get_data(data_mem, pcb.IMM);
			break;
		case 11://SB
			// Do the Store Byte
			set_data(data_mem, pcb.IMM, pcb.R1);
			break;
		case 15://HALT
			printf("Warning: trying to execute a halt command\n");
			break;
		default:
			printf("Error: Invalid OPCode: %d\n", pcb.OPCode);
			exit(1);
	}
}


#endif
