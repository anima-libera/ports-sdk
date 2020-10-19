
#ifndef __PSDK_SPACE_HEADER__
#define __PSDK_SPACE_HEADER__

#include <stdint.h>

enum port_type_t
{
	PORT_INST,
};
typedef enum port_type_t port_type_t;

typedef struct port_inst_t port_inst_t;
struct port_inst_t
{
	int index;
};

typedef struct port_t port_t;
struct port_t
{
	uint32_t name;
	port_type_t type;
	union
	{
		port_inst_t inst;
	} dyn;
};

typedef struct space_t space_t;
struct space_t
{
	const code_t* code; /* not owned */
	port_t* port_array;
	int port_array_len;
};

#endif /* __PSDK_SPACE_HEADER__ */
