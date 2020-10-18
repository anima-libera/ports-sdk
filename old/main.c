
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

/* ABREVIATION TABLE
 * arr   array
 * cnt   compressed name table
 * cr    create
 * cs    code stream
 * dp    debug print
 * dpt   debug print trace
 * idmm  identity map maximum
 * inst  instruction
 * inter interpreter
 * kv    key value
 * len   length
 * os    other side
 * pos   position
 * rec   recursive
 * spec  special
 * surf  surface
 * tok   token
 */

/* MACRO OPTIONS
 * PSDK_NO_DPT  no debug print trace
 * PSDK_NO_DP   no debug print
 * PSDK_NO_D    PSDK_NO_DPT and PSDK_NO_DP
 */

#ifdef PSDK_NO_D
	#undef PSDK_NO_DPT
	#define PSDK_NO_DPT
	#undef PSDK_NO_DP
	#define PSDK_NO_DP
#endif

#ifndef PSDK_NO_DPT
	int g_dpt_active = 0;
	int g_dpt_level = 0;
	char* g_dpt_indent_start = "/";
	char* g_dpt_indent_end = "\\";
	char* g_dpt_indent = "|";
#endif
#ifndef PSDK_NO_DP
	int g_dp_active = 0;
#endif

int g_colors_active = 0;
#define COLOR_DPT_INDENT "32"
#define COLOR_DPT_NAME "32"

#ifndef PSDK_NO_DPT
	void _print_indent(FILE* stream)
	{
		if (g_colors_active)
		{
			fprintf(stream, "\x1b[" COLOR_DPT_INDENT "m");
		}
		for (int i = 0; i < g_dpt_level; i++)
		{
			fprintf(stream, "%s", g_dpt_indent);
		}
		if (g_colors_active)
		{
			fprintf(stream, "\x1b[39m");
		}
	}
	void lfprintf(FILE* stream, const char* format, ...)
	{
		va_list va_args;
		va_start(va_args, format);
		if (g_dpt_active)
		{
			#define FIRST_TRY_SIZE 256
			char* s = malloc(FIRST_TRY_SIZE * sizeof(char));
			int real_size = vsnprintf(s, FIRST_TRY_SIZE, format, va_args);
			if (real_size > FIRST_TRY_SIZE-1)
			{
				s = realloc(s, real_size+1);
				vsnprintf(s, real_size+1, format, va_args);
			}
			#undef FIRST_TRY_SIZE
			static int indent_next = 1;
			for (int i = 0; s[i] != '\0'; i++)
			{
				if (indent_next)
				{
					_print_indent(stream);
					indent_next = 0;
				}
				fprintf(stream, "%c", s[i]);
				if (s[i] == '\n')
				{
					indent_next = 1;
				}
			}
			/* TODO
			 * Print entier \n-separated strings (not individual chars) to
			 * speed up outputs to non-buffered streams like stderr. */
		}
		else
		{
			vfprintf(stream, format, va_args);
		}
		va_end(va_args);
	}
#else
	#define lfprintf(stream_, ...) \
		fprintf(stream_, __VA_ARGS__)
#endif

#define lprintf(...) \
	lfprintf(stdout, __VA_ARGS__)

#ifndef PSDK_NO_DPT
	#define DPT_OPEN() \
		do {if (g_dpt_active) { \
			lfprintf(stderr, "%s%s%s%s%s\n", \
				g_colors_active ? "\x1b[" COLOR_DPT_INDENT "m" : "", \
				g_dpt_indent_start, \
				g_colors_active ? "\x1b[" COLOR_DPT_NAME "m" : "", \
				__func__, \
				g_colors_active ? "\x1b[39m" : ""); \
			g_dpt_level++; \
		}} while (0)
	#define DPT_CLOSE() \
		do {if (g_dpt_active) { \
			g_dpt_level--; \
			lfprintf(stderr, "%s%s%s%s%s\n", \
				g_colors_active ? "\x1b[" COLOR_DPT_INDENT "m" : "", \
				g_dpt_indent_end, \
				g_colors_active ? "\x1b[" COLOR_DPT_NAME "m" : "", \
				__func__, \
				g_colors_active ? "\x1b[39m" : ""); \
		}} while (0)
	#define DPT_RETURN() \
		DPT_CLOSE()
