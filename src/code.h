
#ifndef __PSDK_CODE_HEADER__
#define __PSDK_CODE_HEADER__

#include <stdint.h>

enum inst_type_t
{
	INST_PORT,
	INST_LINK,
	INST_CUT,
	INST_DEBUGACT,
};
typedef enum inst_type_t inst_type_t;

typedef struct inst_port_t inst_port_t;
struct inst_port_t
{
	uint32_t name;
};

typedef struct inst_link_t inst_link_t;
struct inst_link_t
{
	uint32_t pn_a;
	uint32_t pn_b;
};

typedef struct inst_cut_t inst_cut_t;
struct inst_cut_t
{
	uint32_t pn;
};

typedef struct inst_debugact_t inst_debugact_t;
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
		inst_link_t link;
		inst_cut_t cut;
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

/* Initializes the given code to an empty code. */
void code_init(code_t* code);

/* Cleans up the given code and all its instructions. */
void code_cleanup(code_t* code);

/* Allocates and returns an unilitialized instrction at the end of the given
 * code. */
inst_t* code_alloc_inst(code_t* code);

#endif /* __PSDK_CODE_HEADER__ */
