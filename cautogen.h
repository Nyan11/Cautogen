#ifndef _H_YLG_CATOGEN_
#define _H_YLG_CATOGEN_

int	get_status(int argv, char **argc, int* verbose_level, int* force_level);
int is_option_present(int argv, char **argc, char *str_option);
void	show_usage();
void	to_upper(char* str);
void	format_time(char *output);
void	generate_header_def(char *output, char *file_name);
void	generate_header(FILE *fptr, char *file_name, char *timeinfo);
void	generate_code(FILE *fptr, char *file_name_h, char *timeinfo);
int	do_files_exist(FILE *fptr, char *file_name);
void	generate_file_name(char *file_name, char *file_name_c, char *file_name_h);

#endif
