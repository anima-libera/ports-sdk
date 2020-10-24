
#include "space.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

port_t* space_init(space_t* space, const code_t* code, uint32_t init_pn)
{
	space->code = code;
	space->pa = NULL;
	space->pa_cap = 0;
	space->pa_len = 0;
	port_t* init_port = space_alloc_port(space);
	init_port->name = init_pn;
	for (int i = 0; i < code->len; i++)
	{
		inst_t* inst = &code->inst_array[i];
		if (inst->type == INST_PORT)
		{
			port_t* port = space_alloc_port(space);
			port->name = inst->dyn.port.name;
			port->type = PORT_INST;
			port->dyn.inst.index = i;
			if (i == code->first_port_index)
			{
				port->link = init_pn;
				init_port->link = port->name;
			}
			else
			{
				port->link = NULL_PN;
			}
			/* TODO
			 * stat loop at first_port_index + 1 instead to delete that if,
			 * and do the first port inst before the loop */
		}
	}
	return init_port;
}

port_t* space_alloc_port(space_t* space)
{
	if (space->pa_len >= space->pa_cap)
	{
		int new_cap = space->pa_cap < 16 ? 16 : space->pa_cap * 2;
		port_t* new_pa = realloc(space->pa, new_cap * sizeof(port_t));
		if (new_pa == NULL)
		{
			/* TODO
			 * handle the error */
			fprintf(stderr, "error 2\n");
		}
		space->pa = new_pa;
		space->pa_cap = new_cap;
	}
	return &space->pa[space->pa_len++];
}


