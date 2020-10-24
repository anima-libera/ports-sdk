
#include "code.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void inst_cleanup(inst_t* inst)
{
	(void)inst;
}

void code_init(code_t* code)
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
		for (int i = 0; i < code->len; i++)
		{
			inst_cleanup(&code->inst_array[i]);
		}
		free(code->inst_array);
	}
}

inst_t* code_alloc_inst(code_t* code)
{
	if (code->len >= code->cap)
	{
		int new_cap = code->cap < 16 ? 16 : code->cap * 2;
		inst_t* new_inst_array =
			realloc(code->inst_array, new_cap * sizeof(inst_t));
		if (new_inst_array == NULL)
		{
			/* TODO
			 * handle the error */
			fprintf(stderr, "error 1\n");
		}
		code->inst_array = new_inst_array;
		code->cap = new_cap;
	}
	return &code->inst_array[code->len++];
}

#if 0
void code_add_inst(code_t* code, inst_t* inst)
{
	inst_t* allocated_inst = code_alloc_inst(code);
	memcpy(allocated_inst, inst, sizeof(inst_t));
}
#endif
