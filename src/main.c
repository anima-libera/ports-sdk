
#include "code.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{
	code_t code;
	code_initialize(&code);
	
	for (int i = 0; i < 20; i++)
	{
		inst_t* inst = code_alloc_inst(&code);
		inst->type = INST_PORT;
		inst->dyn.port.name = i;
	}

	for (int i = 0; i < 20; i++)
	{
		assert((int)code.inst_array[i].dyn.port.name == i);
	}

	code_cleanup(&code);
	return 0;
}
