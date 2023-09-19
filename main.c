#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void warnAndExit (void) {
	fprintf(stderr, "Error: invalid date: date must be of dd-mm-yyyy format\n");
	exit(1);
}

int* parseInput (int* argc, char*** argv) {

	// check if the user provided only one argument
	if (*argc < 2) {
		fprintf(stderr, "Error: no date supplied\n");
		exit(1);
	} else if (*argc > 2) {
		fprintf(stderr, "Error: too many arguments\n");
		exit(1);
	}

	// place day, month and year into an array of strings and check the strings
	char date_strings[3][10] = { 0, 0, 0 };
	int current_pos = 0;

	for (int i = 0; i < strlen( (*argv)[1] ); i++) {
		if ( current_pos > 2 ) {
			break;
		} else if ( (*argv)[1][i] == '-' || (*argv)[1][i] == '/' || (*argv)[1][i] == '.' ) {
			current_pos++;
		} else if ( isdigit( (*argv)[1][i] ) ) {
			strncat(date_strings[current_pos], &(*argv)[1][i], 1);
		} else {
			warnAndExit();
		}
	}

	if ( current_pos > 2 ) {
		warnAndExit();
	}

	for (int i = 0; i < 3; i++) {
		if ( strlen(date_strings[i]) == 0 ) {
			warnAndExit();
		}
	}

	// convert the strings to numbers and place them into an array
	int* date = calloc(3, sizeof(int));
	for (int i = 0; i < 3; i++) {
		date[i] = atoi(date_strings[i]);
	}

	// check month
	if ( date[1] > 12 || date[1] == 0 ) {
		warnAndExit();
	}

	// check day
	if ( date[0] == 0 ) {
		warnAndExit();
	}

	switch ( date[1] ) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if ( date[0] > 31 ) {
				warnAndExit();
			}
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			if ( date[0] > 30 ) {
				warnAndExit();
			}
			break;

		case 2:
			if ( date[0] > 29 ) {
				warnAndExit();
			}

			if ( date[0] == 29 ) {
				if ( date[2] % 100 == 0 && date[2] % 400 != 0 || date[2] % 4 != 0 ) {
					warnAndExit();
				}
			}
			break;
	}

	return date;
}

int main (int argc, char** argv) {

	int* array = parseInput(&argc, &argv);

	printf("contents of for loop: ");
	for (int i = 0; i < 3; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	return 0;
}
