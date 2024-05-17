#ifndef DECODE_H
#define DECODE_H

void decode(uint16_t inst, PCB_t *pcb, reg_t *reg) {
	// get the OPCode
	pcb->OPCode = (inst & 0b1111000000000000) >> 12;
	
	// get the first operand
	pcb->operand1 = (inst & 0b0000111111000000) >> 6;
	
	// get the second operand
	pcb->operand2 = (inst & 0b0000000000111111);

	// get the format
	if ((pcb->OPCode >= 0 && pcb->OPCode <=2) || (pcb->OPCode >= 5 && pcb->OPCode <= 7)) {
		pcb->is_R_format = true;
	} else {
		pcb->is_R_format = false;
	}

	// get the R1, R2, IMM
	pcb->R1 = reg->GPRS[pcb->operand1];
	if (pcb->is_R_format) {
		pcb->R2 = reg->GPRS[pcb->operand2];
		pcb->IMM = 0;
	} else {
		pcb->IMM = pcb->operand2;
		pcb->R2 = 0;
	}
}

void pretty_print_pcb(PCB_t *pcb) {
	printf("==================PCB=================\n");
	printf("OPCode: 0x%02x\n", pcb->OPCode);
	printf("Operand1: 0x%02x\n", pcb->operand1);
	printf("Operand2: 0x%02x\n", pcb->operand2);
	printf("R1: 0x%02x\n", pcb->R1);
	printf("R2: 0x%02x\n", pcb->R2);
	printf("IMM: 0x%02x\n", pcb->IMM);
	printf("is_R_format: %s\n", pcb->is_R_format ? "true" : "false");
	printf("======================================\n");
}

#endif
