#include <getopt.h>
#include "functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static void help(void) {
	printf("\nUsage:\n"
			"    cpps [Project Name] [Option]     For creating a C programming project.\n"
			"    cpps [Option]                    For other options to show help, version, etc.\n"
			"\n"
			"Options:\n"
			"    -e, --exe-name     Filename of the executable file to be created.\n"
			"    -f, --filename     Filename of the C source file to be created.\n"
			"    -h, --help         Show this help.\n"
			"        --verbose      Show messages what is being done.\n"
			"    -v, --version      Show current version of this software.\n");
}


static void version(void) {
	printf("\nCPPS version 0.1.1-alpha.3, Copyright (C) 2022 Radoq10288\n"
			"CPPS comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions; type `show c' for details.\n");
}


int make_project(char *project_name, char *exe_name, char *c_source_file) {
	char *bin_dir_location = "projectname/bin",
		 *src_dir_location = "projectname/src",
		 *src_file = "projectname/src/srcfile",
		 *makefile = "projectname/Makefile";

	if (mkdir(project_name)) {return 1;}
	bin_dir_location = strrep(bin_dir_location, "projectname", project_name);
	src_dir_location = strrep(src_dir_location, "projectname", project_name);
	src_file = strrep(src_file, "projectname", project_name);
	src_file = strrep(src_file, "srcfile", c_source_file);
	makefile = strrep(makefile, "projectname", project_name);

	mkdir(bin_dir_location);
	mkdir(src_dir_location);
	make_csf(src_file);
	make_makefile(makefile, exe_name);

	return 0;
}


int main(int argc, char *argv[]) {
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

	if (make_project(project_name, exe_filename, c_source_filename) != 0) {
		fprintf(stderr, "cpps\nerror: Directory \"%s\" already exist.\n", project_name);
		goto cpps_error;
	}
	if (flag == 1) {printf("cpps\nInfo: New project '%s' is created.\n", project_name);}
	skip_project_creation:;
	return 0;

	cpps_error:;
	return 1;
}


