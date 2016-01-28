#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic.h"
#include "sort.h"

struct arrayOfDict {
	int length;
	int	maxLen;
	struct dict **dicts;
};

typedef struct arrayOfDict arrayOfDict;

arrayOfDict *buildDict(int size) {
	arrayOfDict *returnedDict = NULL;
	// handle invalid cases
	if(size < 1) return NULL;
	if((returnedDict = malloc(sizeof(dict))) == NULL) return NULL;
	if((returnedDict->dicts = malloc(size * sizeof(bucket *))) == NULL) return NULL;
	// build the dictionary
	returnedDict->length = size;
	int i;
	for(i = 0; i < size; i++) {
		returnedDict->dicts[i] = NULL;
	}
	return returnedDict;
}

arrayOfDict *buildArrayOfDict(int size) {
	arrayOfDict *array = NULL;
	// some invalid cases
	if(size < 1) return NULL;
	if((array = malloc(sizeof(arrayOfDict))) == NULL) return NULL;
	if((array->dicts = malloc(size * sizeof(dict *))) == NULL) return NULL;
	// start construct the array of dictionaries
	array->maxLen = 0;
	array->length = size;
	int i;
	for(i = 0; i < size; i++) {
		array->dicts[i] = NULL;
	}
	return array;
}

void deleteDict(dict *dict) {
	int i;
	bucket *nextWord;
	bucket *prevWord;
	for(i = 0; i < dict->size; i++) {
		nextWord = dict->words[i];
		while(nextWord != NULL) {
			free(nextWord->value);
			prevWord = nextWord;
			nextWord = nextWord->next;
			free(prevWord);
		}
	}
	free(dict->words);
	free(dict);
}

int hash(char* word) {
	int i;
	int hashCode;
	hashCode = 0;
	int temp = (int)strlen(word);
	for(i = 0; i < temp; i++) {
		if(word[i] == 39) {
			temp -= 2;
			break;
		}
		if(word[i] > 96 && word[i] < 123) {
			hashCode += (int)word[i]-96;
		} else if(word[i] > 64 && word[i] < 91) {
			hashCode += (int)word[i]-64;
		} else {
			return -1;
		}
	}
	return hashCode - temp;
}

bucket *buildEntry(char* item, int bucketNumber) {
	struct bucket *toReturn;
	int len = strlen(item);
	if((toReturn = malloc(sizeof(bucket))) == NULL) return NULL;
	if((toReturn->value = memcpy(malloc(len), item, len)) == NULL) return NULL;
	toReturn->next = NULL;
	toReturn->key = bucketNumber;
	return toReturn;
}

insertEntry(char *item, arrayOfDict *array) {
	int len = (int)strlen(item);
	if(array->dicts[len-1] == NULL) array->dicts[len-1] = buildDict(len*13);
	dict *current = array->dicts[len-1];
	int bucketNumber = hash(item);
	if(bucketNumber < 0) return;
	if(bucketNumber >= current->size) {
		int i = current->size;
		current->size = bucketNumber+1;
		current->words = realloc(current->words, current->size * sizeof(char *));
		for(i; i < current->size-1; i++) {
			current->words[i] = NULL;
		}
	}
	bucket *newWord = NULL;
	bucket *lastWord = NULL;
	bucket *next = NULL;
	newWord = buildEntry(item, bucketNumber);
	next = current->words[bucketNumber];
	int count = 0;
	while(next != NULL && next->value != NULL) {
		lastWord = next;
		next = next->next;
		count++;
	}
	if(count > array->maxLen) array->maxLen = count;
	if(next == current->words[bucketNumber]) {
		newWord->next = next;
		current->words[bucketNumber] = newWord;
	} else {
		lastWord->next = newWord;
	}
}

char **getWord(char *searchWord, arrayOfDict *array) {
	bucket **bucketEntry = array->dicts[strlen(searchWord)-1]->words;
	int code = hash(searchWord);
	if(bucketEntry[code] == NULL) return NULL;
	char **anagrams = malloc(52 * sizeof(char *));
	bucket *next = bucketEntry[code];
	int i = 0;
	while(next != NULL && next->value != NULL) {
		char *temp;
		int length = strlen(next->value);
		temp = memcpy(malloc(length), sort_string(next->value), length );
		if(strcmp(temp, sort_string(searchWord)) == 0) anagrams[i++] = next->value;
		next = next->next;
	}
	return anagrams;
}

char **scrabble(char *search, char letter, int place, arrayOfDict *ray) {
	bucket **bucketEntry = ray->dicts[strlen(search)-1]->words;
	int code = hash(search);
	if(bucketEntry[code] == NULL)
		return NULL;
	char **results = malloc(52 * sizeof(char *));
	bucket *next = bucketEntry[code];
	int i = 0;
	while(next != NULL && next->value != NULL) {
		char *temp;
		int length = strlen(next->value);
		temp = memcpy(malloc(length), sort_string(next->value), length );
		if(strcmp(temp, sort_string(search)) == 0 &&
				next->value[place] == letter) results[i++] = next->value;
		next = next->next;
	}
	return results;
}
