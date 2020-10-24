
#include "code.h"
#include <stdio.h>
#include <assert.h>

/* ABREVIATION TABLE
 * alloc allocate, allocated
 * cap   capacity
 * dyn   dynamic
 * init  initialize, initialization, initial
 * inst  instruction
 * la    link array
 * len   length
 * pa    port array
 * pn    port name
 * psdk  ports software development kit
 */

int main(void)
{
	code_t code;
	code_init(&code);
	
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

	printf("a\n");

	code_cleanup(&code);
	return 0;
}
