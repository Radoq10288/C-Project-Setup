#ifndef FUNCTIONS_H
#define FUNCTIONS_H

char *get_date(void);

char *get_time(void);

int make_csf(char *file_name);

int make_makefile(char *exe_name);

int make_pds(char *project_name);

char* strrep(const char *input_string, const char *old_string, const char *new_string);

#endif // FUNCTIONS_H_INCLUDED


