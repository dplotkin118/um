/*
 *     instruction.h
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Header file of instruction, which outlines the 
 *     functions that handle the instruction specified by
 *     the opcode in a 32-bit word
 */

#include "memory.h"

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

Um_opcode get_opcode(uint32_t word);

/*
 * call_function
 * Purpose: 
 *     Central function for calling the right instruction 
 *     based off of opcode found in the 32-bit instruction
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     opcode: the opcode found in the 32-bit word
 *     word: the 32-bit word instruction retrieved from memory
 */
void call_function(registers_struct registers, memory_struct memory,
                                                Um_opcode opcode, 
                                                uint32_t word);

/*
 * load_program
 * Purpose: 
 *     Sets the current 0th memory segment to a copy
 *     of the segment at the value of register B
 *     and sets the program counter to the (value 
 *     at register C)th word in the new segment 
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     word: the 32-bit word instruction retrieved from memory
 */
uint32_t load_program(registers_struct registers, memory_struct memory,
                                                         uint32_t word);