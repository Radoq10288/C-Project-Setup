#include <getopt.h>
#include <io.h>
#include "functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


static void help(void) {
	printf("\nUsage:\n"
			"    cpps [Project Name] [Option]     For creating a C programming project.\n"
			"    cpps [Option]                    For other options to show help, version, etc.\n"
			"\n"
			"Options:\n"
			"    -e, --exe-name     Filename of the executable file to be created.\n"
			"    -f, --filename     Filename of the C source file to be created.\n"
			"    -h                 Show this help.\n"
			"        --verbose      Show messages what is being done.\n"
			"    -v, --version      Show current version of this software.\n");
}


static void version(void) {
	printf("\nCPPS version 0.1.1-alpha.2, Copyright (C) 2022 Radoq10288\n"
			"CPPS comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions; type `show c' for details.\n");
}


int main(int argc, char *argv[]) {
	char c_source_filename[260] = "main", c_source_file_loc[260] = "projectname\\src\\",
		 exe_filename[260], project_name[260];
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
			{"brief",		no_argument,       	&flag,		0},
			{"verbose",		no_argument,		&flag,		1},
            {0,				0,					0,			0}
        };

		opterr = 0;
		getopt_status = getopt_long(argc, argv, ":e:f:hv", long_options, &option_index);
        if (getopt_status == -1) {
			break;
		}

		switch(getopt_status) {
			case 0:
				/* Do nothing here... */
				break;
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
				if (optopt == '\0') {
					fprintf(stderr, "cpps\nError: Unknown long option of '%s'\n", argv[optind - 1]);
				}
				else {
					fprintf(stderr, "cpps\nError: Unknown short option of '-%c'\n", optopt);
				}
				goto cpps_error;
			case ':':
				if (optopt =='\0') {
					fprintf(stderr, "cpps\nError: Long option '--%s' requires an argument.\n", long_options[optind - 1].name);
				}
				else {
					fprintf(stderr, "cpps\nError: Short option '-%c' requires an argument.\n", optopt);
				}
				goto cpps_error;
        }
    }

	if (make_pds(project_name) != 0) {goto cpps_error;}
	if (flag == 1) {printf("Project directory structure is created.\n");}

	strcat(c_source_file_loc, c_source_filename);
	strcpy(c_source_file_loc, strrep(c_source_file_loc, "projectname", project_name));
	if (make_csf(c_source_file_loc) != 0) {goto cpps_error;}
	if (flag == 1) {printf("C source file created.\n");}

	_chdir(project_name);
	if (make_makefile(exe_filename) != 0) {goto cpps_error;}
	if (flag == 1) {printf("Makefile is created.\n");}
	_chdir("..");

	if (flag == 1)  {printf("New project \"%s\" is created.\n", project_name);}

	skip_project_creation:;
	return 0;

	cpps_error:;
	return 1;
}


