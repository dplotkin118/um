/*
 *     instruction.c
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Implementation of instruction, which holds all the
 *     functions that handle the instruction specified by
 *     the opcode in a 32-bit word
 */

#include "instruction.h"

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
Um_opcode get_opcode(uint32_t word)
{
    Um_opcode opcode = Bitpack_getu(word, 4, 28);
    return opcode;

}

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
void load_val(registers_struct registers, uint32_t word)
{

    int r = Bitpack_getu(word, 3, 25);
    uint32_t val = Bitpack_getu(word, 25, 0);

    set_register(registers, r, val);
}

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
void call_function(registers_struct registers,
memory_struct memory, Um_opcode opcode, uint32_t word)
{
    if (opcode == ADD) {
        add(registers, word);
    }
    if (opcode == MUL) {
        multiply(registers, word);
    }
    if (opcode == DIV) {
        divide(registers, word);
    }
    if (opcode == NAND) {
        bitwise_nand(registers, word);
    }
    if (opcode == CMOV) {
        conditional_move(registers, word);
    }
    if (opcode == OUT) {
        output_register(registers, word);
    }
    if (opcode == IN) {
        input_register(registers, word);
    }
    if (opcode == LV) {
        load_val(registers, word);
    }
    if (opcode == ACTIVATE) {
        map_segment(registers, memory, word);
    }
    if (opcode == INACTIVATE) {
        unmap_segment(registers, memory, word);
    }
    if (opcode == SLOAD) {
        segmented_load(registers, memory, word);
    }
    if (opcode == SSTORE) {
        segmented_store(registers, memory, word);
    }
}

/*
 * add
 * Purpose:
 *     Sets the register A described in the 32-bit word
 *     to the sum of the values in registers B and C
 *     also described in the word
 *     Mods to ensure result is in range
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void add(registers_struct registers, uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    set_register(registers, r1, (r2_val + r3_val)
     % (uint64_t)pow(2, 32));
}

/*
 * multiply
 * Purpose:
 *     Sets the register A described in the 32-bit word
 *     to the product of the values in registers B and C
 *     also described in the word
 *     Mods to ensure result is in range
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void multiply(registers_struct registers, uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    set_register(registers, r1, (r2_val * r3_val)
     % (uint64_t)pow(2, 32));
}

/*
 * divide
 * Purpose:
 *     Sets the register A described in the 32-bit word
 *     to the division of the values in registers B and C
 *     also described in the word
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void divide(registers_struct registers, uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    set_register(registers, r1, r2_val / r3_val);
}

/*
 * bitwise_nand
 * Purpose:
 *     Sets the register A described in the 32-bit word
 *     to the bitwise not of the bitwise and operation of
 *     registers B and C also described in the word
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void bitwise_nand(registers_struct registers, uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    set_register(registers, r1, ~(r2_val & r3_val));
}

/*
 * conditional_move
 * Purpose:
 *     Sets the register A described in the 32-bit word
 *     to the value in register B if register C is not 0
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void conditional_move(registers_struct registers, uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    if (r3_val != 0) {
        set_register(registers, r1, r2_val);
    }
}

/*
 * output_register
 * Purpose:
 *     Prints the value in register C described in
 *     the 32-bit word as a char to output stream
 *     if its value is less than or equal to 255
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved from memory
 */
void output_register(registers_struct registers, uint32_t word)
{
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r3_val = get_register(registers, r3);
    assert(r3_val <= 255);
    printf("%c", r3_val);
}

/*
 * input_register
 * Purpose:
 *     Sets the value in register C described in the 32-bit
 *     word from a value received from the
 *     input stream provided that value is
 *     less than or equal to 255
 *     If the end of input is signaled, sets the
 *     value to be the bitwise not of 0 instead
 * Arguments:
 *     registers: the struct containing the 8 registers
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void input_register(registers_struct registers, uint32_t word)
{
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r3_val = getchar();
    if (r3_val <= 255) {
        set_register(registers, r3, r3_val);
    }
    else if (r3_val == (uint32_t)EOF) {
        set_register(registers, r3, ~0);
    }
}

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
 *     word: the 32-bit word instruction retrieved
 *     from memory
 */
void map_segment(registers_struct registers, memory_struct memory,
                                                    uint32_t word)
{
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r3_val = get_register(registers, r3);
    UArray_T array = UArray_new(r3_val, sizeof(uint32_t));
    int length = UArray_length(array);
    for (int i = 0; i < length; i++) {
        uint32_t *tmp = UArray_at(array, i);
        *tmp = 0;
    }
    if (ids_length(memory) == 0) {
        set_register(registers, r2, memory_length(memory));
        addhi_to_memory(memory, array);
    }
    else {
        uint32_t lowest_id = get_lowest_id(memory);
        add_to_memory(memory, lowest_id, array);
        set_register(registers, r2, lowest_id);
    }
}

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
                                                        uint32_t word)
{
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r3_val = get_register(registers, r3);

    UArray_T array = (UArray_T)get_memory(memory, r3_val);
    UArray_free(&array);

    add_to_memory(memory, r3_val, NULL);
    add_id(memory, r3_val);
}

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
                                                     uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    UArray_T array = (UArray_T)get_memory(memory, r2_val);
    uint32_t *value = (uint32_t *)UArray_at(array, r3_val);
    set_register(registers, r1, *value);
}

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
                                                        uint32_t word)
{
    int r1 = Bitpack_getu(word, 3, 6);
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r1_val = get_register(registers, r1);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    UArray_T array = (UArray_T)get_memory(memory, r1_val);
    uint32_t *value = UArray_at(array, r2_val);
    *value = r3_val;

}

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
                                                        uint32_t word)
{
    int r2 = Bitpack_getu(word, 3, 3);
    int r3 = Bitpack_getu(word, 3, 0);
    uint32_t r2_val = get_register(registers, r2);
    uint32_t r3_val = get_register(registers, r3);
    if (r2_val == 0) {
        return r3_val;
    }
    UArray_T array = (UArray_T)get_memory(memory, r2_val);
    UArray_T copy = UArray_copy(array, UArray_length(array));
    //UArray_free(&array);
    add_to_memory(memory, 0, copy);
    return r3_val;
}
