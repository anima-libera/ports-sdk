
#ifndef __PSDK_CODE_HEADER__
#define __PSDK_CODE_HEADER__

typedef struct code_t code_t;
struct code_t
{
	inst_t* inst_array;
	int len;
	int cap;
	int first_port_index;
};

void code_initialize(code_t* code);
void code_cleanup(code_t* code);

#endif /* __PSDK_CODE_HEADER__ */