#else
	#define DPT_OPEN()
	#define DPT_CLOSE()
	#define DPT_RETURN()
#endif

#ifndef PSDK_NO_DP
	#define DP(...) \
		do {if (g_dp_active) { \
			lfprintf(stderr, __VA_ARGS__); \
		}} while (0)
#else
	#define DP(...)
#endif

#if 0
typedef struct cnt_kv_t cnt_kv_t;
typedef struct cnt_t cnt_t;
#endif

#define PORT_NAME_NULL 0xffffffff

typedef enum inst_type_t inst_type_t;
typedef struct inst_t inst_t;
typedef struct code_t code_t;
typedef enum port_type_t port_type_t;
typedef struct port_t port_t;
typedef struct space_t space_t;
typedef struct inter_t inter_t;

enum inst_type_t
{
	INST_TYPE_PORT,
	INST_TYPE_LINK,
	INST_TYPE_CR_PORT,
	INST_TYPE_CR_SPACE,
	INST_TYPE_CR_SPACE_REC,
	INST_TYPE_DEBUG_ACT,
};

struct inst_t
{
	inst_type_t type;
	union {
		struct {uint32_t name;}
			port;
		struct {uint32_t name_a; uint32_t name_b;}
			link;
		struct {uint32_t name_a; uint32_t name_b; uint32_t name_s;}
			cr_port;
		struct {uint32_t name_a; uint32_t name_b; code_t* code;}
			cr_space;
		struct {uint32_t name_a; uint32_t name_b;}
			cr_space_rec;
		struct {uint32_t act;}
			debug_act;
	} data;
};

void inst_print(inst_t* inst)
{
	switch (inst->type)
	{
		case INST_TYPE_PORT:
			lprintf("port %d\n",
				(int)inst->data.port.name);
		break;
		case INST_TYPE_LINK:
			lprintf("link %d - %d\n",
				(int)inst->data.link.name_a,
				(int)inst->data.link.name_b);
		break;
		case INST_TYPE_CR_PORT:
			lprintf("cr_port %d, %d, %d\n",
				(int)inst->data.cr_port.name_a,
				(int)inst->data.cr_port.name_b,
				(int)inst->data.cr_port.name_s);
		break;
		case INST_TYPE_CR_SPACE:
			lprintf("cr_space TODO\n");
		break;
		case INST_TYPE_CR_SPACE_REC:
			lprintf("cr_space_rec TODO\n");
		break;
		case INST_TYPE_DEBUG_ACT:
			lprintf("debug_act TODO\n");
		break;
		default:
			lprintf("error\n");
		break;
	}
}

struct code_t
{
	inst_t* inst_arr;
	uint32_t inst_arr_len;
	uint32_t first_port_name;
	#ifndef PSDK_NO_DP
		char* path;
	#endif
};

void code_destroy(code_t* code)
{
	DPT_OPEN();
	for (uint32_t i = 0; i < code->inst_arr_len; i++)
	{
		if (code->inst_arr[i].type == INST_TYPE_CR_SPACE)
		{
			code_destroy(code->inst_arr[i].data.cr_space.code);
		}
	}
	free(code->inst_arr);
	#ifndef PSDK_NO_DP
		free(code->path);
	#endif
	free(code);
	DPT_CLOSE();
}

void code_print(code_t* code)
{
	for (uint32_t i = 0; i < code->inst_arr_len; i++)
	{
		inst_print(&code->inst_arr[i]);
	}
}

enum port_type_t
{
	PORT_TYPE_INST,
	PORT_TYPE_SURF,
	PORT_TYPE_SPEC,
	PORT_TYPE_NULL,
};

struct port_t
{
	port_type_t type;
	union {
		struct {uint32_t pos;}
			inst;
		struct {space_t* os_space; uint32_t os_name;}
			surf;
		struct {uint32_t os_name;}
			spec;
	} data;
	uint32_t link;
};

