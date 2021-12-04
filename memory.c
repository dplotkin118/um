/*
 *     memory.c
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Implementation of memory, which holds the memory
 *     struct and all the helper functions for initializing
 *     this struct and accessing and setting the segmented
 *     memory
 *     Additionally holds the register struct and all of the
 *     helper functions for initializng this struct and
 *     accessing and setting the registers
 */

#include "memory.h"
#include "instruction.h"

struct registers_struct{
    uint32_t *registers;
};

struct memory_struct{
    Seq_T memory_segments;
    Seq_T ids;
};


// uint64_t newu(uint64_t word, unsigned width, unsigned lsb,
//                       uint64_t value)
// {
//         assert(width <= 64);
//         unsigned hi = lsb + width; /* one beyond the most significant bit */
//         assert(hi <= 64);
//         assert(value >> width == 0);
//         return (word >> hi) << hi                 /* high part */
//                 | ((word << (64 - lsb)) >> (64 - lsb))  /* low part  */
//                 | (value << lsb);                   /* new part  */
// }
/*
 * add_to_memory
 * Purpose:
 *      Adds a value to a given segment in memory
 * Arguments:
 *      memory: the struct containing the memory segments
 *      index: a uint32_t representing the segment of memory
 *      value: the value to be added to the segment of memory
 */
void add_to_memory(memory_struct memory, uint32_t index, void *value)
{
    Seq_T segments = memory->memory_segments;
    Seq_put(segments, index, value);
}

/*
 * addhi_to_memory
 * Purpose:
 *      Adds a value to the last memory segment, thereby expanding
 *      the number of segments in memory
 * Arguments:
 *      memory: the struct containing the memory segments
 *      value: the value to be added to the segment of memory
 */
void addhi_to_memory(memory_struct memory, void *value)
{
    Seq_T segments = memory->memory_segments;
    Seq_addhi(segments, value);
}

/*
 * memory_length
 * Purpose:
 *      finds the length of the memory sequence
 * Arguments:
 *      memory: the struct containing the memory segments
 * Output:
 *      a uint32_t representing a bitpacked word
 */
uint32_t memory_length(memory_struct memory)
{
    Seq_T segments = memory->memory_segments;
    return (uint32_t)Seq_length(segments);
}

/*
 * ids_length
 * Purpose:
 *      Gets the length of the sequence containing all unused ids
 * Arguments:
 *      memory: the struct containing the memory segments and ids
 */
uint32_t ids_length(memory_struct memory)
{
    Seq_T ids = memory->ids;
    return (uint32_t)Seq_length(ids);
}

/*
 * initialize_memory
 * Purpose:
 *      allocates memory and initializes the memory struct
 *
 * Arguments:
 *      None
 * Output:
 *      an initialized memory struct
 */
memory_struct initialize_memory()
{
    /*malloc memory segments sequence*/
    /*malloc a single memory segment to start*/
    memory_struct memory = malloc(sizeof(struct memory_struct));
    Seq_T id_sequence = Seq_new(0);
    Seq_T segment = Seq_new(0);
    memory->ids = id_sequence;
    memory->memory_segments = segment;
    UArray_T array = UArray_new(pow(2, 25), sizeof(uint32_t));
    Seq_addlo(segment, array);
    return memory;
}

/*
 * set_register
 * Purpose:
 *      Sets register name in the register struct with a given value
 * Arguments:
 *      r: the struct containing the registers array
 *      name: a name of the register between r0 and r7
 *      value: a uint32_t to be put in the register
 */
void set_register(registers_struct r, int name, uint32_t value)
{
    assert(name < 8 && name >= 0);
    (r->registers)[name] = value;
    (void) name;
}

/*
 * get_register
 * Purpose:
 *      Gets register with name name in the register struct
 * Arguments:
 *      name: a name of a register between r0 and r7
 *      r: the struct containing the registers
 * Output:
 *      a uint32_t representing a register
 */
uint32_t get_register(registers_struct r, int name)
{
    assert(name < 8 && name >= 0);
    uint32_t var = (r->registers)[name];
    (void)name;
    return var;
}

/*
 * initialize_registers
 * Purpose:
 *      Allocates memory and initializes all registers to 0
 * Arguments:
 *      None
 * Output:
 *      an initialized registers struct
 */
registers_struct initialize_registers()
{
    uint32_t *array = malloc(8 * sizeof(uint32_t));
    registers_struct result = malloc(sizeof(registers_struct));
    result->registers = array;
    for (int i = 0; i < 8; i++) {
        array[i] = 0;
    }
    return result;
}

/*
 * populate_segment_zero
 * Purpose:
 *      reads in from a file and populates the 0 segment with instructions
 * Arguments: a file and a memory struct
 */
void populate_segment_zero(FILE *fp, memory_struct memory)
{
    uint32_t counter = 0;
    Seq_T segments = memory->memory_segments;
    UArray_T array = Seq_get(segments, 0);
    while (1) {
        uint32_t codeword = get_word(fp);
        if (feof(fp)) {
            break;
        }
        uint32_t *tmp = UArray_at(array, counter);
        *tmp = codeword;
        counter++;
    }
}

/*
 * get_word
 * Purpose:
 *      a helper function for populate_segment_zero that reads in a string
 *      from the file and returns it as a bitpacked word
 * Arguments:
 *      a file
 * Output:
 *      a uint32_t representing a bitpacked word
 */
uint32_t get_word(FILE *fp)
{
    char tmp = fgetc(fp);
    uint8_t value = (uint8_t)tmp;
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

    return codeword;
}

/*
 * get_memory
 * Purpose:
 *      retrieves the value at a given segment of memory
 * Arguments:
 *      memory: the struct containing the memory segments
 *      segment: a uint32_t representing a segment number
 * Output:
 *      a void* representing the value at the given segment
 */
void *get_memory(memory_struct memory, uint32_t segment)
{
    Seq_T sequence = memory->memory_segments;
    void *value = Seq_get(sequence, segment);
    return value;
}

/*
 * get_lowest_id
 * Purpose:
 *      Gets the first value from the sequence of unused ids
 * Arguments:
 *      memory: the struct containing the memory segments and ids
 */
uint32_t get_lowest_id(memory_struct memory)
{
    Seq_T ids = memory->ids;
    uint32_t id = (uint32_t)(uintptr_t)Seq_remlo(ids);
    return id;
}

/*
 * add_id
 * Purpose:
 *      Adds an id to the struct containing all unused ids
 * Arguments:
 *      memory: the struct containing the memory segments and ids
 *      value: a uint32_t representing the value of an unused segment
 */
void add_id(memory_struct memory, uint32_t value)
{
    Seq_T ids = memory->ids;
    Seq_addhi(ids, (void *)(uintptr_t)value);
}


/*
 * free_memory
 * Purpose:
 *      Frees the memory struct and all values within it
 * Arguments:
 *      memory: the struct containing the memory segments and ids
 */
void free_memory(memory_struct memory)
{
    Seq_T main = memory->memory_segments;
    Seq_T ids = memory->ids;
    int length = Seq_length(main);
    for (int i = 0; i < length; i++) {
        UArray_T value = Seq_get(main, i);
        if(value != NULL) {
            UArray_free(&value);
        }
    }
    Seq_free(&main);
    Seq_free(&ids);
    free(memory);
}

/*
 * free_registers
 * Purpose:
 *      Frees the registers struct and all values within it
 * Arguments:
 *      r: the struct containing the registers array
 */
void free_registers(registers_struct r)
{
    uint32_t *array = r->registers;
    free(array);
    free(r);
}
