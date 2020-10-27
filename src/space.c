
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

void space_cleanup(space_t* space)
{
	free(space->pa);
	space->code = NULL;
	for (int i = 0; i < space->pa_len; i++)
	{
		if (space->pa[i].type == PORT_SURF)
		{
			space_t* os_space = space->pa[i].dyn.surf.os_space;
			if (os_space != NULL)
			{
				if (os_space->code != NULL) /* is not parent */
				{
					space_cleanup(os_space);
				}
				for (int j = i+1; j < space->pa_len; j++)
				{
					if (space->pa[j].type == PORT_SURF &&
						space->pa[j].dyn.surf.os_space == os_space)
					{
						space->pa[j].dyn.surf.os_space = NULL;
					}
				}
			}
		}
	}
	/* Recursive clean up that ensures that all sub-spaces are cleaned up once
	 * by deleting other occurences of a sub-space after cleaning it up, and by
	 * avoiding to clean up the parent space whose code has been set to NULL
	 * (a way to tag the spaces currently being cleaned up). */
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

port_t* space_find_port(const space_t* space, uint32_t pn)
{
	for (int i = 0; i < space->pa_len; i++)
	{
		if (space->pa[i].name == pn)
		{
			return &space->pa[i];
		}
	}
	return NULL;
}

void space_cut(space_t* space, uint32_t pn)
{
	printf("cut %d\n", (int)pn);
	port_t* port_a = space_find_port(space, pn);
	if (port_a->link != NULL_PN)
	{
		port_t* port_b = space_find_port(space, port_a->link);
		port_a->link = NULL_PN;
		port_b->link = NULL_PN;
	}
}

void space_link(space_t* space, uint32_t pn_a, uint32_t pn_b)
{
	printf("link %d and %d\n", (int)pn_a, (int)pn_b);
	port_t* port_a = space_find_port(space, pn_a);
	port_t* port_b = space_find_port(space, pn_b);
	if (port_a == NULL || port_b == NULL)
	{
		/* TODO
		 * handle error */
		fprintf(stderr, "error 88 uwu\n");
	}
	if (port_a->link != NULL_PN)
	{
		port_t* port_a_link = space_find_port(space, port_a->link);
		port_a_link->link = NULL_PN;
	}
	if (port_b->link != NULL_PN)
	{
		port_t* port_b_link = space_find_port(space, port_b->link);
		port_b_link->link = NULL_PN;
	}
	port_a->link = pn_b;
	port_b->link = pn_a;
}
