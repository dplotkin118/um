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

/*
 * load_val
 * Purpose: 
 *     Sets the register described in the 32-bit word
 *     to a 25-bit unsigned value described in the
 *     word
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void load_val(registers_struct registers, uint32_t word);

/*
 * get_opcode
 * Purpose: 
 *     Retrieves opcode from 32-bit word by
 *     accessing the 4 most significant bits
 * Arguments:
 *     word: the 32-bit word instruction retrieved from memory
 * Returns:
 *     the opcode in enum format corresponding to an instruction
 */
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
 * add
 * Purpose: 
 *     Sets the register A described in the 32-bit word
 *     to the sum of the values in registers B and C
 *     also described in the word
 *     Mods to ensure result is in range
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void add(registers_struct registers, uint32_t word);

/*
 * multiply
 * Purpose: 
 *     Sets the register A described in the 32-bit word
 *     to the product of the values in registers B and C
 *     also described in the word
 *     Mods to ensure result is in range
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void multiply(registers_struct registers, uint32_t word);

/*
 * divide
 * Purpose: 
 *     Sets the register A described in the 32-bit word
 *     to the division of the values in registers B and C
 *     also described in the word
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void divide(registers_struct registers, uint32_t word);

/*
 * bitwise_nand
 * Purpose: 
 *     Sets the register A described in the 32-bit word
 *     to the bitwise not of the bitwise and operation of
 *     registers B and C also described in the word
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void bitwise_nand(registers_struct registers, uint32_t word);

/*
 * conditional_move
 * Purpose: 
 *     Sets the register A described in the 32-bit word
 *     to the value in register B if register C is not 0
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void conditional_move(registers_struct registers, uint32_t word);

/*
 * output_register
 * Purpose: 
 *     Prints the value in register C described in the 32-bit
 *     word as a char to output stream if its value is less
 *     than or equal to 255
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void output_register(registers_struct registers, uint32_t word);

/*
 * input_register
 * Purpose: 
 *     Sets the value in register C described in the 32-bit
 *     word from a value received from the input stream provided
 *     that value is less than or equal to 255
 *     If the end of input is signaled, sets the value to be 
 *     the bitwise not of 0 instead
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void input_register(registers_struct registers, uint32_t word);

/*
 * map_segment
 * Purpose: 
 *     Creates a new segment with a number of words
 *     initialized as 0s equal to the value in 
 *     register C (specified by 32-bit word)
 *     Maps segment with an id that is placed in
 *     register B
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     word: the 32-bit word instruction retrieved from memory
 */
void map_segment(registers_struct registers, memory_struct memory, 
                                                uint32_t word);

/*
 * unmap_segment
 * Purpose: 
 *     Unmaps segment with id specified by value
 *     in register C; id can be reused by future
 *     segment mappings
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     word: the 32-bit word instruction retrieved from memory
 */
void unmap_segment(registers_struct registers, memory_struct memory, 
                                                        uint32_t word);

/*
 * segmented_load
 * Purpose: 
 *     Sets the value of register A specified by word
 *     to the 32-bit word at the value of register C's
 *     position in the value of register B's segment
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     word: the 32-bit word instruction retrieved from memory
 */
void segmented_load(registers_struct registers, memory_struct memory,
                                                uint32_t word);

/*
 * segmented_store
 * Purpose: 
 *     Sets the value of the 32-bit word at the 
 *     value of register B's position in the 
 *     value of register A's segment to the value
 *     stored in register C
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     memory: the struct containing the memory segments
 *     word: the 32-bit word instruction retrieved from memory
 */
void segmented_store(registers_struct registers, memory_struct memory, 
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