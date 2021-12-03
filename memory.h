/*
 *     memory.h
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Header file of memory, which typedefs the memory
 *     struct and outlines all the helper functions 
 *     for initializing this struct and accessing 
 *     and setting the segmented memory
 *     Additionally holds the register struct and all of the
 *     helper functions for initializng this struct and 
 *     accessing and setting the registers
 */

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "seq.h"
#include "uarray.h"
#include <bitpack.h>
#include "fmt.h"


typedef struct registers_struct *registers_struct;
typedef struct memory_struct *memory_struct;

/*
 * set_register
 * Purpose: 
 *      Sets register name in the register struct with a given value
 * Arguments: 
 *      r: the struct containing the registers array
 *      name: a name of the register between r0 and r7
 *      value: a uint32_t to be put in the register
 */
void set_register(registers_struct r, int name, uint32_t value);

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
uint32_t get_register(registers_struct r, int name);

/*
 * initialize_registers
 * Purpose: 
 *      Allocates memory and initializes all registers to 0
 * Arguments:
 *      None
 * Output: 
 *      an initialized registers struct
 */
registers_struct initialize_registers();

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
memory_struct initialize_memory();

/*
 * populate_segment_zero
 * Purpose: 
 *      reads in from a file and populates the 0 segment with instructions
 * Arguments: a file and a memory struct
 */
void populate_segment_zero(FILE *fp, memory_struct memory);

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
uint32_t get_word(FILE *fp);

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
void *get_memory(memory_struct memory, uint32_t segment);

/*
 * memory_length
 * Purpose: 
 *      finds the length of the memory sequence
 * Arguments: 
 *      memory: the struct containing the memory segments
 * Output:
 *      a uint32_t representing a bitpacked word
 */
uint32_t memory_length(memory_struct memory);

/*
 * add_to_memory
 * Purpose: 
 *      Adds a value to a given segment in memory
 * Arguments: 
 *      memory: the struct containing the memory segments
 *      index: an int representing the segment of memory
 *      value: the value to be added to the segment of memory
 */
void add_to_memory(memory_struct memory, uint32_t index, void *value);

/*
 * addhi_to_memory
 * Purpose: 
 *      Adds a value to the last memory segment, thereby expanding
 *      the number of segments in memory
 * Arguments: 
 *      memory: the struct containing the memory segments
 *      value: the value to be added to the segment of memory
 */
void addhi_to_memory(memory_struct memory, void *value);

/*
 * get_lowest_id
 * Purpose: 
 *      Gets the first value from the sequence of unused ids
 * Arguments: 
 *      memory: the struct containing the memory segments and ids
 */
uint32_t get_lowest_id(memory_struct memory);

/*
 * add_id
 * Purpose: 
 *      Adds an id to the struct containing all unused ids
 * Arguments: 
 *      memory: the struct containing the memory segments and ids
 *      value: a uint32_t representing the value of an unused segment
 */
void add_id(memory_struct memory, uint32_t value);

/*
 * ids_length
 * Purpose: 
 *      Gets the length of the sequence containing all unused ids
 * Arguments: 
 *      memory: the struct containing the memory segments and ids
 */
uint32_t ids_length(memory_struct memory);

/*
 * free_registers
 * Purpose: 
 *      Frees the registers struct and all values within it
 * Arguments: 
 *      r: the struct containing the registers array
 */
void free_registers(registers_struct r);

/*
 * free_memory
 * Purpose: 
 *      Frees the memory struct and all values within it
 * Arguments: 
 *      memory: the struct containing the memory segments and ids
 */
void free_memory(memory_struct memory);

#endif