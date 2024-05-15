#ifndef DECODE_H
#define DECODE_H


struct PCB_t {
	uint8_t OPCode;
	uint8_t operand1;
	uint8_t operand2;
};


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


#endif
