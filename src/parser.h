
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

uint32_t cs_parse_name(cs_t* cs);

#endif /* __PSDK_PARSER_HEADER__ */
