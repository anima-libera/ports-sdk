
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
	PORT_SPEC,
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

typedef struct port_surf_t port_spec_t;
struct port_spec_t
{
	uint32_t os_pn;
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
		port_spec_t spec;
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

/* Initializes the given space and sets its code to the given code.
 * Port instructions and an initial port of the given name are added to the
 * port table, and the initial link is created.
 * Returns the initial port that is just named and linked (not even typed). */
port_t* space_init(space_t* space, const code_t* code, uint32_t init_pn);

/* Cleans up the given space and all its sub spaces, assuming its parent space
 * has its code set to NULL. */
void space_cleanup(space_t* space);

/* Allocates and returns an uninitialized port in the given space. */
port_t* space_alloc_port(space_t* space);

/* Returns the port named by the given name and that is in the given space. */
port_t* space_find_port(const space_t* space, uint32_t pn);

/* Cut the link to the given port if any. */
void space_cut(space_t* space, uint32_t pn);

/* Links the two given ports, breaking any previous link to those ports. */
void space_link(space_t* space, uint32_t pn_a, uint32_t pn_b);

#endif /* __PSDK_SPACE_HEADER__ */
