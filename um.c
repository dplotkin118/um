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
        uint32_t *instruction = (uint32_t *)UArray_at(segment_zero, i);
        Um_opcode opcode = get_opcode(*instruction);
        if (opcode == HALT) {
            break;
        }
        else if (opcode == LOADP) {
            uint32_t val = load_program(registers, memory, *instruction);
            i = val;
        }
        else {
            call_function(registers, memory, opcode, *instruction);
            i++;
        }
    }
    fclose(fp);
    free_memory(memory);
    free_registers(registers);
    return 0;
}
