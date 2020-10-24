
#ifndef __PSDK_SPACE_HEADER__
#define __PSDK_SPACE_HEADER__

#include "code.h"
#include <stdint.h>

#define NULL_PN 0xffffffff

typedef struct space_t space_t;

enum port_type_t
{
	PORT_INST,
	PORT_SURF,
};
typedef enum port_type_t port_type_t;

typedef struct port_inst_t port_inst_t;
struct port_inst_t
{
	int index;
};

typedef struct port_surf_t port_surf_t;
struct port_surf_t
{
	uint32_t os_pn;
	space_t* os_space;
};

typedef struct port_t port_t;
struct port_t
{
	uint32_t name;
	port_type_t type;
	union
	{
		port_inst_t inst;
		port_surf_t surf;
	} dyn;
	uint32_t link;
};

struct space_t
{
	const code_t* code; /* not owned */
	port_t* pa;
	int pa_cap;
	int pa_len;
};

port_t* space_alloc_port(space_t* space);

#endif /* __PSDK_SPACE_HEADER__ */
