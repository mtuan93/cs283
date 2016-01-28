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

dict *buildDict(int size) {
	dict *returnedDict = NULL;
	// handle invalid cases
	if(size < 1) return NULL;
	if((returnedDict = malloc(sizeof(dict))) == NULL) return NULL;
	if((returnedDict->words = malloc(size * sizeof(bucket *))) == NULL) return NULL;
	// build the dictionary
	returnedDict->size = size;
	int i;
	for(i = 0; i < size; i++) {
		returnedDict->words[i] = NULL;
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

int hash(char* word)
{
	int i, hash;
	hash = 0;
	int temp = (int)strlen(word);
	for(i = 0; i < temp; i++) {
		if(word[i] == 39) {
			temp -= 2;
			break;
		}
		if(word[i] > 96 && word[i] < 123) {
			hash += (int)word[i]-96;
		} else if(word[i] > 64 && word[i] < 91) {
			hash += (int)word[i]-64;
		} else {
			return -1;
		}
	}
	return hash - temp;
}

bucket *buildEntry(char* value, int bucketNumber) {
	struct bucket *newWord;
	int length = strlen(value);
	if((newWord = malloc(sizeof(bucket)) ) == NULL)
		return NULL;
	if( (newWord->value = memcpy(malloc(length), value, length )) == NULL)
		return NULL;
	newWord->key = bucketNumber;//already found hash for newWord
	newWord->next = NULL;
	/*----END CREATING WORD----*/

	return newWord;
}

//insert should insert into the dictionary based on a word's hash
insert_entry(char *value, arrayOfDict *ray)
{
	int wordLength = (int)strlen(value);
	//if dictionary for this array doesn't exist, create one
	if(ray->dicts[wordLength-1] == NULL)
		//create dictionary of size based on word length * 13
		ray->dicts[wordLength-1] = buildDict(wordLength*13);

	//for ease of writing:
	dict *dict = ray->dicts[wordLength-1];//index a at 0 (instead of 1)

	int bucket = hash(value);
	if(bucket < 0)
		return;
	//if bucket is larger than dictionary size, resize dict
	if(bucket >= dict->size)
	{
		int i = dict->size;
		dict->size = bucket+1;
		dict->words = realloc(dict->words, dict->size * sizeof(char *));
		for(i; i < dict->size-1; i++)
			dict->words[i] = NULL;
	}

	word *newWord = NULL;
	word *next = NULL;
	word *lastWord = NULL;
	
	//creating new entry with malloc'd memory and correct values
	newWord = buildEntry(value, bucket);

	//traversal node
	next = dict->words[bucket];
	int count = 0;//list length count
	//find the last item in the bucket:
	while(next != NULL && next->value != NULL)
	{
		lastWord = next;
		next = next->next;
		count++;
	}
	//if new count for list size is bigger than current count
	if(count > ray->maxLen)
		ray->maxLen = count;

	/*----ACTUAL INSERTION----*/
	//if we didn't move anywhere in the bucket (first bucket entry)
	if(next == dict->words[bucket])
	{
		newWord->next = next;
		dict->words[bucket] = newWord;
	}
	else//reached the end of the list
	{
		lastWord->next = newWord;
	}
}

//only thing to check for in get_word() is if same letter are in both words
char **get_word(char *search, arrayOfDict *ray)
{
	//bucket of words = the length of the sesarch word - 1
	word **bucket = ray->dicts[strlen(search)-1]->words;
	int ash = hash(search);//hash of the search word
	//if hash doesn't exist
	if(bucket[ash] == NULL)
		return NULL;

	//find the anagrams of the search word
	//there *shouldn't* exist more than 7 anagrams of a word, but we'll
	//do 52 (the most recorded for a 1700k-word dictionary)
	char **anagrams = malloc(52 * sizeof(char *));
	word *next = bucket[ash];
	int i = 0;
	while(next != NULL && next->value != NULL)
	{
		//we already know word has same length, but does it have same chars?
		char *temp;
		int length = strlen(next->value);
		temp = memcpy(malloc(length), sort_string(next->value), length );
		if(strcmp(temp, sort_string(search)) == 0)
		{
			//found a match, add it to array of anagrams
			anagrams[i++] = next->value;
			next = next->next;//move on
		}
		else
			next = next->next;//always move on
	}

	return anagrams;
}

char **scrabble(char *search, char letter, int place, arrayOfDict *ray)
{
	//bucket of words = the length of the sesarch word - 1
	word **bucket = ray->dicts[strlen(search)-1]->words;
	int ash = hash(search);//hash of the search word
	//if hash doesn't exist
	if(bucket[ash] == NULL)
		return NULL;

	//shouldn't have more than 52 permutations
	char **results = malloc(52 * sizeof(char *));
	word *next = bucket[ash];
	int i = 0;
	while(next != NULL && next->value != NULL)
	{
		//we already know word has same length, but does it have same chars?
		char *temp;
		int length = strlen(next->value);
		temp = memcpy(malloc(length), sort_string(next->value), length );
		if(strcmp(temp, sort_string(search)) == 0)
		{
			//found a match, but is does it have the character in the
			//right position?
			if(next->value[place] == letter)
			{
				results[i++] = next->value;
				next = next->next;//move on
			}
			else
				next = next->next;
		}
		else
			next = next->next;//always move on
	}

	return results;
}
void test()
{
	/*---HASH TEST---*
	int s = hash("a");
	int a = hash("aabb");
	int b = hash("adacada");
	printf("%d vs %d vs %d",s, a, b);
	---END  TEST---*/

}
