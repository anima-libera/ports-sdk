
#ifndef __PSDK_CODE_HEADER__
#define __PSDK_CODE_HEADER__

#include <stdint.h>

enum inst_type_t
{
	INST_PORT,
	INST_DEBUGACT,
};
typedef enum inst_type_t inst_type_t;

typedef struct inst_port_t inst_port_t;
struct inst_port_t
{
	uint32_t name;
};

typedef struct inst_port_t inst_debugact_t;
struct inst_debugact_t
{
	int number;
};

typedef struct inst_t inst_t;
struct inst_t
{
	inst_type_t type;
	union
	{
		inst_port_t port;
		inst_debugact_t debugact;
	} dyn;
};

typedef struct code_t code_t;
struct code_t
{
	inst_t* inst_array;
	int len;
	int cap;
	int first_port_index;
};

void code_init(code_t* code);
void code_cleanup(code_t* code);

inst_t* code_alloc_inst(code_t* code);

#endif /* __PSDK_CODE_HEADER__ */
