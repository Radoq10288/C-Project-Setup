#include <ctype.h>
#include "functions.h"

/* This header file will only be available when compiling
 * CPPS under Windows using MinGW.
 */
#ifndef __linux__
	#include <io.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>	// mkdir() function resides here in linux
#include <time.h>


#define EMPTY_STRING	"\0"


void get_date(char *date_str) {
	char date_string[11];
	time_t date;
	struct tm *date_info;

	time(&date);
	date_info = localtime(&date);
	strftime(date_string, 11, "%m/%d/%Y", date_info);
	strcpy(date_str, date_string);
}


void get_time(char *time_str) {
	char time_string[12];
	time_t time_;
	struct tm *time_info;

	time(&time_);
	time_info = localtime(&time_);
	strftime(time_string, 12, "%I:%M:%S-%p", time_info);
	strcpy(time_str, time_string);
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
	char file_content[200] = {
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
	char date_str[11], date_time_string[24], new_file_name[260], time_str[12];

	get_date(date_str);
	get_time(time_str);

	strcpy(new_file_name, file_name);
	strcat(new_file_name, ".c");
	strrep(file_content, "filename", new_file_name, file_content);

	strcpy(date_time_string, date_str);
	strcat(date_time_string, "-");
	strcat(date_time_string, time_str);
	strrep(file_content, "datetime", date_time_string, file_content);

	if (make_file(new_file_name, file_content)) {return 1;}

	return 0;
}


int make_dir(const char *dir_name) {
	#ifdef __linux__
		if (mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {goto makedir_error;}
	#else
		if (_mkdir(dir_name)) {goto makedir_error;}
	#endif
	return 0;

	makedir_error:;
	return 1;
}


int make_makefile(char *project_name, char *exe_name) {
	char file_content[570] = {
		"BINDIR=bin\n"
		"OBJDIR=obj\n"
		"SRCDIR=src\n"
		"\n"
		"OS=$(shell uname -o)\n"
		"ifeq ($(OS), Msys)\n"
		"BINFILE=exename.exe\n"
		"endif\n"
		"ifeq ($(OS), GNU/Linux)\n"
		"BINFILE=exename\n"
		"endif\n"
		"\n"
		"BIN=$(BINDIR)/$(BINFILE)\n"
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
		"\n"
		"distclean: clean\n"
		"\trmdir $(BINDIR) $(OBJDIR)\n"
		"\n\n"
	};

	strrep(file_content, "exename", exe_name, file_content);	// For a weird reason, it only replace 'exename' from the string 'BINFILE=exename.exe'.
	strrep(file_content, "exename", exe_name, file_content);	// The second one is used to replace 'exename' from the string 'BINFILE=exename'.
	if (make_file(project_name, file_content)) {return 1;}

	return 0;
}


size_t strrep(const char *input_string, const char *old_string, const char *new_string, char output_string[]) {
	bool is_old_string_found = false;
	size_t buffer_size = strlen(old_string),
		   input_str_size = strlen(input_string),
		   new_str_size = strlen(new_string),
		   old_str_size = buffer_size,
		   result_str_size = input_str_size + 1;
	char buffer[buffer_size], *result_string;

	/* Set the size of result_string based on the length of input_string
	 * and new_string. If new_string has the same length as old_string, use
	 * the length of input_string as default size.
	 */

	if (new_str_size > old_str_size || new_str_size < old_str_size) {
		result_str_size = (input_str_size - old_str_size + new_str_size + 1);
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

	if (is_old_string_found == false) {
		strcpy(output_string, "\0");	// old_string is not found, return NULL
	}
	else {
		strncpy(output_string, result_string, result_str_size);
		output_string[result_str_size] = '\0';
	}
	free(result_string);
	result_string = NULL;
	return result_str_size;
}


