#ifndef FUNCTIONS_H
#define FUNCTIONS_H

char *get_date(void);

int make_csf(char *file_name);

int make_makefile(char *exe_name);

int make_pds(char *project_name);

char* strrep(char *input_string, char *old_string, char *new_string);

#endif // FUNCTIONS_H_INCLUDED


