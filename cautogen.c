#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cautogen.h"

#define STAT_USAGE 			0
#define STAT_READY 			1

#define HEADER_PRE			"_H_YLG_"
#define HEADER_POS			"_"

#define	INCLUDE_LIB(ptr)		(fprintf((ptr), \
						"#include <stdio.h>\n" \
						"#include <stdlib.h>\n" \
						))

#define INCLUDE_AUTHOR(PTR, TIMEINFO)	(fprintf((PTR), \
						"/*\n" \
						" * Author: Yann LE GOFF <y.legoff97@gmail.com>\n" \
						" * Date:   %s\n" \
						" */\n", \
						(TIMEINFO)\
						))

#define INCLUDE_HEADER(PTR, HEADER_DEF)	(fprintf((PTR), \
						"#ifndef %s\n" \
						"#define %s\n" \
						"\n\n\n\n\n" \
						"#endif\n", \
						(HEADER_DEF), (HEADER_DEF)\
						))

int get_status(int argv, char **argc, int* verbose_level, int* force_level)
{
	int status = -1;
	if (argv == 1)
		status = STAT_USAGE;
	else if (is_option_present(argv, argc, "-h"))
		status = STAT_USAGE;
	else 
	{
		*verbose_level = is_option_present(argv, argc, "-v");
		*force_level = is_option_present(argv, argc, "-f");
		status = STAT_READY;
	}
	return status;
}

int is_option_present(int argv, char **argc, char *str_option)
{
	for (int i = 0; i < argv; i++)
		if (!strcmp(argc[i], str_option))
				return 1;
	return 0;
}

void show_usage()
{
	printf("./cautogen [options] [name]\n");
	printf("\n");
	printf("name : name of the header file\n");
	printf("options:\n");
	printf("\t -f force\n");
	printf("\t -h help\n");
	printf("\t -v verbose\n");
}

void to_upper(char *str)
{
	for(int i = 0; str[i] != 0; i++)
		if('a' <= str[i] && str[i] <= 'z')
			str[i] += 'A' - 'a';
}

void format_time(char *output)
{
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);

	sprintf(output, "%02d/%02d/%2d %02d:%02d:%02d",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void generate_header_def(char *output, char *file_name)
{
	to_upper(file_name);
	sprintf(output, "%s%s%s", HEADER_PRE, file_name, HEADER_POS);
}

void generate_header(FILE *fptr, char *file_name, char *timeinfo)
{
	char header_name [100];
	generate_header_def(header_name, file_name);
	INCLUDE_AUTHOR(fptr, timeinfo);
	INCLUDE_HEADER(fptr, header_name);
}

void generate_code(FILE *fptr, char *file_name_h, char *timeinfo)
{
	INCLUDE_AUTHOR(fptr, timeinfo);
	INCLUDE_LIB(fptr);
	fprintf(fptr, "#include \"%s\"\n", file_name_h);
	fprintf(fptr, "\n\n\n\n\n");
}

int do_files_exist(FILE *fptr, char* file_name)
{
	fptr = fopen(file_name, "r");
	
	if (fptr != NULL)
	{
		fclose(fptr);
		return 1;
	}
	else
	{
		return 0;
	}
}

void generate_file_name(char *file_name, char *file_name_c, char *file_name_h)
{	
	strcpy(file_name_c, file_name);
	strcat(file_name_c, ".c");
	strcpy(file_name_h, file_name);
	strcat(file_name_h, ".h");
}

int main(int argv, char **argc)
{
	int verbose_level = 0;
	int force_level = 0;
	char file_name[100];
	int status = get_status(argv, argc, &verbose_level, &force_level);

	if (status == STAT_USAGE)
	{
		show_usage();
		exit(0);
	}
	else if (status == STAT_READY)
	{
		FILE *fptr_c, *fptr_h;
		char file_name_c[100];
		char file_name_h[100];
		char timeinfo [100];
		
		strcpy(file_name, argc[1]);
		generate_file_name(file_name, file_name_c, file_name_h);
		format_time(timeinfo);
		
		if (!force_level && (do_files_exist(fptr_c, file_name_h) || do_files_exist(fptr_h, file_name_h)))
		{
			printf("Error: files already exists\n");
			exit(1);
		}

		fptr_c = fopen(file_name_c, "w");
		fptr_h = fopen(file_name_h, "w");
		if (fptr_c == NULL || fptr_h == NULL)
		{
			printf("Error: file doesn't open\n");
			exit(1);
		}	

		generate_code(fptr_c, file_name_h, timeinfo);
		if (verbose_level)
			printf("Finish generated .c file\n");
		
		generate_header(fptr_h, file_name, timeinfo);
		if (verbose_level)
			printf("Finish generated .h file\n");

		fclose(fptr_c);
		fclose(fptr_h);
		printf("Done.\n");
	}

	return 0;
}

