#include <getopt.h>
#include <io.h>
#include "functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
	char c_source_filename[260] = "main", c_source_file_loc[260] = "projectname\\src\\",
		 exe_filename[260], project_name[260];
	int getopt_status;

	strcpy(project_name, argv[1]);
	strcpy(exe_filename, argv[1]);
	while(true) {
        int option_index = 0;
        static struct option long_options[] = {
            {"exe-name", required_argument, 0,  0},
            {"filename", required_argument, 0,  0},
            {"verbose",  no_argument,       0,  0},
            {0,          0,                 0,  0}
        };

		opterr = 0;
		getopt_status = getopt_long(argc, argv, "e:f:v", long_options, &option_index);
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
			case '?':
				fprintf(stderr, "cpps\nError: Unknown option of \"%c\"\n", optopt);
				break;
			case ':':
				fprintf(stderr, "cpps\nWarning: Missing argument for option -%c/--%s\n", optopt, long_options[option_index].name);
				break;
        }
    }

	if (make_pds(project_name) != 0) {goto cpps_error;}

	strcat(c_source_file_loc, c_source_filename);
	strcpy(c_source_file_loc, strrep(c_source_file_loc, "projectname", project_name));
	if (make_csf(c_source_file_loc) != 0) {goto cpps_error;}

	_chdir(project_name);
	if (make_makefile(exe_filename) != 0) {goto cpps_error;}
	_chdir("..");

	return 0;

	cpps_error:;
	return 1;
}