struct space_t
{
	code_t* code; /* not owned */
	size_t port_arr_len;
	port_t* port_arr;
};

space_t* space_create(code_t* code)
{
	DPT_OPEN();
	space_t* space = malloc(sizeof(space_t));
	space->code = code;
	space->port_arr_len = 256;
	space->port_arr = malloc(sizeof(port_t) * space->port_arr_len);
	for (uint32_t i = 0; i < space->port_arr_len; i++)
	{
		space->port_arr[i].type = PORT_TYPE_NULL;
	}
	for (uint32_t i = 0; i < code->inst_arr_len; i++)
	{
		if (code->inst_arr[i].type == INST_TYPE_PORT)
		{
			uint32_t name = code->inst_arr[i].data.port.name;
			space->port_arr[name].type = PORT_TYPE_INST;
			space->port_arr[name].data.inst.pos = i;
			space->port_arr[name].link = PORT_NAME_NULL;
		}
	}
	DPT_RETURN();
	return space;
}

void space_destroy(space_t* space)
{
	DPT_OPEN();
	/* TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
	/* list the spaces on the other sides of surface ports */
	/* free dat space */
	/* recursive calls to the listed spaces */
	/* when calling recursively, some ports are the other sides of ports
	 * already destroyed, that is a problem, take care of that */
	lprintf("TODO uwu\n");
	DPT_CLOSE();
}

void space_create_link(space_t* space, uint32_t name_a, uint32_t name_b)
{
	DPT_OPEN();
	assert(space->port_arr[name_a].type != PORT_TYPE_NULL);
	assert(space->port_arr[name_b].type != PORT_TYPE_NULL);
	space->port_arr[name_a].link = name_b;
	space->port_arr[name_b].link = name_a;
	DPT_CLOSE();
}

void space_cr_link_inst(space_t* space, uint32_t name_a, uint32_t name_b)
{
	DPT_OPEN();
	space_create_link(space, name_a, name_b);
	DPT_CLOSE();
}

void space_create_port(space_t* space, space_t* os_space,
	uint32_t name_a, uint32_t name_b)
{
	DPT_OPEN();
	assert(space->port_arr[name_a].type == PORT_TYPE_NULL);
	assert(os_space->port_arr[name_b].type == PORT_TYPE_NULL);
	port_t* port_a = &space->port_arr[name_a];
		port_a->type = PORT_TYPE_SURF;
		port_a->data.surf.os_space = os_space;
		port_a->data.surf.os_name = name_b;
		port_a->link = PORT_NAME_NULL;
	port_t* port_b = &os_space->port_arr[name_b];
		port_b->type = PORT_TYPE_SURF;
		port_b->data.surf.os_space = space;
		port_b->data.surf.os_name = name_a;
		port_b->link = PORT_NAME_NULL;
	DPT_CLOSE();
}

void space_create_special_port(space_t* space,
	uint32_t name_a, uint32_t name_b)
{
	DPT_OPEN();
	assert(space->port_arr[name_a].type == PORT_TYPE_NULL);
	port_t* port = &space->port_arr[name_a];
	port->type = PORT_TYPE_SPEC;
	port->data.spec.os_name = name_b;
	port->link = PORT_NAME_NULL;
	DPT_CLOSE();
}

void space_cr_port_inst(space_t* space,
	uint32_t name_a, uint32_t name_b, uint32_t name_s)
{
	DPT_OPEN();
	port_t* port_s = &space->port_arr[name_s];
	if (port_s->type == PORT_TYPE_SURF)
	{
		space_t* os_space = port_s->data.surf.os_space;
		space_create_port(space, os_space, name_a, name_b);
	}
	else if (port_s->type == PORT_TYPE_SPEC)
	{
		space_create_special_port(space, name_a, name_b);
	}
	else
	{
		assert(0); /* TODO */
	}
	DPT_CLOSE();
}

