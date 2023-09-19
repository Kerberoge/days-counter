#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

void warnAndExit (void)
{
	fprintf(stderr, "Error: invalid date: date must be of dd-mm-yyyy format\n");
	exit(1);
}

int isLeapYear (int year)
{
	if ( ( year % 100 == 0 && year % 400 == 0 ) || ( year % 100 != 0 && year % 4 == 0 ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int numberOfDaysInMonth (int month, int year)
{
	int numberOfDays;
	switch ( month )
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			numberOfDays = 31;
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			numberOfDays = 30;
			break;

		case 2:
			if ( isLeapYear(year) )
			{
				numberOfDays = 29;
			}
			else
			{
				numberOfDays = 28;
			}
			break;
	}
	
	return numberOfDays;
}

int* parseInput (int* argc, char*** argv)
{

	// check if the user provided only one argument
	if (*argc < 2)
	{
		fprintf(stderr, "Error: no date supplied\n");
		exit(1);
	}
	else if (*argc > 2)
	{
		fprintf(stderr, "Error: too many arguments\n");
		exit(1);
	}

	// place day, month and year into an array of strings and check the strings
	char date_strings[3][10] = { 0, 0, 0 };
	int current_pos = 0;

	for (int i = 0; i < strlen( (*argv)[1] ); i++)
	{
		if ( current_pos > 2 )
		{
			break;
		}
		else if ( (*argv)[1][i] == '-' || (*argv)[1][i] == '/' || (*argv)[1][i] == '.' )
		{
			current_pos++;
		}
		else if ( isdigit( (*argv)[1][i] ) )
		{
			strncat(date_strings[current_pos], &(*argv)[1][i], 1);
		}
		else
		{
			warnAndExit();
		}
	}

	if ( current_pos > 2 )
	{
		warnAndExit();
	}

	for (int i = 0; i < 3; i++)
	{
		if ( strlen(date_strings[i]) == 0 )
		{
			warnAndExit();
		}
	}

	// convert the strings to numbers and place them into an array
	int* date = calloc(3, sizeof(int));
	for (int i = 0; i < 3; i++)
	{
		date[i] = atoi(date_strings[i]);
	}

	// check month
	if ( date[1] > 12 || date[1] == 0 )
	{
		warnAndExit();
	}

	// check day
	if ( date[0] == 0 || date[0] > numberOfDaysInMonth(date[1], date[2]) )
	{
		warnAndExit();
	}

	return date;
}

int main (int argc, char** argv)
{

	int* user_date = parseInput(&argc, &argv);

	// get current date
	int current_date[3];
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	current_date[0] = time.tm_mday;
	current_date[1] = time.tm_mon + 1;
	current_date[2] = time.tm_year + 1900;

	// verify that provided date isn't in the future
	if ( user_date[2] > current_date[2] ||
		(user_date[1] > current_date[1] && user_date[2] == current_date[2]) ||
		(user_date[0] > current_date[0] && user_date[1] == current_date[1] && user_date[2] == current_date[2]) )
	{
		fprintf(stderr, "Error: date is in the future\n");
		exit(1);
	}

	int amount_of_days = 0;

	// count days till the same date in the year in current_date
	for (int i = user_date[2]; i < current_date[2]; i++)
	{
		if ( isLeapYear(i) )
		{
			amount_of_days += 366;
		}
		else
		{
			amount_of_days += 365;
		}
	}

	// add to or subtract from amount_of_days till the date in current_date is reached
	if ( user_date[1] < current_date[1] )
	{
		amount_of_days += numberOfDaysInMonth(user_date[1], current_date[2]) - user_date[0];

		for (int i = user_date[1] + 1; i < current_date[1]; i++)
		{
			amount_of_days += numberOfDaysInMonth(i, current_date[2]);
		}

		amount_of_days += current_date[0];
	}
	else if	( user_date[0] < current_date[0] && user_date[1] == current_date[1] )
	{
		amount_of_days += current_date[0] - user_date[0];
	}
	else if ( user_date[1] > current_date[1] )
	{
		amount_of_days -= user_date[0];

		for (int i = user_date[1] - 1; i > current_date[1]; i--)
		{
			amount_of_days -= numberOfDaysInMonth(i, current_date[2]);
		}

		amount_of_days -= numberOfDaysInMonth(current_date[1], current_date[2]) - current_date[0];
	}
	else if ( user_date[0] > current_date[0] && user_date[1] == current_date[1] )
	{
		amount_of_days -= user_date[0] - current_date[0];
	}

	printf("Number of days that have passed since %02d-%02d-%04d: %d\n", user_date[0], user_date[1], user_date[2], amount_of_days);

	return 0;
}
