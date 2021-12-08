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


struct memory_struct{
    UArray_T memory_segments;
    UArray_T ids;
    uint32_t memory_size;
    uint32_t ids_size;
};

// static void add_to_memory(memory_struct memory, uint32_t index, void *value)
// {
//     //UArray_T segments = memory->memory_segments;
//     UArray_T *tmp = UArray_at(memory->memory_segments, index);
//     *tmp = value;
//     //Seq_put(segments, index, value);
// }

// static void addhi_to_memory(memory_struct memory, void *value)
// {
//     //UArray_T segments = memory->memory_segments;
//     //call add_to_memory with current size
//     //uint32_t size = memory->memory_size;
//     //UArray_T *tmp = UArray_at(memory->memory_segments, memory->memory_size);
//     //*tmp = value;
//     //UArray_T *tmp = UArray_at(memory->memory_segments, size);
//     //*tmp = value;
//     //add_to_memory(memory, size, value);
//     UArray_T *tmp = UArray_at(memory->memory_segments, memory->memory_size);
//     *tmp = value;
//     size++;
//     memory->memory_size = size;
//     //Seq_addhi(segments, value);
// }

/*
uint32_t memory_length(memory_struct memory)
{
    uint32_t size = memory->memory_size;
    return size;
    //UArray_T segments = memory->memory_segments;
    //return (uint32_t)UArray_length(segments);
}
*/

/*
uint32_t ids_length(memory_struct memory)
{
    uint32_t size = memory->ids_size;
    return size;
    //return (uint32_t)UArray_length(ids);
}
*/


// memory_struct initialize_memory()
// {
//     /*malloc memory segments sequence*/
//     /*malloc a single memory segment to start*/
//     memory_struct memory = malloc(sizeof(struct memory_struct));
//     UArray_T id_sequence = UArray_new(pow(2, 25), sizeof(uint32_t));
//     UArray_T segment = UArray_new(pow(2, 25), sizeof(UArray_T));
//     memory->ids = id_sequence;
//     memory->memory_segments = segment;
//     uint32_t memory_size = 1;
//     memory->memory_size = memory_size;
//     uint32_t ids_size = 0;
//     memory->ids_size = ids_size;
//     UArray_T array = UArray_new(pow(2, 25), sizeof(uint32_t));
//     UArray_T *tmp = UArray_at(segment, 0);
//     *tmp = array;
//     return memory;
// }


// void populate_segment_zero(FILE *fp, memory_struct memory)
// {
//     uint32_t counter = 0;
//     UArray_T segments = memory->memory_segments;
//     UArray_T *array = UArray_at(segments, 0);
//     while (1) {
//         uint32_t codeword = get_word(fp);
//         if (feof(fp)) {
//             break;
//         }
//         uint32_t *tmp = UArray_at(*array, counter);
//         *tmp = codeword;
//         //printf("counter: %d\n", counter);
//         counter++;
        
//     }
// }

// static uint32_t get_word(FILE *fp)
// {
//     char tmp = fgetc(fp);
//     uint8_t value = (uint8_t)tmp;
//     uint32_t codeword = 0;
//     codeword = Bitpack_newu(codeword, 8, 24, value);

//     char tmp2 = fgetc(fp);
//     uint8_t value2 = (uint8_t)tmp2;
//     codeword = Bitpack_newu(codeword, 8, 16, value2);

//     char tmp3 = fgetc(fp);
//     uint8_t value3 = (uint8_t)tmp3;
//     codeword = Bitpack_newu(codeword, 8, 8, value3);


//     char tmp4 = fgetc(fp);
//     uint8_t value4 = (uint8_t)tmp4;
//     codeword = Bitpack_newu(codeword, 8, 0, value4);

//     return codeword;
// }

// void *get_memory(memory_struct memory, uint32_t segment)
// {
//     //UArray_T sequence = memory->memory_segments;
//     void *value = UArray_at(memory->memory_segments, segment);
//     return value;
// }

