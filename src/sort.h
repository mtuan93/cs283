#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int compare(const void *x, const void *y)
{
	return *(char *)x - *(char *)y;
}

char *sort_string(char s[])
{
	char *result;
	result = malloc( (size_t)strlen(s) );
	strcpy(result, s);
 
	//if the first result is uppercase, set it to lowercase
	if(result[0] >= 'A' && result[0] <= 'Z')
		result[0] += 32;

	qsort(result, (size_t)strlen(result), (size_t)sizeof(char), compare);

	return result;
}