void space_cr_space_inst(space_t* space,
	uint32_t name_a, uint32_t name_b, code_t* code)
{
	DPT_OPEN();
	space_t* new_space = space_create(code);
	space_create_port(space, new_space, name_a, name_b);
	space_create_link(new_space, name_b, code->first_port_name);
	DPT_CLOSE();
}

void space_cr_space_rec_inst(space_t* space, uint32_t name_a, uint32_t name_b)
{
	DPT_OPEN();
	space_cr_space_inst(space, name_a, name_b, space->code);
	DPT_CLOSE();
}

/* Start from a given port in a given space and follow links as long as
 * possible, then return the final port. The space is also modified so that
 * at the end it is the space where the returned port is in. */
port_t* space_follow_links(space_t** space, uint32_t name);

port_t* space_follow_links(space_t** space, uint32_t name)
{
	DPT_OPEN();
	port_t* port = &(*space)->port_arr[name];
	while (port->link != PORT_NAME_NULL)
	{
		port = &(*space)->port_arr[port->link];
		if (port->type == PORT_TYPE_SURF)
		{
			*space = port->data.surf.os_space;
			port = &(*space)->port_arr[port->data.surf.os_name];
		}
		else
		{
			break;
		}
	}
	/* TODO
	 * debug-print the path and stuff */
	DPT_RETURN();
	return port;
}

struct inter_t
{
	space_t* root_space;
	space_t* spark_space;
	uint32_t spark_pos;
	int running;
	uint8_t byte;
	int is_output_mode;
};

inter_t* inter_create(code_t* code)
{
	DPT_OPEN();
	inter_t* inter = malloc(sizeof(inter_t));
	inter->root_space = space_create(code);
	space_t* rs = inter->root_space;
	space_create_special_port(rs, 0, 0);
	space_create_link(rs, 0, code->first_port_name);
	inter->spark_space = rs;
	inter->spark_pos = rs->port_arr[code->first_port_name].data.inst.pos;
	inter->spark_pos = (inter->spark_pos+1) % code->inst_arr_len;
	inter->running = 0;
	inter->byte = 0x00;
	inter->is_output_mode = 0;
	DPT_RETURN();
	return inter;
}

void inter_destroy(inter_t* inter)
{
	DPT_OPEN();
	space_destroy(inter->root_space);
	free(inter);
	DPT_CLOSE();
}

void inter_exec_port_inst(inter_t* inter, uint32_t name)
{
	DPT_OPEN();
	space_t* space = inter->spark_space;
	port_t* port = space_follow_links(&space, name);
	if (port->type == PORT_TYPE_INST)
	{
		inter->spark_space = space;
		inter->spark_pos = port->data.inst.pos;
	}
	else if (port->type == PORT_TYPE_SPEC)
	{
		switch (port->data.spec.os_name)
		{
			case 0:
				inter->running = 0;
			break;
			case 10: case 11: case 12: case 13:
			case 14: case 15: case 16: case 17:
				if (inter->is_output_mode)
				{
					inter->byte ^= 1 << (port->data.spec.os_name - 10);
				}
				else
				{
					assert(0); /* TODO */
				}
			break;
			case 18:
				inter->is_output_mode = 1;
				inter->byte = 0x00;
			break;
			case 19:
				if (inter->is_output_mode)
				{
					lprintf("%c", (char)inter->byte);
				}
				else
				{
					assert(0); /* TODO */
				}
			break;
			default:
				assert(0); /* TODO */
			break;
		}
	}
	else if (PORT_TYPE_SURF)
	{
		; /* Do nothing here. */
	}
	else if (port->type == PORT_TYPE_NULL)
	{
		assert(0); /* TODO: error ? */
	}
	DPT_CLOSE();
}