// uint32_t get_lowest_id(memory_struct memory)
// {
//     UArray_T ids = memory->ids;
//     uint32_t size = memory->ids_size;
//     size--;
//     uint32_t *id = UArray_at(ids, size);
//     memory->ids_size = size;
//     return *id;
// }

// void add_id(memory_struct memory, uint32_t value)
// {
//     UArray_T ids = memory->ids;
//     //call add_to_memory with current size
//     uint32_t size = memory->ids_size;
//     uint32_t *tmp = UArray_at(ids, size);
//     *tmp = value;
//     size++;
//     memory->ids_size = size;
    
//     //UArray_addhi(ids, (void *)(uintptr_t)value);
// }

// void free_memory(memory_struct memory)
// {
//     UArray_T main = memory->memory_segments;
//     UArray_T ids = memory->ids;
//     uint32_t size = memory->memory_size;
//     //int length = UArray_length(main);
//     for (uint32_t i = 0; i < size; i++) {
//         UArray_T *value = UArray_at(main, i);
//         if(*value != NULL) {
//             UArray_free(value);
//         }
//     }
//     (void) size;
//     UArray_free(&main);
//     UArray_free(&ids);
//     free(memory);
// }


// uint64_t getu(uint64_t word, unsigned width, unsigned lsb){
//     assert(width <= 64);
//     unsigned hi = lsb + width; /* one beyond the most significant bit */
//     assert(hi <= 64);
//     if (hi == 64 || width == 0) {
//         word = 0;
//     }
//     else {
//         word = word << (64 - hi);
//         word = word >> (64 - width);
//     }
//     return word;
// }

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Must Specify a File");
        exit(EXIT_FAILURE);
    }

    memory_struct memory = malloc(sizeof(struct memory_struct));
    UArray_T id_sequence = UArray_new(pow(2, 25), sizeof(uint32_t));
    UArray_T start_segment = UArray_new(pow(2, 25), sizeof(UArray_T));
    memory->ids = id_sequence;
    memory->memory_segments = start_segment;
    uint32_t memory_size = 1;
    memory->memory_size = memory_size;
    uint32_t ids_size = 0;
    memory->ids_size = ids_size;
    UArray_T start_array = UArray_new(pow(2, 25), sizeof(uint32_t));
    UArray_T *tmp = UArray_at(start_segment, 0);
    *tmp = start_array;

    FILE *fp = fopen(argv[1], "rb");
    uint32_t counter = 0;
    while (1) {
        //uint32_t codeword = get_word(fp);
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
        uint32_t *tmp = UArray_at(start_array, counter);
        *tmp = codeword;
        counter++;
        
    }
    //populate_segment_zero(fp, memory);

    uint32_t registers[8];
    for (int i = 0; i < 8; i++) {
        registers[i] = 0;
    }
    uint32_t i = 0;
    while (1) {
        UArray_T sequence = memory->memory_segments;
        UArray_T *segment_zero = UArray_at(sequence, 0);        
        uint32_t *word = UArray_at(*segment_zero, i);
        uint64_t word2 = *word;
        //int opcode = getu(*word, 4, 28);
        word2 = word2 << 32;
        int opcode = word2 >> 60;
        uint64_t word5 = *word;
        word5 = word5 << 55;
        word5 = word5 >> 61;
        int r1 = word5;

        uint64_t word4 = *word;
        word4 = word4 << 58;
        word4 = word4 >> 61;
        int r2 = word4;
            
            //int r3 = getu(*word, 3, 0);
        uint64_t word3 = *word;
        word3 = word3 << 61;
        word3 = word3 >> 61;
        int r3 = word3;
        if (opcode == 7) {
           break;
        }
        else if (opcode == 12) {
            //int r2 = getu(*word, 3, 3);
            
            uint32_t r2_val = registers[r2];
            uint32_t r3_val = registers[r3];
            if (r2_val == 0) {
                i = r3_val;
            }
            else {
                UArray_T *array = UArray_at(memory->memory_segments, r2_val);
                UArray_T copy = UArray_copy(*array, UArray_length(*array));
                UArray_T *zero_array = UArray_at(sequence, 0);
                UArray_free(zero_array);
                UArray_T *tmp = UArray_at(memory->memory_segments, 0);
                *tmp = copy;
                //add_to_memory(memory, 0, copy);
                i = r3_val;
            }
        }
        else {
            //int r1 = getu(*word, 3, 6);
            //int r2 = getu(*word, 3, 3);
            //int r3 = getu(*word, 3, 0);
            if (opcode == 3) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = ((r2_val + r3_val) % (uint64_t)pow(2, 32));
            }
            if (opcode == 4) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = (r2_val * r3_val) % (uint64_t)pow(2, 32);
            }
            if (opcode == 5) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                registers[r1] = (r2_val / r3_val);
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
                uint64_t word6 = *word;
                word6 = word6 << 36;
                int r = word6 >> 61;
                //int r = getu(*word, 3, 25);
                //uint32_t val = getu(*word, 25, 0);
                uint64_t word7 = *word;
                word7 = word7 << 39;
                word7 = word7 >> 39;
                uint32_t val = word7;
                registers[r] = val;
            }
            if (opcode == 8) {
                uint32_t r3_val = registers[r3];
                UArray_T array = UArray_new(r3_val, sizeof(uint32_t));
                int length = UArray_length(array);
                for (int i = 0; i < length; i++) {
                    uint32_t *tmp = UArray_at(array, i);
                    *tmp = 0;
                }
                if (memory->ids_size == 0) {
                    registers[r2] = memory->memory_size;
                    UArray_T *tmp = UArray_at(memory->memory_segments, memory->memory_size);
                    *tmp = array;
                    memory->memory_size = memory->memory_size + 1;
                    //addhi_to_memory(memory, array);
                }
                else {
                    //uint32_t lowest_id = get_lowest_id(memory);
                    UArray_T ids = memory->ids;
                    uint32_t size = memory->ids_size;
                    size--;
                    uint32_t *id = UArray_at(ids, size);
                    memory->ids_size = size;
                    UArray_T *tmp = UArray_at(memory->memory_segments, *id);
                    *tmp = array;
                    //add_to_memory(memory, *id, array);
                    registers[r2] = *id;
                }
            }
            if (opcode == 9) {;
                uint32_t r3_val = registers[r3];

                UArray_T *array = UArray_at(memory->memory_segments, r3_val);
                UArray_free(array);
                UArray_T *tmp = UArray_at(memory->memory_segments, r3_val);
                *tmp = NULL;
                //add_to_memory(memory, r3_val, NULL);
                UArray_T ids = memory->ids;
                //call add_to_memory with current size
                uint32_t size = memory->ids_size;
                uint32_t *tmp2 = UArray_at(ids, size);
                *tmp2 = r3_val;
                size++;
                memory->ids_size = size;
                //add_id(memory, r3_val);
            }
            if (opcode == 1) {
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                UArray_T *array = UArray_at(memory->memory_segments, r2_val);
                uint32_t *value = UArray_at(*array, r3_val);
                registers[r1] = *value;
            }
            if (opcode == 2) {
                uint32_t r1_val = registers[r1];
                uint32_t r2_val = registers[r2];
                uint32_t r3_val = registers[r3];
                UArray_T *array = UArray_at(memory->memory_segments, r1_val);
                uint32_t *value = UArray_at(*array, r2_val);
                *value = r3_val;
            }
            i++;
        }
    }
    fclose(fp);
    
    UArray_T main = memory->memory_segments;
    UArray_T ids = memory->ids;
    uint32_t size = memory->memory_size;
    for (uint32_t i = 0; i < size; i++) {
        UArray_T *value = UArray_at(main, i);
        if(*value != NULL) {
            UArray_free(value);
        }
    }
    (void) size;
    UArray_free(&main);
    UArray_free(&ids);
    free(memory);
    
    return 0;
}
