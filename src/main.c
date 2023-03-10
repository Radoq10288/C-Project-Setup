#include <getopt.h>
#include <ctype.h>

/* This header file will only be available when compiling
 * CPS under Windows using MinGW.
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


static char c_file[500] = {
	"/* File:         %s\n"
	" *\n"
	" * Author:       \n"
	" *\n"
	" * Date & time:  %s-%s\n"
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


static char makefile[700] = {
	"BINDIR=bin\n"
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
	"\n"
	"run:\n"
	"\t$(BIN)\n"
	"\n\n"
};


static void help(void) {
	printf("\nUsage:\n"
			"    cps [Project Name] [Option] [argument]	To setup a C programming project.\n"
			"    cps [Option]                    		For other options to show help, version, etc.\n"
			"\n"
			"Options:\n"
			"    -e, --exe-name     Filename of the executable file to be created.\n"
			"    -f, --filename     Filename of the C source file to be created.\n"
			"        --help         Show this help.\n"
			"        --verbose      Show messages what is being done.\n"
			"        --version      Show current version of this software.\n");
}


static void version(void) {
	printf("\nCPS version 0.1.2-alpha, Copyright (C) 2022 Radoq10288\n"
			"CPS comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions; type `show c' for details.\n");
}


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


static char content_string[600] = EMPTY_STRING;
static char date_string[11] = EMPTY_STRING;
static char time_string[12] = EMPTY_STRING;


static int make_project(char *project_name, char *exe_name, char *c_source_file) {
	char bin_dir_location[260], src_dir_location[260], src_file[260], makefile_name[260];

	if (make_dir(project_name)) {return 1;}

	sprintf(bin_dir_location, "%s/bin", project_name);
	sprintf(src_dir_location, "%s/src", project_name);
	make_dir(bin_dir_location);
	make_dir(src_dir_location);
	
	get_date(date_string);
	get_time(time_string);
	
	sprintf(src_file, "%s/src/%s", project_name, c_source_file);
	strcat(src_file, ".c");
	sprintf(content_string, c_file, src_file, date_string, time_string);
	make_file(src_file, content_string);
	
	sprintf(makefile_name, "%s/Makefile", project_name);
	sprintf(content_string, makefile, exe_name, exe_name);
	make_file(makefile_name, content_string);

	return 0;
}


// This is only used for testing purposes.
// #define ON_TEST_MODE

#ifndef ON_TEST_MODE

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
		fprintf(stderr, "cps\nerror: No argument(s) specified.\ninfo: Please type 'cps --help' for guidance on proper usage.\n");
		goto cps_error;
	}

	char c_source_filename[260] = "main", exe_filename[260], project_name[260];
	int getopt_status;

	// Flags
	static int flag;

	strcpy(project_name, argv[1]);
	strcpy(exe_filename, argv[1]);
	while(true) {
        int option_index = 0;
        static struct option long_options[] = {
            {"exe-name",	required_argument,	0,  		'e'},
            {"filename",	required_argument,	0,  		'f'},
            {"help",		no_argument,		0,  		'h'},
            {"version",		no_argument,		0,			'v'},
			{"brief",		no_argument,       	&flag,		1},
			{"verbose",		no_argument,		&flag,		2},
            {0,				0,					0,			0}
        };

		opterr = 0;
		getopt_status = getopt_long(argc, argv, ":e:f:", long_options, &option_index);
        if (getopt_status == -1) {
			break;
		}

		switch(getopt_status) {
			case 'e':
				strcpy(exe_filename, optarg);
				break;
			case 'f':
				strcpy(c_source_filename, optarg);
				break;
			case 'h':
				help();
				goto skip_project_creation;
			case 'v':
				version();
				goto skip_project_creation;
			case '?':
				fprintf(stderr, "cps\nError: Unknown option of '%s'\n", argv[optind - 1]);
				goto cps_error;
			case ':':
				fprintf(stderr, "cps\nError: Option '%s' requires an argument.\n", argv[optind - 1]);
				goto cps_error;
        }
    }

	if (make_project(project_name, exe_filename, c_source_filename) != 0) {
		fprintf(stderr, "cps\nerror: Directory \"%s\" already exist.\n", project_name);
		goto cps_error;
	}
	if (flag == 2) {printf("cps\nInfo: New project '%s' is created.\n", project_name);}
	skip_project_creation:;
	return 0;

	cps_error:;
	return 1;
}

#endif


