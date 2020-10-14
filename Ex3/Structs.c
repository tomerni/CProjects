#include <string.h>
#include <stdlib.h>
#include "Structs.h"

#define EQUAL (0)
#define LOWER (-1)
#define GREATER (1)
#define FAIL (0)
#define SUCCESS (1)

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
	char* word1 = (char *) a;
	char* word2 = (char *) b;
	int compareVal = strcmp(word1, word2);
	return compareVal;
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	if(word == NULL || pConcatenated == NULL)
	{
		return FAIL;
	}
	char* concat = (char *) pConcatenated;
	char* nWord = (char *) word;
	strcat(concat, nWord);
	strcat(concat, "\n");
	return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
	char* data = (char *) s;
	free(data);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	Vector* vector1 = (Vector *) a;
	Vector* vector2 = (Vector *) b;
	int minLength = 0;
	if(vector1->len <= vector2->len)
	{
		minLength = vector1->len;
	}
	else
	{
		minLength = vector2->len;
	}
	for(int i = 0; i < minLength; i++)
	{
		if(vector1->vector[i] > vector2->vector[i])
		{
			return GREATER;
		}
		else if(vector1->vector[i] < vector2->vector[i])
		{
			return LOWER;
		}
	}
	if(vector1->len == vector2->len)
	{
		return EQUAL;
	}
	else if(vector1->len < vector2->len)
	{
		return LOWER;
	}
	else
	{
		return GREATER;
	}
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
	Vector* vector = (Vector *) pVector;
	free(vector->vector);
	free(vector);
}

/**
 * calculates the norm of the vector
 * @param vector the current vector
 * @return the norm of the vector
 */
double normCalc(Vector* vector)
{
	double sum = 0;
	for(int i = 0; i < vector->len; i++)
	{
		sum += vector->vector[i] * vector->vector[i];
	}
	return sum;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	if(pVector == NULL || pMaxVector == NULL)
	{
		return FAIL;
	}
	Vector* vec = (Vector *) pVector;
	Vector* maxVec = (Vector *) pMaxVector;
	if(vec->vector == NULL || vec->len == 0)
	{
		return FAIL;
	}
	double pVectorNorm = normCalc(vec);
	size_t vecSize = sizeof(double) * vec->len;
	if(maxVec->vector == NULL)
	{
		maxVec->vector = (double *) malloc(vecSize);
		memcpy(maxVec->vector, vec->vector, vecSize);
		maxVec->len = vec->len;
	}
	else if(pVectorNorm > normCalc(maxVec))
	{
		maxVec->vector = (double *) realloc(maxVec->vector, vecSize);
		memcpy(maxVec->vector, vec->vector, vecSize);
		maxVec->len = vec->len;
	}
	return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if(tree == NULL)
	{
		return NULL;
	}
	Vector* maxNorm = (Vector *) malloc(sizeof(Vector));
	maxNorm->vector = NULL;
	int res = forEachRBTree(tree, copyIfNormIsLarger, maxNorm);
	if(res == 0)
	{
		return NULL;
	}
	return maxNorm;
}
