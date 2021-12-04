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
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        set_register(registers, r1, (r2_val + r3_val)
                                                    % (uint64_t)pow(2, 32));
    }
    if (opcode == MUL) {
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        set_register(registers, r1, (r2_val * r3_val)
                                                    % (uint64_t)pow(2, 32));
    }
    if (opcode == DIV) {
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        set_register(registers, r1, r2_val / r3_val);
    }
    if (opcode == NAND) {
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        set_register(registers, r1, ~(r2_val & r3_val));
    }
    if (opcode == CMOV) {
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        if (r3_val != 0) {
            set_register(registers, r1, r2_val);
        }
    }
    if (opcode == OUT) {
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r3_val = get_register(registers, r3);
        assert(r3_val <= 255);
        printf("%c", r3_val);
    }
    if (opcode == IN) {
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r3_val = getchar();
        if (r3_val <= 255) {
            set_register(registers, r3, r3_val);
        }
        else if (r3_val == (uint32_t)EOF) {
            set_register(registers, r3, ~0);
        }
    }
    if (opcode == LV) {
        int r = Bitpack_getu(word, 3, 25);
        uint32_t val = Bitpack_getu(word, 25, 0);

        set_register(registers, r, val);
    }
    if (opcode == ACTIVATE) {
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
    if (opcode == INACTIVATE) {
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r3_val = get_register(registers, r3);

        UArray_T array = (UArray_T)get_memory(memory, r3_val);
        UArray_free(&array);

        add_to_memory(memory, r3_val, NULL);
        add_id(memory, r3_val);
    }
    if (opcode == SLOAD) {
        int r1 = Bitpack_getu(word, 3, 6);
        int r2 = Bitpack_getu(word, 3, 3);
        int r3 = Bitpack_getu(word, 3, 0);
        uint32_t r2_val = get_register(registers, r2);
        uint32_t r3_val = get_register(registers, r3);
        UArray_T array = (UArray_T)get_memory(memory, r2_val);
        uint32_t *value = (uint32_t *)UArray_at(array, r3_val);
        set_register(registers, r1, *value);
    }
    if (opcode == SSTORE) {
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
}
