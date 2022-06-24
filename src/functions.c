#include <ctype.h>
#include "functions.h"
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