void inter_exec_inst(inter_t* inter, inst_t* inst)
{
	DPT_OPEN();
	switch (inst->type)
	{
		case INST_TYPE_PORT:
			inter_exec_port_inst(inter,
				inst->data.port.name);
		break;
		case INST_TYPE_LINK:
			space_cr_link_inst(inter->spark_space,
				inst->data.link.name_a,
				inst->data.link.name_b);
		break;
		case INST_TYPE_CR_PORT:
			space_cr_port_inst(inter->spark_space,
				inst->data.cr_port.name_a,
				inst->data.cr_port.name_b,
				inst->data.cr_port.name_s);
		break;
		case INST_TYPE_CR_SPACE:
			space_cr_space_inst(inter->spark_space,
				inst->data.cr_space.name_a,
				inst->data.cr_space.name_b,
				inst->data.cr_space.code);
		break;
		case INST_TYPE_CR_SPACE_REC:
			space_cr_space_rec_inst(inter->spark_space,
				inst->data.cr_space_rec.name_a,
				inst->data.cr_space_rec.name_b);
		break;
		case INST_TYPE_DEBUG_ACT:
			switch (inst->data.debug_act.act)
			{
				default:
					assert(0); /* TODO */
				break;
			}
		break;
		default:
			assert(0);
		break;
	}
	DPT_CLOSE();
}

void inter_iter(inter_t* inter)
{
	DPT_OPEN();
	inter_exec_inst(inter,
		&inter->spark_space->code->inst_arr[inter->spark_pos]);
	inter->spark_pos = (inter->spark_pos+1) % 
		inter->spark_space->code->inst_arr_len;
	DPT_CLOSE();
}

void inter_run(inter_t* inter)
{
	DPT_OPEN();
	inter->running = 1;
	while (inter->running)
	{
		inter_iter(inter);
	}
	DPT_CLOSE();
}

/* TODO
 * redo the funcking inst_arr_len !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* Code stream structure.
 * Ports source code that is being parsed is read via a code stream that
 * behaves as a stack of characters ended by an infinite amount of '\0'. */
typedef struct cs_t cs_t;

struct cs_t
{
	FILE* file;
};

/* TODO
 * things */

cs_t* cs_create_from_file(const char* file_path)
{
	DPT_OPEN();
	FILE* file = fopen(file_path, "r");
	if (file == NULL)
	{
		/* TODO: handle the error */
		lfprintf(stderr, "no\n");
		exit(69);
		return NULL;
	}
	cs_t* cs = malloc(sizeof(cs_t));
	cs->file = file;
	DPT_RETURN();
	return cs;
}

void cs_destroy(cs_t* cs)
{
	DPT_OPEN();
	fclose(cs->file);
	free(cs);
	DPT_CLOSE();
}

/* Discards a character from the code stream. */
void cs_discard_char(cs_t* cs);

void cs_discard_char(cs_t* cs)
{
	DPT_OPEN();
	fgetc(cs->file);
	DPT_CLOSE();
}

#if 0
/* Pops a character from the code stream. */
char cs_pop_char(cs_t* cs);

char cs_pop_char(cs_t* cs)
{
	int c = fgetc(cs->file);
	return c == EOF ? '\0' : c;
}
#endif

/* Returns the next character of the code stream but without popping it. */
char cs_peek_char(cs_t* cs);

char cs_peek_char(cs_t* cs)
{
	DPT_OPEN();
	int c = fgetc(cs->file);
	if (c != EOF)
	{
		fseek(cs->file, -1, SEEK_CUR);
	}
	DP("peek char '%c' (%d)\n", (char)c, (int)c);
	DPT_RETURN();
	return c == EOF ? '\0' : c;
}

/* Discards whitespace and comments from the code stream until some meaningful
 * character is uncountered. */
void cs_skip_skippable(cs_t* cs);

void cs_skip_skippable(cs_t* cs)
{
	DPT_OPEN();
	int in_comment = 0;
	char c;
	while ((c = cs_peek_char(cs)),
		c == ' ' || c == '\n' || c == '\t' || c == '\r' || in_comment)
	{
		if (c == '#')
		{
			in_comment = !in_comment;
		}
		else if (c == '\0' && in_comment)
		{
			/* TODO: handle this properly */
			lfprintf(stderr, "unfinisehd comment\n");
		}
		cs_discard_char(cs);
	}
	DPT_CLOSE();
}

/* Pops a name, assuming there is a name on the top of the code stream. */
uint32_t cs_parse_name(cs_t* cs);

