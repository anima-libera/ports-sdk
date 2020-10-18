
#include "code.h"

void code_initialize(code_t* code)
{
	code->inst_array = NULL;
	code->len = 0;
	code->cap = 0;
	code->first_port_index = -1;
}

void code_cleanup(code_t* code)
{
	if (code->inst_array != NULL)
	{
		/* TODO
		 * cleanup instructions before */
		free(code->inst_array);
	}
}
