/*
 *     um.h
 *     by Arvind Pillai, Daniel Plotkin, 11/24/21
 *     um
 *
 *     Header file of um, which includes memory.h 
 *     that in turn includes instruction.h, thus
 *     tying all of the methods together for use
 *     by the implementation of the universal
 *     machine
 */

#ifndef UM_INCLUDED
#define UM_INCLUDED
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
 
uint32_t get_word(FILE *fp);
//static inline void addhi_to_memory(memory_struct memory, void *value);
//static inline void add_to_memory(memory_struct memory, uint32_t index, void *value);

#endif