uint32_t cs_parse_name(cs_t* cs)
{
	DPT_OPEN();
	uint32_t name = 0;
	char c;
	while ((c = cs_peek_char(cs)), '0' <= c && c <= '9')
	{
		name = name * 10 + (c - '0');
		cs_discard_char(cs);
	}
	DPT_RETURN();
	return name;
}

/* Pops an instruction from the code stream and write it at inst, if any.
 * Returns 0 if there is no instruction to pop from the code stream.
 * If invalid code is poped instead, an error is printed. */
int cs_parse_inst(cs_t* cs, inst_t* inst);

int cs_parse_inst(cs_t* cs, inst_t* inst)
{
	DPT_OPEN();
	char c;
	cs_skip_skippable(cs);
	c = cs_peek_char(cs);
	if (c == '\0')
	{
		DPT_RETURN();
		return 0;
	}
	else if (c == '.')
	{
		/* TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
	}
	else if (c < '0' || '9' < c)
	{
		/* TODO: handle the error */
		lfprintf(stderr, "no 1\n");
		exit(69);
		return 0;
	}
	uint32_t name_a = cs_parse_name(cs);

	cs_skip_skippable(cs);
	c = cs_peek_char(cs);
	if (c == '-')
	{
		cs_skip_skippable(cs);
		c = cs_peek_char(cs);
		if ('0' <= c && c <= '9')
		{
			inst->type = INST_TYPE_LINK;
			inst->data.link.name_a = name_a;
			inst->data.link.name_b = cs_parse_name(cs);
			DPT_RETURN();
			return 1;
		}
		else
		{
			/* TODO: handle the error */
			lfprintf(stderr, "no 2\n");
			exit(69);
			return 0;
		}
	}
	else if (c == ',')
	{
		cs_skip_skippable(cs);
		c = cs_peek_char(cs);
		if ('0' <= c && c <= '9')
		{
			uint32_t name_b = cs_parse_name(cs);

			cs_skip_skippable(cs);
			if (c != ',')
			{
				/* TODO: handle the error */
				lfprintf(stderr, "no 3\n");
				exit(69);
				return 0;
			}
			cs_skip_skippable(cs);
			if ('0' <= c && c <= '9')
			{
				inst->type = INST_TYPE_CR_PORT;
				inst->data.cr_port.name_a = name_a;
				inst->data.cr_port.name_b = name_b;
				inst->data.cr_port.name_s = cs_parse_name(cs);
				DPT_RETURN();
				return 1;
			}
			else if (c == '&')
			{
				inst->type = INST_TYPE_CR_SPACE_REC;
				inst->data.cr_space_rec.name_a = name_a;
				inst->data.cr_space_rec.name_b = name_b;
				DPT_RETURN();
				return 1;
			}
			else if (c == '{')
			{
				/* TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
				lfprintf(stderr, "TODO\n");
				DPT_RETURN();
				return 1;
			}
			else if (c == '[')
			{
				/* TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
				lfprintf(stderr, "TODO\n");
				DPT_RETURN();
				return 1;
			}
			else
			{
				/* TODO: handle the error */
				lfprintf(stderr, "no 4\n");
				exit(69);
				return 0;
			}
		}
		else if (c == '?')
		{
			assert(0); /* TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
		}
		else
		{
			/* TODO: handle the error */
			lfprintf(stderr, "no 5\n");
			exit(69);
			return 0;
		}
	}
	else
	{
		inst->type = INST_TYPE_PORT;
		inst->data.port.name = name_a;
		DPT_RETURN();
		return 1;
	}
}

