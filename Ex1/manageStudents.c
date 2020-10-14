#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_STUDENTS 5500
#define MAX_CHARS_ROW 61
#define MAX_CHARS_FIELD 42
#define SPACE 32
#define HYPHEN 45
#define ID_LENGTH 11
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define FAIL 1
#define SUCCESS 0
#define INPUT_FORMAT "%42[^,],%42[^,],%d,%d,%42[^,],%42[^\n]"
#define GET_INPUT "Enter student info. To exit press q, then enter\n"
#define INVALID_ARG "USAGE: invalid argument"
#define NUMBER_ARGS "USAGE: wrong number of arguments"
#define ERR_EOF "ERROR: could not read info\nin line %d\n"
#define ERR_INPUT_LENGTH "ERROR: can't read more inputs\nin line %d\n"
#define ERR_FORMAT "ERROR: info must match specified format\nin line %d\n"
#define ERR_ID "ERROR: id must be a 10 digits number that does not start with 0\nin line %d\n"
#define ERR_NAME "ERROR: name can only contain alphabetic characters, whitespaces or '-'\nin line %d\n"
#define ERR_AGE "ERROR: age must be an integer between 18 and 120\nin line %d\n"
#define ERR_GRADE "ERROR: grade must be an integer between 0 and 100\nin line %d\n"
#define ERR_CITY "ERROR: city can only contain alphabetic characters or '-'\nin line %d\n"
#define ERR_COUNTRY "ERROR: country can only contain alphabetic characters or '-'\nin line %d\n"
#define PRINT_FORMAT "%s,%s,%d,%d,%s,%s\n"
#define BEST_STUDENT_FORMAT "best student info is: %s,%s,%d,%d,%s,%s\n"

/**
 * A student struct that contains all of the field of the student - id, name, grade, age, country
 * and city
 */
typedef struct Student
{
	char id[MAX_CHARS_FIELD];
	char name[MAX_CHARS_FIELD];
	int grade;
	int age;
	char country[MAX_CHARS_FIELD];
	char city[MAX_CHARS_FIELD];
} Student;

/**
 * executes the mission of part 1 - gets the input from the user and finds the best student
 */
void bestStudent();
/**
 * Checks if the input of the user is valid
 * @param str - the input string from the user
 * @param lineIndex - the index of the line that the user entered
 * @return 1 if invalid 0 if valid
 */
int checkInput(char str[], int lineIndex);
/**
 * finds the best student according the grade/age formula
 * @param students - array of students
 * @param arrIndex - the number of students that the user entered to the array
 * @return the index of the best student
 */
int findBestStudent(struct Student students[], int arrIndex);
/**
 * checks if the id is valid
 * @param str - the string of the id
 * @return 1 if invalid 0 if valid
 */
int isValidId(char str[]);
/**
* checks if the city and country are valid
* @param str - the string of the city or country
* @return 1 if invalid 0 if valid
*/
int isValidLocation(char str[]);
/**
 * checks if the name is valid
 * @param str - the string of the name
 * @return 1 if invalid 0 if valid
 */
int isValidName(char str[]);
/**
 * responsible on the input of the user
 * @param students - empty array of students
 * @return the number of rows that were entered to the array
 */
int inputLoop(Student students[]);
/**
 * executes the sort according to the merge sort algorithm
 */
void mergeSort();
/**
 * the merge sort algorithm
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 */
void mergeSortHelper(Student students[], int start, int end);
/**
 * merges between the two sorted parts of the array
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 * @param middle index to the middle of the array
 */
void merge(Student students[], int start, int middle, int end);
/**
 * executes the sort according to the quick sort algorithm
 */
void quickSort();
/**
 * responsible for the recursion
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 */
void quickSortMain(Student students[], int start, int end);
/**
 * switches between the students according the sort
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 * @return the index of the new pivot
 */
int quickSortHelper(Student students[], int start, int end);
/**
 * print the array
 * @param students array of students
 * @param arrIndex the length of the array
 */
void printArray(Student students[], int arrIndex);

/**
 * the main function of the program
 */
int f(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf(NUMBER_ARGS);
		return FAIL;
	}
	if(strcmp(argv[1], "best") == 0)
	{
		bestStudent();
	}
	else if(strcmp(argv[1], "merge") == 0)
	{
		mergeSort();
	}
	else if(strcmp(argv[1], "quick") == 0)
	{
		quickSort();
	}
	else
	{
		printf(INVALID_ARG);
		return FAIL;
	}
	return SUCCESS;
}

