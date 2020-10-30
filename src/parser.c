
#include "parser.h"
#include "code.h"
#include <stdio.h>

void cs_init_from_filepath(cs_t* cs, const char* filepath)
{
	printf("read file %s\n", filepath);
	cs->file = fopen(filepath, "r");
	if (cs->file == NULL)
	{
		/* TODO
		 * handle the error */
		fprintf(stderr, "error can't file\n");
	}
}

void cs_cleanup(cs_t* cs)
{
	fclose(cs->file);
}

char cs_peek_char(cs_t* cs)
{
	int c = fgetc(cs->file);
	if (c != EOF)
	{
		fseek(cs->file, -1, SEEK_CUR);
	}
	return c == EOF ? '\0' : c;
}

void cs_discard_char(cs_t* cs)
{
	fgetc(cs->file);
}

void cs_skip_skippable(cs_t* cs)
{
	int comment_mode = 0;
	while (1)
	{
		char c = cs_peek_char(cs);
		if (c == '#' && !comment_mode)
		{
			comment_mode = 1;
		}
		else if (c != ' ' && c != '\n' && c != '\t' && c != '\r' &&
			!comment_mode)
		{
			break;
		}
		else if (c == '#' && comment_mode)
		{
			comment_mode = 0;
		}
		else if (c == '\0' && comment_mode)
		{
			/* TODO
			 * handle error */
			fprintf(stderr,
				"error unfinisehd comment u functionnal programmer\n");
		}
		cs_discard_char(cs);
	}
}

int cs_parse_number(cs_t* cs)
{
	int number = 0;
	char c;
	while ((c = cs_peek_char(cs)), '0' <= c && c <= '9')
	{
		number = number * 10 + (c - '0');
		cs_discard_char(cs);
	}
	return number;
}

int char_is_inst_beg(char c)
{
	return ('0' <= c && c <= '9') || c == '.' || c == '\0' || c == '}';
}

int cs_parse_inst(cs_t* cs, code_t* code)
{
	cs_skip_skippable(cs);
	char c;
	c = cs_peek_char(cs);
	if ('0' <= c && c <= '9')
	{
		int n1 = cs_parse_number(cs);
		cs_skip_skippable(cs);
		c = cs_peek_char(cs);
		if (char_is_inst_beg(c))
		{
			inst_t* inst = code_alloc_inst(code);
			inst->type = INST_PORT;
			inst->dyn.port.name = n1;
			return 0;
		}
		else if (c == '-')
		{
			cs_discard_char(cs);
			c = cs_peek_char(cs);
			if (c == '-')
			{
				cs_discard_char(cs);
				inst_t* inst = code_alloc_inst(code);
				inst->type = INST_CUT;
				inst->dyn.cut.pn = n1;
				return 0;
			}
			else if (cs_skip_skippable(cs), c = cs_peek_char(cs),
				'0' <= c && c <= '9')
			{
				int n2 = cs_parse_number(cs);
				inst_t* inst = code_alloc_inst(code);
				inst->type = INST_LINK;
				inst->dyn.link.pn_a = n1;
				inst->dyn.link.pn_b = n2;
				return 0;
			}
			else
			{
				printf("error unexpected %c\n", c);
				return 3;
			}
		}
		else
		{
			printf("error unexpected %c\n", c);
			return 3;
		}
	}
	else if (c == '.')
	{
		cs_discard_char(cs);
		cs_skip_skippable(cs);
		int number = cs_parse_number(cs);
		inst_t* inst = code_alloc_inst(code);
		inst->type = INST_DEBUGACT;
		inst->dyn.debugact.number = number;
		return 0;
	}
	else if (c == '/')
	{
		/* TODO
		 * actually a perprocessor directive isn't an instruction at all, 
		 * this should probably be handeled elsewhere */

		/* preprocessor */
		return 0;
	}
	else if (c == '\0')
	{
		return 1;
	}
	else if (c == '}')
	{
		cs_discard_char(cs);
		return 2;
	}
	else
	{
		printf("error unexpected %c\n", c);
		return 3;
	}
}
