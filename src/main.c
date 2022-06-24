#include <getopt.h>
#include "functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
	char exe_filename[260], c_source_filename[260];
	int getopt_status;

	while(true) {
        int option_index = 0;
        static struct option long_options[] = {
            {"exe-name", required_argument, 0,  0 },
            {"filename", required_argument, 0,  0 },
            {"verbose",  no_argument,       0,  0 },
            {0,          0,                 0,  0 }
        };

		opterr = 0;
		getopt_status = getopt_long(argc, argv, ":e:f:v", long_options, &option_index);
        if (getopt_status == -1) {
			break;
		}

		switch(getopt_status) {
			case 'e':
				strcpy(exe_filename, optarg);
				printf("Executable file name is: %s\n", exe_filename);
				break;
			case 'f':
				strcpy(c_source_filename, optarg);
				printf("C source file name is: %s\n", c_source_filename);
				break;
			case '?':
				fprintf(stderr, "cpps\nError: Unknown option of \"%c\"\n", optopt);
				break;
			case ':':
				fprintf(stderr, "cpps\nWarning: Missing argument for option -%c/--%s\n", optopt, long_options[option_index].name);
				break;
        }
    }

	if (make_pds(argv[1]) != 0) {goto cpps_error;}

	return 0;

	cpps_error:;
	return 1;
}


