
#include "parser.h"
#include "code.h"
#include "space.h"
#include "interp.h"
#include <stdio.h>
#include <assert.h>

/* ABREVIATION TABLE
 * alloc  allocate, allocated
 * cap    capacity
 * cs     character stream
 * dyn    dynamic
 * init   initialize, initialization, initial
 * inst   instruction
 * la     link array
 * len    length
 * os     other side
 * pa     port array
 * pn     port name
 * psdk   ports software development kit
 * spec   special
 * surf   surface
 */

int main(int argc, const char** argv)
{
	if (argc <= 1)
	{
		/* TODO
		 * say something*/
		return 2;
	}
	cs_t cs;
	
	cs_init_from_filepath(&cs, argv[1]);


	code_t code;
	code_init(&code);

	while (cs_parse_inst(&cs, &code) == 0)
	{
		#if 0
		printf("%d\n", inst.dyn.debugact.number);
		#endif
	}

	cs_cleanup(&cs);

	code_interp(&code);

	code_cleanup(&code);

	#if 0

	code_t code;
	code_init(&code);

	inst_t* inst;

	inst = code_alloc_inst(&code);
	inst->type = INST_LINK;
	inst->dyn.link.pn_a = 2;
	inst->dyn.link.pn_b = 8;

	inst = code_alloc_inst(&code);
	inst->type = INST_LINK;
	inst->dyn.link.pn_a = 5;
	inst->dyn.link.pn_b = 9;

	for (int i = 1; i <= 10; i++)
	{
		inst = code_alloc_inst(&code);
		inst->type = INST_PORT;
		inst->dyn.port.name = i;
	}

	inst = code_alloc_inst(&code);
	inst->type = INST_LINK;
	inst->dyn.link.pn_a = 400;
	inst->dyn.link.pn_b = 800;

	inst = code_alloc_inst(&code);
	inst->type = INST_CUT;
	inst->dyn.cut.pn = 800;

	inst = code_alloc_inst(&code);
	inst->type = INST_PORT;
	inst->dyn.port.name = 400;

	inst = code_alloc_inst(&code);
	inst->type = INST_DEBUGACT;
	inst->dyn.debugact.number = 7;

	inst = code_alloc_inst(&code);
	inst->type = INST_PORT;
	inst->dyn.port.name = 800;

	inst = code_alloc_inst(&code);
	inst->type = INST_DEBUGACT;
	inst->dyn.debugact.number = 1;

	inst = code_alloc_inst(&code);
	inst->type = INST_DEBUGACT;
	inst->dyn.debugact.number = 8;

	inst = code_alloc_inst(&code);
	inst->type = INST_LINK;
	inst->dyn.link.pn_a = 900;
	inst->dyn.link.pn_b = 0;

	inst = code_alloc_inst(&code);
	inst->type = INST_PORT;
	inst->dyn.port.name = 900;

	inst = code_alloc_inst(&code);
	inst->type = INST_DEBUGACT;
	inst->dyn.debugact.number = 2;

	code_interp(&code);

	#if 0

	printf("a\n");

	space_t space;
	port_t* init_port = space_init(&space, &code, 0);
	init_port->type = PORT_SPEC;
	init_port->dyn.spec.os_pn = 0;

	printf("b\n");

	for (int i = 10; i <= 19; i++)
	{
		port_t* port = space_alloc_port(&space);
		port->name = i;
		port->type = PORT_SPEC;
		port->dyn.spec.os_pn = i;
		port->link = NULL_PN;
	}

	for (int i = 0; i < space.pa_len; i++)
	{
		printf("%2d -> %2d\n", (int)space.pa[i].name, (int)space.pa[i].link);
	}

	space_cleanup(&space);

	#endif

	code_cleanup(&code);

	#endif

	return 0;
}
