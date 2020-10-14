#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "limits.h"

#define SUCCESS_TEMPLATE "The minimal price is: %d"
#define USAGE_TEMPLATE "Usage: RailwayPlanner <InputFile>"
#define INVALID_INPUT_TEMPLATE "Invalid input in line: %d."
#define OUTPUT_PATH "railway_planner_output.txt"
#define FILE_EMPTY "File is empty."
#define FILE_DOESNT_EXISTS "File doesn't exists."
#define INVALID_VALUE -1
#define RAILS_START_INDEX 4
#define FIRST_ROW 1
#define SECOND_ROW 2
#define THIRD_ROW 3
#define MAX_LINE 1024
#define SEPARATOR ','
#define INITIAL_ALLOC 15

/**
 * struct that holds the properties of a rail
 */
typedef struct Railroad
{
	int length;
	int price;
	char startPart;
	char endPart;
	int startPartIndex;
} road;

/**
 * opens the output file, enters the relevant error and frees the memory
 * @param file the input file
 * @param rowIndex the index with the error
 * @param symbols the allocation of the symbols
 * @param railsArr the rails array
 */
void outputExit(FILE* file, int rowIndex, char* symbols, road* railsArr)
{
	FILE* output = fopen(OUTPUT_PATH, "w");
	if(output != NULL)
	{
		fprintf(output, INVALID_INPUT_TEMPLATE, rowIndex);
		fclose(output);
	}
	fclose(file);
	if(symbols != NULL)
	{
		free(symbols);
	}
	if(railsArr != NULL)
	{
		free(railsArr);
	}
	exit(EXIT_FAILURE);
}

/**
 * frees the memory if the allocation failed
 * @param file the input file
 * @param symbols the symbols array
 * @param railsArr the rails array
 */
void allocFail(FILE* file, char* symbols, road* railsArr)
{
	if(file != NULL)
	{
		fclose(file);
	}
	if(symbols != NULL)
	{
		free(symbols);
	}
	if(railsArr != NULL)
	{
		free(railsArr);
	}
	exit(EXIT_FAILURE);
}

/**
 * check if there are 2 args from the command line
 * @param argc number of args from the command line
 */
void invalidArgs(int argc)
{
	if(argc != 2)
	{
		FILE* output = fopen(OUTPUT_PATH, "w");
		if(output != NULL)
		{
			fprintf(output, USAGE_TEMPLATE);
			fclose(output);
		}
		exit(EXIT_FAILURE);
	}
}

/**
 * checks if the input file is valid
 * @param file the input file
 */
void invalidFile(FILE* file)
{
	if(file == NULL)
	{
		FILE* output = fopen(OUTPUT_PATH, "w");
		if(output != NULL)
		{
			fprintf(output, FILE_DOESNT_EXISTS);
			fclose(output);
		}
		exit(EXIT_FAILURE);
	}
}

/**
 * check if a symbol is valid
 * @param symbol the symbol to check
 * @param validSymbols array with the valid symbols
 * @return -1 if invalid symbol, else the location of the symbol in the array
 */
int checkValidSymbol(char symbol, char *validSymbols)
{
	int listLength = (int) strlen(validSymbols);
	for(int i = 0; i < listLength; i++)
	{
		if(symbol == validSymbols[i])
		{
			return i;
		}
	}
	return INVALID_VALUE;
}

/**
 * check if a line of rail is valid
 * @param line the line of the rail
 * @param symbols array of the valid symbols
 * @return -1 if invalid, else the location of the start symbol in the array symbols
 */
road checkRoadInput(char line[], char* symbols, FILE* file, int rowIndex, road* railsArr)
{
	road temp;
	int flag = true;
	char *chunk;
	chunk = strtok(line, ",");
	int counter = 0, startPartLoc = 0, endPartLoc = 0;
	while(chunk != NULL)
	{
		if(counter == 0)
		{
			if(strlen(chunk) != 1)
			{
				flag = false;
				break;
			}
			startPartLoc = checkValidSymbol(*chunk, symbols);
			if(startPartLoc == INVALID_VALUE)
			{
				flag = false;
				break;
			}
			temp.startPart = *chunk;
			temp.startPartIndex = startPartLoc;
		}
		else if(counter == 1)
		{
			if(strlen(chunk) != 1)
			{
				flag = false;
				break;
			}
			endPartLoc = checkValidSymbol(*chunk, symbols);
			if(endPartLoc == INVALID_VALUE)
			{
				flag = false;
				break;
			}
			temp.endPart = *chunk;
		}
		else if(counter == 2)
		{
			char *ptr = NULL;
			long length = strtol(chunk, &ptr, 10);
			if(strlen(ptr) != 0 || length <= 0 || length > INT_MAX)
			{
				flag = false;
				break;
			}
			temp.length = (int) length;
		}
		else if(counter == 3)
		{
			char *ptr = NULL;
			long price = strtol(chunk, &ptr, 10);
			if(strlen(ptr) != 0 || price > INT_MAX || price <= 0)
			{
				flag = false;
				break;
			}
			temp.price = (int) price;
		}
		chunk = strtok(NULL, ",");
		counter++;
	}
	if(flag == false || counter != 4)
	{
		outputExit(file, rowIndex, symbols, railsArr);
	}
	return temp;
}

