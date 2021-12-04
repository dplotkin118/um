/*
 *     um.c
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Implementation of um, which is the central file
 *     that grabs 32-bit word instructions from a .um
 *     file, initializes the memory and register structs,
 *     and then handles all the instructions through using
 *     the correct functions
 */

#include "um.h"
#include "inttypes.h"
#include <bitpack.h>
#include "instruction.h"

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
        Um_opcode opcode = get_opcode(*word);
        if (opcode == HALT) {
            break;
        }
        else if (opcode == LOADP) {
            int r2 = Bitpack_getu(*word, 3, 3);
            int r3 = Bitpack_getu(*word, 3, 0);
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
            call_function(registers, memory, opcode, *word);
            i++;
        }
    }
    fclose(fp);
    free_memory(memory);
    free_registers(registers);
    return 0;
}
