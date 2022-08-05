#include <ctype.h>
#include "functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define EMPTY_STRING	"\0"


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


static int make_file(const char *file_name, const char *file_content) {
	FILE *new_file;
	if ((new_file = fopen(file_name, "r")) == NULL) {
		if ((new_file = fopen(file_name, "w")) == NULL) {
			goto makefile_error;
		}
		fputs(file_content, new_file);
		fclose(new_file);
	}
	else {
		fclose(new_file);
		goto makefile_error;
	}

	return 0;

	makefile_error:;
	return 1;
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

	strcpy(new_file_name, file_name);
	strcat(new_file_name, ".c");
	file_content = strrep(file_content, "filename", new_file_name);

	strcpy(date_time_string, get_date());
	strcat(date_time_string, "-");
	strcat(date_time_string, get_time());
	file_content = strrep(file_content, "datetime", date_time_string);

	if (make_file(new_file_name, file_content)) {return 1;}

	return 0;
}


int make_makefile(char *project_name, char *exe_name) {
	char *file_content = {
		"BINDIR=bin\n"
		"OBJDIR=obj\n"
		"SRCDIR=src\n"
		"\n"
		"BIN=$(BINDIR)/exename\n"
		"OBJ=$(OBJDIR)/*.o\n"
		"CFILES=$(SRCDIR)/*.c\n"
		"\n"
		"CC=gcc\n"
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

	file_content = strrep(file_content, "exename", exe_name);
	if (make_file(project_name, file_content)) {return 1;}

	return 0;
}


static char *result_string;

static void free_result_string(void) {
	free(result_string);
	result_string = NULL;
}

char* strrep(const char *input_string, const char *old_string, const char *new_string) {
	bool is_old_string_found = false;
	size_t buffer_size = strlen(old_string),
		   input_str_size = strlen(input_string),
		   new_str_size = strlen(new_string),
		   result_str_size = input_str_size + 1;
	char buffer[buffer_size];

	/* Set the size of result_string based on the length of input_string
	 * and new_string. If new_string has the same length as old_string, use
	 * the length of input_string as default size.
	 */

	if (new_str_size > strlen(old_string) || new_str_size < strlen(old_string)) {
		result_str_size = (input_str_size - strlen(old_string) + new_str_size + 1);
	}
	result_string = malloc(sizeof(char) * result_str_size);

	strcpy(buffer, EMPTY_STRING);
	strcpy(result_string, EMPTY_STRING);
	int buffer_count = 1, char_index = 0;

	while (char_index != input_str_size) {
		strncat(buffer, &input_string[char_index], 1);

		if (strcmp(old_string, buffer) == 0) {
			strcat(result_string, new_string);
			strcpy(buffer, EMPTY_STRING);
			is_old_string_found = true;
			buffer_count -= 1;
		}
		else if (buffer_count == buffer_size) {
			strncat(result_string, &buffer[0], 1);
			if (buffer_count > 1) {
				memmove(buffer, buffer+1, (buffer_size - 1));
			}
			buffer[(buffer_size - 1)] = '\0';
			buffer_count -= 1;
		}
		else {
			// Do nothing here...
		}
		buffer_count++;
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


