#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>


void get_date(char *date_str);

void get_time(char *time_str);

int make_csf(char *file_name);

int make_dir(const char *dir_name);

int make_makefile(char *project_name, char *exe_name);

size_t strrep(const char *input_string, const char *old_string, const char *new_string, char output_string[]);

#endif // FUNCTIONS_H_INCLUDED


