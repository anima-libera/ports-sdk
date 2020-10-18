
#include "code.h"

int main(void)
{
	code_t code;
	code_initialize(&code);
	printf("-1 == %d\n", code.first_port_index);
	code_cleanup(&code);
	return 0;
}