/**
 * executes the mission of part 1 - gets the input from the user and finds the best student
 */
void bestStudent()
{
	Student students[MAX_STUDENTS];
	int arrIndex = inputLoop(students);
	if(arrIndex > 0) // Checks if the input wasn't empty
	{
		int bestIndex = findBestStudent(students, arrIndex);
		printf(BEST_STUDENT_FORMAT, students[bestIndex].id, students[bestIndex].name,
				students[bestIndex].grade, students[bestIndex].age, students[bestIndex].country,
				students[bestIndex].city);
	}
}

/**
 * responsible on the input of the user
 * @param students - empty array of students
 * @return the number of rows that were entered to the array
 */
int inputLoop(Student students[])
{
	int lineIndex = 0, arrIndex = 0;
	while(1)
	{
		printf(GET_INPUT);
		char stud[MAX_CHARS_ROW];
		char *input = fgets(stud, MAX_CHARS_ROW, stdin);
		if(input == NULL)
		{
			printf(ERR_EOF, lineIndex);
			exit(FAIL);
		}
		else
		{
			if(strcmp(stud, "q\n") == 0)
			{
				break;
			}
			if(arrIndex > 5500)
			{
				printf(ERR_INPUT_LENGTH, lineIndex);
				lineIndex++;
			}
			else
			{
				int value = checkInput(stud, lineIndex);
				if(value == SUCCESS)
				{
					sscanf(stud, INPUT_FORMAT, students[arrIndex].id, students[arrIndex].name,
						   &students[arrIndex].grade, &students[arrIndex].age, students[arrIndex]
						   .country, students[arrIndex].city);
					students[arrIndex].city[strcspn(students[arrIndex].city, "\n")] = 0;
					arrIndex++;
					lineIndex++;
				}
				else
				{
					lineIndex++;
				}
			}
		}
	}
	return arrIndex;
}

/**
 * Checks if the input of the user is valid
 * @param str - the input string from the user
 * @param lineIndex - the index of the line that the user entered
 * @return 1 if invalid 0 if valid
 */
int checkInput(char str[], int lineIndex)
{
	char id[MAX_CHARS_FIELD], name[MAX_CHARS_FIELD], country[MAX_CHARS_FIELD], city[MAX_CHARS_FIELD];
	int age, grade;
	int formattingValue = sscanf(str, INPUT_FORMAT, id, name, &grade, &age, country, city);
	city[strcspn(city, "\n")] = 0; // removing \n from the city
	if(formattingValue != 6) // wrong number of fields in the input
	{
		printf(ERR_FORMAT, lineIndex);
		return FAIL;
	}
	if(strlen(id) != ID_LENGTH-1 || isValidId(id) == FAIL || id[0] == '0')
	{
		printf(ERR_ID, lineIndex);
		return FAIL;
	}
	if(isValidName(name) == FAIL)
	{
		printf(ERR_NAME, lineIndex);
		return FAIL;
	}
	if(grade < MIN_GRADE || grade > MAX_GRADE)
	{
		printf(ERR_GRADE, lineIndex);
		return FAIL;
	}
	if(age < MIN_AGE || age > MAX_AGE)
	{
		printf(ERR_AGE, lineIndex);
		return FAIL;
	}
	if(isValidLocation(country) == FAIL)
	{
		printf(ERR_COUNTRY,
			   lineIndex);
		return FAIL;
	}
	if(isValidLocation(city) == FAIL)
	{
		printf(ERR_CITY, lineIndex);
		return FAIL;
	}
	return SUCCESS;
}

/**
 * checks if the id is valid
 * @param str - the string of the id
 * @return 1 if invalid 0 if valid
 */
int isValidId(char str[])
{
	for(int i = 0; i < (int)strlen(str); i++)
	{
		if(isdigit((unsigned char) str[i]) == 0)
		{
			return FAIL;
		}
	}
	return SUCCESS;
}

/**
 * checks if the city and country are valid
 * @param str - the string of the city or country
 * @return 1 if invalid 0 if valid
 */
int isValidLocation(char str[])
{
	for(int i = 0; i < (int)strlen(str); i++)
	{
		if(isalpha((unsigned char) str[i]) == 0 && str[i] != HYPHEN)
		{
			return FAIL;
		}
	}
	return SUCCESS;
}

/**
 * checks if the name is valid
 * @param str - the string of the name
 * @return 1 if invalid 0 if valid
 */
