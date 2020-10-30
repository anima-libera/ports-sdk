
#ifndef __PSDK_PARSER_HEADER__
#define __PSDK_PARSER_HEADER__

#include "code.h"
#include <stdio.h>

typedef struct cs_t cs_t;
struct cs_t
{
	FILE* file;
};

void cs_init_from_filepath(cs_t* cs, const char* filepath);
void cs_cleanup(cs_t* cs);

char cs_peek_char(cs_t* cs);
void cs_discard_char(cs_t* cs);

void cs_skip_skippable(cs_t* cs);
int cs_parse_number(cs_t* cs);

/* Prases and pops the next instruction of the given cs (if any), and append it
 * to the given code.
 * Returns 0 if an instruction was parsed and popped,
 * returns 1 if the end of the cs was encountered, and
 * TODO. */
int cs_parse_inst(cs_t* cs, code_t* code);

/* TODO
 * make an enum for the return value of cs_parse_inst ! */

#endif /* __PSDK_PARSER_HEADER__ */
