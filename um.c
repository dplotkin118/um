/*
 *     um.c
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Implementation of um, which is the central file
 *     that grabs 32-bit *word instructions from a .um
 *     file, initializes the memory and register structs,
 *     and then handles all the instructions through using
 *     the correct functions
 */

#include "um.h"
#include "inttypes.h"


typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

uint64_t getu(uint64_t word, unsigned width, unsigned lsb){
    assert(width <= 64);
    unsigned hi = lsb + width; /* one beyond the most significant bit */
    assert(hi <= 64);
    if (hi == 64 || width == 0) {
        word = 0;
    }
    else {
        word = word << (64 - hi);
        word = word >> (64 - width);
    }
    return word;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Must Specify a File");
        exit(EXIT_FAILURE);
    }
    memory_struct memory = initialize_memory();
    FILE *fp = fopen(argv[1], "rb");
    populate_segment_zero(fp, memory);
    registers_struct registers = initialize_registers();
    (void) registers;
    uint32_t i = 0;
    while (1) {
        UArray_T segment_zero = (UArray_T)get_memory(memory, 0);
        uint32_t *word = (uint32_t *)UArray_at(segment_zero, i);
        Um_opcode opcode = getu(*word, 4, 28);
        if (opcode == HALT) {
            break;
        }
        else if (opcode == LOADP) {
            int r2 = getu(*word, 3, 3);
            int r3 = getu(*word, 3, 0);
            uint32_t r2_val = get_register(registers, r2);
            uint32_t r3_val = get_register(registers, r3);
            if (r2_val == 0) {
                i = r3_val;
            }
            else {
                UArray_T array = (UArray_T)get_memory(memory, r2_val);
                UArray_T copy = UArray_copy(array, UArray_length(array));
                UArray_T zero_array = (UArray_T)get_memory(memory, 0);
                UArray_free(&zero_array);
                add_to_memory(memory, 0, copy);
                i = r3_val;
            }
        }
        else {
            int r1 = getu(*word, 3, 6);
            int r2 = getu(*word, 3, 3);
            int r3 = getu(*word, 3, 0);
            if (opcode == ADD) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                set_register(registers, r1, (r2_val + r3_val)
                                                    % (uint64_t)pow(2, 32));
            }
            if (opcode == MUL) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                set_register(registers, r1, (r2_val * r3_val)
                                                    % (uint64_t)pow(2, 32));
            }
            if (opcode == DIV) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                set_register(registers, r1, r2_val / r3_val);
            }
            if (opcode == NAND) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                set_register(registers, r1, ~(r2_val & r3_val));
            }
            if (opcode == CMOV) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                if (r3_val != 0) {
                    set_register(registers, r1, r2_val);
                }
            }
            if (opcode == OUT) {
                uint32_t r3_val = get_register(registers, r3);
                assert(r3_val <= 255);
                printf("%c", r3_val);
            }
            if (opcode == IN) {
                uint32_t r3_val = getchar();
                if (r3_val <= 255) {
                    set_register(registers, r3, r3_val);
                }
                else if (r3_val == (uint32_t)EOF) {
                    set_register(registers, r3, ~0);
                }
            }
            if (opcode == LV) {
                int r = getu(*word, 3, 25);
                uint32_t val = getu(*word, 25, 0);

                set_register(registers, r, val);
            }
            if (opcode == ACTIVATE) {
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
            if (opcode == INACTIVATE) {;
                uint32_t r3_val = get_register(registers, r3);

                UArray_T array = (UArray_T)get_memory(memory, r3_val);
                UArray_free(&array);

                add_to_memory(memory, r3_val, NULL);
                add_id(memory, r3_val);
            }
            if (opcode == SLOAD) {
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                UArray_T array = (UArray_T)get_memory(memory, r2_val);
                uint32_t *value = (uint32_t *)UArray_at(array, r3_val);
                set_register(registers, r1, *value);
            }
            if (opcode == SSTORE) {
                uint32_t r1_val = get_register(registers, r1);
                uint32_t r2_val = get_register(registers, r2);
                uint32_t r3_val = get_register(registers, r3);
                UArray_T array = (UArray_T)get_memory(memory, r1_val);
                uint32_t *value = UArray_at(array, r2_val);
                *value = r3_val;
            }
            i++;
        }
    }
    fclose(fp);
    free_memory(memory);
    free_registers(registers);
    return 0;
}