/**
 * checks if the requested length and the number of connections is valid
 * @param line string with data
 * @return EXIT_FAILURE if invalid, else EXIT_SUCCESS
 */
int checkLengthAndConnections(char line[])
{
	long num;
	char *ptr = NULL;
	num = strtol(line, &ptr, 10);
	if (strlen(ptr) != 0)
	{
		return EXIT_FAILURE;
	}
	if(0 > num)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/**
 * parses the input line according the template.
 * @param line the input line
 * @param symbols empty array of symbols
 * @param numOfConnections number of connections
 * @return pointer to the symbols array if valid, else terminates the program
 */
char* parseSymbols(char *symbols, int numOfConnections, FILE* file)
{
	int validLength = (numOfConnections * 2) - 1;
	int arrIndex = 0, flag = true;
	char *line = (char *) malloc((validLength + 3)*sizeof(char));
	char *temp = fgets(line, validLength + 3, file);
	if(temp == NULL || line == NULL)
	{
		free(line);
		allocFail(file, symbols, NULL);
	}
	symbols = (char *) malloc((numOfConnections + 1)*sizeof(char));
	if(symbols == NULL)
	{
		free(line);
		allocFail(file, symbols, NULL);
	}
	line[strcspn(line, "\n")] = 0;
	for(int i = 0; i < validLength; i++)
	{
		if((i % 2) == 1)
		{
			if(line[i] != SEPARATOR)
			{
				flag = false;
				break;
			}
		}
		else
		{
			if(line[i] == SEPARATOR)
			{
				flag = false;
				break;
			}
			arrIndex++;
			symbols[arrIndex - 1] = line[i];
		}
	}
	free(line);
	if(flag == false)
	{
		outputExit(file, THIRD_ROW, symbols, NULL);
	}
	symbols[arrIndex] = '\0';
	return symbols;
}

/**
 * parses the first line of the file
 * @param line the first line
 * @return returns the requested length if the input is valid, else terminates
 */
int parseFirstLine(char *line, FILE* file)
{
	char *ptr = NULL;
	if(checkLengthAndConnections(line) == EXIT_FAILURE)
	{
		outputExit(file, FIRST_ROW, NULL, NULL);
	}
	else
	{
		return (int) strtol(line, &ptr, 10);
	}
	return EXIT_SUCCESS;
}

/**
 * parses the second line of the file
 * @param line the second line
 * @return returns the amount of connections if the input is valid, else terminates
 */
int parseSecondLine(FILE* file)
{
	char *ptr = NULL;
	int flag = true, number = 0;
	char line[MAX_LINE];
	char *temp = fgets(line, MAX_LINE, file);
	if(temp == NULL)
	{
		allocFail(file, NULL, NULL);
	}
	line[strcspn(line, "\n")] = 0;
	if(checkLengthAndConnections(line) == EXIT_FAILURE)
	{
		flag = false;
	}
	else
	{
		number = (int) strtol(line, &ptr, 10);
		if(number == 0)
		{
			flag = false;
		}
	}
	if(flag == false)
	{
		outputExit(file, SECOND_ROW, NULL, NULL);
	}
	return number;
}

/**
 * parses the lines that contains the data on the rails
 * @param file the input file
 * @param railsArr array of road variables
 * @param symbols array of the symbols
 * @param rowIndex the current row index
 * @param railsArrLength length of the rails array
 * @return if valid input returns a pointer to the road array, else terminates
 */
road* parseRoads(FILE* file, road* railsArr, char* symbols, int rowIndex, int* railsArrLength)
{
	char line[MAX_LINE];
	int railsIndex = 0, alloc_counter = 1;
	railsArr = (road *) malloc(INITIAL_ALLOC * sizeof(road));
	if(railsArr == NULL)
	{
		allocFail(file, symbols, railsArr);
	}
	while(fgets(line, MAX_LINE, file) != NULL)
	{
		line[strcspn(line, "\n")] = 0;
		road checkOutput = checkRoadInput(line, symbols, file, rowIndex, railsArr);
		rowIndex++;
		railsIndex++;
		if(railsIndex >= INITIAL_ALLOC * alloc_counter)
		{
			alloc_counter++;
			railsArr = (road *) realloc(railsArr, alloc_counter*INITIAL_ALLOC*sizeof(road));
			if(railsArr == NULL)
			{
				allocFail(file, symbols, railsArr);
			}
		}
		railsArr[railsIndex - 1] = checkOutput;
	}
	*railsArrLength = railsIndex;
	return railsArr;
}

/**
 * checks if the file is empty and parses the first row
 * @param file the input file
 * @return if valid line the required length, else closes file and terminates
 */
int firstLine(FILE* file)
{
	char line[MAX_LINE];
	char *temp = fgets(line, MAX_LINE, file);
	if(temp == NULL)
	{
		FILE* output = fopen(OUTPUT_PATH, "w");
		if(output != NULL)
		{
			fprintf(output, FILE_EMPTY);
			fclose(output);
		}
		fclose(file);
		exit(EXIT_FAILURE);
	}
	line[strcspn(line, "\n")] = 0;
	int length = parseFirstLine(line, file);
	return length;
}

/**
 * calculates using the data in the results table the min price to build a rail given the parms
 * @param symbol the symbol that is being checked
 * @param length the length that is being checked
 * @param railsArr array of roads
 * @param railsArrLength the length of the roads array
 * @param numOfConnections the number of valid symbols
 * @param results the result table
 * @return the minimum price
 */
int helper(char symbol, int length, road* railsArr, int railsArrLength, int rowLen,
		   const int* results)
{
	int min = INT_MAX;
	if(length == 0)
	{
		return 0;
	}
	for(int i = 0; i < railsArrLength; i++)
	{
		road temp = railsArr[i];
		if(temp.endPart == symbol && length - temp.length >= 0)
		{
			int addedPrice = *(results + (length-temp.length + 1)*rowLen + temp.startPartIndex + 1);
			if(addedPrice != INT_MAX && temp.price != INT_MAX)
			{
				int val = temp.price + addedPrice;
				if (val < min)
				{
					min = val;
				}
			}
		}
	}
	return min;
}

/**
 * caculates the min price of the last row in the table
 * @param arr the last row in the table
 * @param length the length of the row
 * @return the min price
 */
int findMinInTable(const int arr[], int length)
{
	int min = INT_MAX;
	for(int i = 1; i < length; i++)
	{
		if(arr[i] < min)
		{
			min = arr[i];
		}
	}
	return min;
}

/**
 * creates the results table using the helper function and finds the min price
 * @param railsArr array of roads
 * @param length the given length
 * @param numOfConnections the number of symbols
 * @param symbols array of symbols
 * @param railsArrLength the length of the roads array
 * @return the min price that was asked in the file
 */
int findMinPrice(road* railsArr, int length, int numOfConnections, char* symbols,
				 int railsArrLength)
{
	int rowLen = numOfConnections + 1;
	int colLen = length + 2;
	int *resultsTable = (int *) calloc(colLen*rowLen, sizeof(int));
	if(resultsTable == NULL)
	{
		allocFail(NULL, symbols, railsArr);
	}
	int minPrice = 0;
	for(int i = 1; i < rowLen; i++) // inserting the symbols to the first row
	{
		*(resultsTable + i) = symbols[i-1];
	}
	for(int j = 1; j < colLen; j++) // inserting the length to the first col
	{
		*(resultsTable + j*rowLen) = j-1;
	}
	for(int i = 1; i < colLen; i++)
	{
		for(int j = 1; j < rowLen; j++)
		{
			*(resultsTable + i*rowLen + j) = helper((char) *(resultsTable + j), *
			                                                        (resultsTable + i*rowLen),
			                                                        railsArr, railsArrLength,
			                                                        rowLen, resultsTable);
		}
	}
	minPrice = findMinInTable((resultsTable + (rowLen*(length + 1))), rowLen);
	free(resultsTable);
	if(minPrice == INT_MAX || minPrice < 0)
	{
		return INVALID_VALUE;
	}
	return minPrice;
}

/**
 * the main function. checks if the given arguments are valid and calls to all of the functions
 * above to find the min price of the rail.
 * if the program terminates successfully the price will be in the output file, else an error will
 * be in the output file.
 */
int main(int argc, char* argv[])
{
	invalidArgs(argc);
	FILE *file = fopen(argv[1], "r");
	invalidFile(file);
	int lengthRailsArr = 0;
	char *symbols = NULL;

	//Handles the first line
	int length = firstLine(file);

	//Handles the second line
	int numOfConnections = parseSecondLine(file);

	//Handles the third line
	symbols = parseSymbols(symbols, numOfConnections, file);

	//Parses the rails
	road *railsArr = NULL;
	railsArr = parseRoads(file, railsArr, symbols, RAILS_START_INDEX, &lengthRailsArr);
	fclose(file);

	//Finds the min price
	int minPrice = findMinPrice(railsArr, length, numOfConnections, symbols, lengthRailsArr);
	FILE* output = fopen(OUTPUT_PATH, "w");
	if(output != NULL)
	{
		fprintf(output, SUCCESS_TEMPLATE, minPrice);
		fclose(output);
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	//Frees memory and terminates
	free(symbols);
	free(railsArr);
	symbols	= NULL;
	railsArr = NULL;
	exit(EXIT_SUCCESS);
}
