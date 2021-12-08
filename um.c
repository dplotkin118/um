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

//UArray_T memory_segments;
//UArray_T ids;
uint32_t memory_size;
uint32_t ids_size;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Must Specify a File");
        exit(EXIT_FAILURE);
    }
    uint32_t *ids = malloc(33554432 * sizeof(uint32_t));
    uint32_t **memory_segments = malloc(33554432 * sizeof(uint32_t *));
    memory_size = 1;
    ids_size = 0;
    uint32_t *start_segment = malloc(33554432 * sizeof(uint32_t));
    //memory_segments[0] =NULL;
    //printf("val: %lu\n", (long unsigned)memory_segments[0]);
    memory_segments[0] = start_segment;
    (void) start_segment;
    FILE *fp = fopen(argv[1], "rb");
    uint32_t counter = 0;
    while (1) {
        char tmp5 = fgetc(fp);
        uint8_t value = (uint8_t)tmp5;
        uint32_t codeword = 0;
        codeword = Bitpack_newu(codeword, 8, 24, value);

        char tmp2 = fgetc(fp);
        uint8_t value2 = (uint8_t)tmp2;
        codeword = Bitpack_newu(codeword, 8, 16, value2);

        char tmp3 = fgetc(fp);
        uint8_t value3 = (uint8_t)tmp3;
        codeword = Bitpack_newu(codeword, 8, 8, value3);

        char tmp4 = fgetc(fp);
        uint8_t value4 = (uint8_t)tmp4;
        codeword = Bitpack_newu(codeword, 8, 0, value4);

        if (feof(fp)) {
            break;
        }

        memory_segments[0][counter] = codeword;
        counter++;     
    }

    uint32_t registers[8];
    for (int i = 0; i < 8; i++) {
        registers[i] = 0;
    }
    uint32_t i = 0;
    while (1) {       
        uint32_t word = memory_segments[0][i];
        uint64_t word2 = word;
        word2 = word2 << 32;

        int opcode = word2 >> 60;

        uint64_t word5 = word;
        word5 = word5 << 55;
        word5 = word5 >> 61;

        int r1 = word5;

        uint64_t word4 = word;
        word4 = word4 << 58;
        word4 = word4 >> 61;

        int r2 = word4;
        
        uint64_t word3 = word;
        word3 = word3 << 61;
        word3 = word3 >> 61;

        int r3 = word3;
        if (opcode == 7) {
           break;
        }
        else if (opcode == 12) {
            uint32_t r2_val = registers[r2];
            uint32_t r3_val = registers[r3];
            if (r2_val == 0) {
                i = r3_val;
            }
            else {
                //UArray_T *array = UArray_at(memory_segments, r2_val);
                //memory_segments[r2_val];
                uint32_t segment_size = (sizeof(memory_segments) / sizeof(memory_segments[r2_val]));
                uint32_t copy[segment_size];
                for (uint32_t k = 0; k < segment_size; k++) {
                    copy[k] = memory_segments[r2_val][k];
                }
                
                //UArray_T copy = UArray_copy(*array, UArray_length(*array));
                //UArray_T *zero_array = UArray_at(sequence, 0);
                free(memory_segments[0]);
                //UArray_free(zero_array);
                //UArray_T *tmp = UArray_at(memory_segments, 0);
                memory_segments[0] = copy;
                //*tmp = copy;
                i = r3_val;
            }
        }
        else {
            if (opcode == 3) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = registers[r2] + registers[r3];
            }
            if (opcode == 4) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = registers[r2] * registers[r3];
            }
            if (opcode == 5) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = r2_val / r3_val;
            }
            if (opcode == 6) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] =  ~(r2_val & r3_val);
            }
            if (opcode == 0) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                if (r3_val != 0) {
                    registers[r1] = r2_val;
                }
            }
            if (opcode == 10) {
                uint32_t r3_val = registers[r3];
                assert(r3_val <= 255);
                printf("%c", r3_val);
            }
            if (opcode == 11) {
                uint32_t r3_val = getchar();
                if (r3_val <= 255) {
                    registers[r3] = r3_val;
                }
                else if (r3_val == (uint32_t)EOF) {
                    registers[r3] = ~0;
                }
            }
            if (opcode == 13) {
                uint64_t word6 = word;
                word6 = word6 << 36;
                int r = word6 >> 61;
                uint64_t word7 = word;
                word7 = word7 << 39;
                word7 = word7 >> 39;
                uint32_t val = word7;
                registers[r] = val;
            }
            if (opcode == 8) {
                uint32_t r3_val = registers[r3];
                //UArray_T array = UArray_new(r3_val, sizeof(uint32_t));
                uint32_t array[r3_val];
                //int length = UArray_length(array);
                for (uint32_t i = 0; i < r3_val; i++) {
                    //uint32_t *tmp = UArray_at(array, i);
                    array[i] = 0;
                    //*tmp = 0;
                }
                if (ids_size == 0) {
                    registers[r2] = memory_size;
                    memory_segments[memory_size] = array;
                    //UArray_T *tmp = UArray_at(memory_segments, memory_size);
                    //*tmp = array;
                    memory_size++;
                }
                else {
                    ids_size--;
                    //uint32_t *id = UArray_at(ids, ids_size);
                    uint32_t id = ids[ids_size];
                    //UArray_T *tmp = UArray_at(memory_segments, *id);
                    //*tmp = array;
                    memory_segments[id] = array;
                    registers[r2] = id;
                }
            }
            if (opcode == 9) {;
                uint32_t r3_val = registers[r3];
                //UArray_T *array = UArray_at(memory_segments, r3_val);
                //UArray_free(array);
                free(memory_segments[r3_val]);
                //UArray_T *tmp = UArray_at(memory_segments, r3_val);
                //*tmp = NULL;
                memory_segments[r3_val] = NULL;
                ids[ids_size] = r3_val;
                // uint32_t *tmp2 = UArray_at(ids, ids_size);
                // *tmp2 = r3_val;
                ids_size++;
            }
            if (opcode == 1) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                // UArray_T *array = UArray_at(memory_segments, r2_val);
                // memory_segments[r2_val]
                // uint32_t *value = UArray_at(*array, r3_val);
                uint32_t value = memory_segments[r2_val][r3_val];
                registers[r1] = value;
            }
            if (opcode == 2) {
                uint32_t r1_val = registers[r1];
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                //UArray_T *array = UArray_at(memory_segments, r1_val);
                //uint32_t *value = UArray_at(*array, r2_val);
                //*value = r3_val;
                memory_segments[r1_val][r2_val] = r3_val;
            }
            i++;
        }
    }
    fclose(fp);
    
    uint32_t size = memory_size;
    for (uint32_t i = 0; i < size; i++) {
        //UArray_T *value = UArray_at(memory_segments, i);
        if(memory_segments[i] != NULL) {
            //UArray_free(value);
            free(memory_segments[i]);
        }
    }
    (void) size;
    free(memory_segments);
    //UArray_free(&memory_segments); //main
    free(ids);
    //UArray_free(&ids);

    return 0;
}