code_t* cs_parse_code(cs_t* cs)
{
	DPT_OPEN();
	int len = 0;
	int cap = 8;
	inst_t* inst_arr = malloc(cap * sizeof(inst_t));
	while (cs_parse_inst(cs, &inst_arr[len]))
	{
		#ifndef PSDK_NO_DP
			if (g_dp_active)
			{
				lprintf("inst parsed: ");
				inst_print(&inst_arr[len]);
			}
		#endif
		len++;
		if (len > cap)
		{
			cap *= 2;
			inst_arr = realloc(inst_arr, cap);
		}
	}
	inst_arr = realloc(inst_arr, len * sizeof(inst_t));
	code_t* code = malloc(sizeof(code_t));
	code->inst_arr = inst_arr;
	code->inst_arr_len = len;
	#ifndef PSDK_NO_DP
		code->path = malloc(4);
		code->path[0] = 'u';
		code->path[1] = 'w';
		code->path[2] = 'u';
		code->path[3] = '\0';
		/* TODO
		 * Write the real path here. */
	#endif
	/* Find the first port name. */
	code->first_port_name = PORT_NAME_NULL;
	for (uint32_t i = 0; i < code->inst_arr_len; i++)
	{
		if (code->inst_arr[i].type == INST_TYPE_PORT)
		{
			code->first_port_name = code->inst_arr[i].data.port.name;
			#ifndef PSDK_NO_DP
				DP("Code %s first port name is %d\n",
					code->path, (int)code->first_port_name);
			#endif
			break;
		}
	}
	if (code->first_port_name == PORT_NAME_NULL)
	{
		#ifndef PSDK_NO_DP
			DP("Code %s has no ports\n", code->path);
			DP("Abort\n");
		#endif
		assert(0);
		/* TODO
		 * Handle that error correctly. */
	}
	DPT_RETURN();
	return code;
}


int main(void)
{
	g_dp_active = 1;
	g_dpt_active = 1;
	g_colors_active = 1;

	cs_t* cs = cs_create_from_file("t.ports");
	code_t* code = cs_parse_code(cs);
	cs_destroy(cs);

	code_print(code);

	inter_t* inter = inter_create(code);
	inter_run(inter);
	inter_destroy(inter);

	code_destroy(code);

	return 0;
}

/* TODO
 * add the cut link instruction (if i feel like its really missing)
 * add the 90:99,10:19,0 syntax (sec can be ?:?)(10:19-30:39 too)(10:19-- too)
 * forbid cycles in code imports via n,n,[file] syntax
 * flag to take ascii/utf-8 files as input (comments can contain shit)
 * split this file into at least two files 
 * add line number and column and meaningful error messages
 * add a lot of DPs, so that the DP flag alone looks cool and consistent */

/* TODO LATER
 * make the special spaces be .so files dynamically loaded
 *  (so that people could create extensions to the language)
 * add multithreading (multiple sparks at the same time) (virtual THEN real)
 * add timetravel (after multithreading)
 * add a graphical api (using gpu, like turtle but can save load & use sprites)
 * add a sound api
 * add a network api
 */

/* STD LIB TO PROVIDE
 * note that the std ports programs should act as examples but sould also be
 *  used in actual code, thus they must be documented
 * a thing that can work out Y branches with cables
 * minsky machine register
 * number register
 * arithmetic unit
 * number printing unit
 * color printing
 * a brainfuck interpreter
 */

/* DEBUG ACTIONS TO ADD
 * note that these are the short names, they also exist in long
 *  (for example: parmsg -> parmessage, parsingmsg, parsingmessage)
 * parsing debug actions
 * .forceok        (no errors in the current code)
 * .dummy          (1,2,{.dummy} should not error, but using this space does)
 * .elipsis        (1-2 1 .elipsis 2 should not warn, but exec .elipsis does)
 * ...             (alias of .dummy or .elipsis depending on the context)
 * .forceok        (no errors in the current code when parsing)
 * .parhalt
 * .parerror {message}
 * .parwarn {message}
 * .parmsg {message}
 * .pardbeg {d (default) or dp or dpt}
 * .pardend
 * runtime debug actions
 * .breakpoint
 * .runhalt
 * .runerror {message}
 * .runwarn {message}
 * .runmsg {message}
 * .runhey         (short for .runmsg {hey})
 * .rundbeg {d (default) or dp or dpt}
 * .rundend
 */

/* PREPROCESSOR DIRECTIVES
 * //name {content}        (then /name will be replaced by content)
 * /read [filename]
 * see the paper note *
 */
