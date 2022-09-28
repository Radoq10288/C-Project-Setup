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


static void get_date(char *date_str) {
	char date_string[11];
	time_t date;
	struct tm *date_info;

	time(&date);
	date_info = localtime(&date);
	strftime(date_string, 11, "%m/%d/%Y", date_info);
	strcpy(date_str, date_string);
}


static void get_time(char *time_str) {
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
	char file_content[500];
	char date_str[11], date_time_string[24], new_file_name[260], time_str[12];

	get_date(date_str);
	get_time(time_str);

	strcpy(new_file_name, file_name);
	strcat(new_file_name, ".c");

	strcpy(date_time_string, date_str);
	strcat(date_time_string, "-");
	strcat(date_time_string, time_str);

	sprintf(file_content, "/* File:         %s\n"
		" *\n"
		" * Author:       \n"
		" *\n"
		" * Date & time:  %s\n"
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
		"\n",
		new_file_name, date_time_string
	);

	if (make_file(new_file_name, file_content)) {return 1;}

	return 0;
}


static int make_dir(const char *dir_name) {
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
	char file_content[590];

	sprintf(file_content, "BINDIR=bin\n"
		"OBJDIR=obj\n"
		"SRCDIR=src\n"
		"\n"
		"OS=$(shell uname -o)\n"
		"ifeq ($(OS), Msys)\n"
		"BINFILE=%s.exe\n"
		"endif\n"
		"ifeq ($(OS), GNU/Linux)\n"
		"BINFILE=%s\n"
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
		"\t@mkdir -p $(BINDIR)\n"
		"\t@mkdir -p $(OBJDIR)\n"
		"\t@mv *.o $(OBJDIR)/\n"
		"\n"
		"clean:\n"
		"\trm $(OBJ) $(BINDIR)/*\n"
		"\n"
		"distclean: clean\n"
		"\trmdir $(BINDIR) $(OBJDIR)\n"
		"\n\n",
		exe_name, exe_name
	);
	if (make_file(project_name, file_content)) {return 1;}

	return 0;
}


int make_project(char *project_name, char *exe_name, char *c_source_file) {
	char bin_dir_location[30], src_dir_location[30], src_file[30], makefile[30];

	if (make_dir(project_name)) {return 1;}

	sprintf(bin_dir_location, "%s/bin", project_name);
	sprintf(src_dir_location, "%s/src", project_name);
	sprintf(src_file, "%s/src/%s", project_name, c_source_file);
	sprintf(makefile, "%s/Makefile", project_name);

	make_dir(bin_dir_location);
	make_dir(src_dir_location);
	make_csf(src_file);
	make_makefile(makefile, exe_name);

	return 0;
}


