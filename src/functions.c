#include <ctype.h>
#include "functions.h"
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static char *date_string;


static void free_date_string(void) {
	free(date_string);
	date_string = NULL;
}


char *get_date(void) {
	time_t date;
	struct tm *date_info;

	date_string = malloc(sizeof(date_string) * 11);
	time(&date);
	date_info = localtime(&date);
	strftime(date_string, 11, "%m/%d/%Y", date_info);
	atexit(free_date_string);
	return date_string;
}


static char *time_string;


static void free_time_string(void) {
	free(time_string);
	time_string = NULL;
}


char *get_time(void) {
	time_t time_;
	struct tm *time_info;

	time_string = malloc(sizeof(time_string) * 12);
	time(&time_);
	time_info = localtime(&time_);
	strftime(time_string, 12, "%I:%M:%S-%p", time_info);
	atexit(free_time_string);
	return time_string;
}


int make_csf(char *file_name) {
	char *file_content = {
		"/* File:         filename\n"
		" *\n"
		" * Author:       \n"
		" *\n"
		" * Date & time:  datetime\n"
		" */\n"
		"\n"
		"#include <stdio.h>\n"
		"\n"
		"\n"
		"int main() {\n"
		"    // Your code goes here...\n"
		"    return 0;\n"
		"}\n"
		"\n"
		"\n"
	};
	char date_time_string[24], new_file_name[260];
	FILE *new_c_file;

	strcpy(new_file_name, file_name);
	strcat(new_file_name, ".c");
	file_content = strrep(file_content, "filename", new_file_name);

	strcpy(date_time_string, get_date());
	strcat(date_time_string, "-");
	strcat(date_time_string, get_time());
	file_content = strrep(file_content, "datetime", date_time_string);

	if ((new_c_file = fopen(new_file_name, "r")) == NULL) {
		if ((new_c_file = fopen(new_file_name, "w")) == NULL) {
			fprintf(stderr, "cpps\nerror: Failed to create the file \"%s\".\n", new_file_name);
			return EXIT_FAILURE;
		}
		fputs(file_content, new_c_file);
		fclose(new_c_file);
	}
	else {
		fprintf(stderr, "cpps\nerror: The file \"%s\" already exist.\n", new_file_name);
		fclose(new_c_file);
		return EXIT_FAILURE;
	}

	return 0;
}


int make_makefile(char *exe_name) {
	char *file_content = {
		"BINDIR=bin\n"
		"OBJDIR=obj\n"
		"SRCDIR=src\n"
		"\n"
		"BIN=$(BINDIR)/exename\n"
		"OBJ=$(OBJDIR)/*.o\n"
		"CFILES=$(SRCDIR)/*.c\n"
		"\n"
		"CC=mingw32-gcc-9.2.0\n"
		"\n"
		"debug: CFLAGS=-g -pedantic -Wall\n"
		"debug: build compile\n"
		"\n"
		"release: CFLAGS=-O2 -Wall\n"
		"release: build compile\n"
		"\n"
		"compile: $(BIN)\n"
		"$(BIN) : $(OBJ)\n"
		"\t$(CC) -o $(BIN) $(OBJ)\n"
		"\n"
		"build: $(OBJ)\n"
		"$(OBJ) : $(CFILES)\n"
		"\t$(CC) -c $(CFILES) $(CFLAGS)\n"
		"\n"
		"\t@mkdir -p $(OBJDIR)\n"
		"\t@mv *.o $(OBJDIR)/\n"
		"\n"
		"clean:\n"
		"\trm $(OBJ) $(BINDIR)/*\n"
		"\n\n"
	};
	FILE *new_makefile;

	file_content = strrep(file_content, "exename", exe_name);
	if ((new_makefile = fopen("Makefile", "r")) == NULL) {
		if ((new_makefile = fopen("Makefile", "w")) == NULL) {
			fprintf(stderr, "cpps\nerror: Failed to create the file \"Makefile\".\n");
			return EXIT_FAILURE;
		}
		fputs(file_content, new_makefile);
		fclose(new_makefile);
	}
	else {
		fprintf(stderr, "cpps\nerror: The file \"Makefile\" already exist.\n");
		fclose(new_makefile);
		return EXIT_FAILURE;
	}

	return 0;
}


int make_pds(char *project_name) {
	char *bin_dir_location = "projectname\\bin";
	char *src_dir_location = "projectname\\src";

	if (_mkdir(project_name)) {
		fprintf(stderr, "cpps\nerror: Directory \"%s\" already exist.\n", project_name);
		return EXIT_FAILURE;
	}
	bin_dir_location = strrep(bin_dir_location, "projectname", project_name);
	_mkdir(bin_dir_location);
	src_dir_location = strrep(src_dir_location, "projectname", project_name);
	_mkdir(src_dir_location);
	return 0;
}


static char *result_string;


static void free_result_string(void) {
	free(result_string);
	result_string = NULL;
}


char* strrep(char *input_string, char *old_string, char *new_string) {
	bool is_buffer_empty = true, is_old_string_found = false;
	char buffer[260];
	int char_index = 0, result_string_length = strlen(input_string) + 1;

	/* Set the size of result_string based on the length of input_string
	 * and new_string. If new_string has the same length as old_string, use
	 * the length of input_string as default size.
	 */

	if (strlen(new_string) > strlen(old_string) || strlen(new_string) < strlen(old_string)) {
		result_string_length = (strlen(input_string) - strlen(old_string) + strlen(new_string) + 2);
	}
	result_string = malloc(sizeof(char) * result_string_length);

	strcpy(buffer, "");
	strcpy(result_string, "");
	while (char_index <= strlen(input_string)) {
		if (isalpha(input_string[char_index])) {
			strncat(buffer, &input_string[char_index], 1);
			is_buffer_empty = false;
		}
		else {
			if (is_buffer_empty == true) {
				strncat(result_string, &input_string[char_index], 1);
			}
			else {
				if (strcmp(buffer, old_string) == 0) {
					strcat(result_string, new_string);
					is_old_string_found = true;
				}
				else {
					strcat(result_string, buffer);
				}
				strcpy(buffer, "");
				is_buffer_empty = true;
				strncat(result_string, &input_string[char_index], 1);
			}
		}
		char_index++;
	}

	atexit(free_result_string);
	if (is_old_string_found == false) {
		return NULL;	// old_string is not found, return NULL
	}
	else {
		return result_string;
	}
}


