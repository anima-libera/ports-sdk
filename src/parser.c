
#include "parser.h"
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
			fprintf(stderr, "error unfinisehd comment u ocaml programmer\n");
		}
		cs_discard_char(cs);
	}
}

uint32_t cs_parse_name(cs_t* cs)
{
	uint32_t name = 0;
	char c;
	while ((c = cs_peek_char(cs)), '0' <= c && c <= '9')
	{
		name = name * 10 + (c - '0');
		cs_discard_char(cs);
	}
	return name;
}