int isValidName(char str[])
{
	for(int i = 0; i < (int)strlen(str); i++)
	{
		if(isalpha((unsigned char) str[i]) == 0 && str[i] != SPACE && str[i] != HYPHEN)
		{
			return FAIL;
		}
	}
	return SUCCESS;
}

/**
 * finds the best student according the grade/age formula
 * @param students - array of students
 * @param arrIndex - the number of students that the user entered to the array
 * @return the index of the best student
 */
int findBestStudent(struct Student students[], int arrIndex)
{
	double bestGrade = 0;
	int bestIndex = 0;
	for(int i = 0; i < arrIndex; i++)
	{
		int grade = students[i].grade;
		int age = students[i].age;
		double score = (double) grade / (double) age;
		if(score > bestGrade)
		{
			bestGrade = score;
			bestIndex = i;
		}
	}
	return bestIndex;
}

/**
 * executes the sort according to the merge sort algorithm
 */
void mergeSort()
{
	Student students[MAX_STUDENTS];
	int arrIndex = inputLoop(students);
	if(arrIndex > 0)
	{
		mergeSortHelper(students, 0, arrIndex - 1);
		printArray(students, arrIndex);
	}
}

/**
 * the merge sort algorithm
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 */
void mergeSortHelper(Student students[], int start, int end)
{
	int middle = 0;
	if(start < end)
	{
		middle = (start + end) / 2;
		mergeSortHelper(students, start, middle);
		mergeSortHelper(students, middle + 1, end);
		merge(students, start, middle, end);
	}
}

/**
 * merges between the two sorted parts of the array
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 * @param middle index to the middle of the array
 */
void merge(Student students[], int start, int middle, int end)
{
	int firstArrInd = start, secondArrInd = middle + 1, tempArrInd = start;
	Student tempArr[MAX_STUDENTS];
	while((firstArrInd <= middle) && (secondArrInd <= end)) // running on both arrays
	{
		if(students[firstArrInd].grade <= students[secondArrInd].grade)
		{
			tempArr[tempArrInd] = students[firstArrInd];
			firstArrInd++;
		}
		else
		{
			tempArr[tempArrInd] = students[secondArrInd];
			secondArrInd++;
		}
		tempArrInd++;
	}
	if(firstArrInd > middle) // finished all of the students in the first arr
	{
		for(int i = secondArrInd; i <= end; i++)
		{
			tempArr[tempArrInd] = students[i];
			tempArrInd++;
		}
	}
	else // finished all of the students in the second arr
	{
		for(int i = firstArrInd; i <= middle; i++)
		{
			tempArr[tempArrInd] = students[i];
			tempArrInd++;
		}
	}
	for(int i = start; i <= end; i++)
	{
		students[i] = tempArr[i];
	}
}

/**
 * executes the sort according to the quick sort algorithm
 */
void quickSort()
{
	Student students[MAX_STUDENTS];
	int arrIndex = inputLoop(students);
	if(arrIndex > 0)
	{
		quickSortMain(students, 0 , arrIndex - 1);
		printArray(students, arrIndex);
	}
}

/**
 * responsible for the recursion
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 */
void quickSortMain(Student students[], int start, int end)
{
	if(start < end)
	{
		int storeIndex = quickSortHelper(students, start, end);
		quickSortMain(students, start, storeIndex - 1);
		quickSortMain(students, storeIndex + 1, end);
	}
}

/**
 * switches between the students according the sort
 * @param students array of student
 * @param start index to the start of the array
 * @param end index to the end of the array
 * @return the index of the new pivot
 */
int quickSortHelper(Student students[], int start, int end)
{
	int smallestIndex = (start - 1);
	int pivotIndex = end;
	for(int j = start; j < end; j++)
	{
		if(strcmp(students[pivotIndex].name, students[j].name) > 0) // compares the names
		{
			smallestIndex++;
			Student temp = students[smallestIndex];
			students[smallestIndex] = students[j];
			students[j] = temp;
		}
	}
	Student temp = students[smallestIndex + 1];
	students[smallestIndex + 1] = students[pivotIndex]; // moving the pivot to his place
	students[pivotIndex] = temp;
	return (smallestIndex + 1);
}

/**
 * print the array
 * @param students array of students
 * @param arrIndex the length of the array
 */
void printArray(Student students[], int arrIndex)
{
	for(int i = 0; i <= arrIndex - 1; i++)
	{
		printf(PRINT_FORMAT, students[i].id, students[i].name, students[i].grade, students[i].age,
				students[i].country, students[i].city);
	}
}