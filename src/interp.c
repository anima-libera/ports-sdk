
#include "interp.h"
#include "code.h"
#include "space.h"
#include <stdio.h>

void code_interp(const code_t* code)
{
	space_t root_space;
	port_t* init_port = space_init(&root_space, code, 0);
	init_port->type = PORT_SPEC;
	init_port->dyn.spec.os_pn = 0;
	space_t* spark_space = &root_space;
	int spark_index = (code->first_port_index+1) % code->len;
	while (1)
	{
		inst_t* inst = &spark_space->code->inst_array[spark_index];
		switch (inst->type)
		{
			case INST_PORT:
			{
				printf("port %d\n", (int)inst->dyn.port.name);
				port_t* port_src = space_find_port(spark_space,
					inst->dyn.port.name);
				if (port_src->link != NULL_PN)
				{
					port_t* port_dst = space_find_port(spark_space,
						port_src->link);
					switch (port_dst->type)
					{
						case PORT_INST:
							printf("follow link to %d\n", (int)port_dst->name);
							spark_index = port_dst->dyn.inst.index;
						break;
						case PORT_SURF:
							/* TODO */
						break;
						case PORT_SPEC:
							printf("special port %d\n",
								(int)port_dst->dyn.spec.os_pn);
							switch (port_dst->dyn.spec.os_pn)
							{
								case 0:
									goto end;
								break;
								default:
									printf("special uwu");
								break;
							}
						break;
					}
				}
			}
			break;
			case INST_LINK:
				space_link(spark_space,
					inst->dyn.link.pn_a, inst->dyn.link.pn_b);
			break;
			case INST_CUT:
				space_cut(spark_space, inst->dyn.cut.pn);
			break;
			case INST_DEBUGACT:
				switch (inst->dyn.debugact.number)
				{
					case 1:
						printf("uwu\n");
					break;
					case 2:
						printf("debugact 2: halt\n");
						goto end;
					break;
					default:
						printf("unknown debugact %d\n",
							inst->dyn.debugact.number);
					break;
				}
			break;
		}
		spark_index = (spark_index+1) % code->len;
	}
	end:
	space_cleanup(&root_space);
}
