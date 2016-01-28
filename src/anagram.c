#include <stdio.h>
#include <stdlib.h>

#include "dic.c"

int main(int argc, char *argv[])
{

	//---- LOADING THE WORDS FROM THE DICTIONARY INTO STORAGE ----//

	printf("Loading words from dictionary . . . \n");
	//create an array of dictionaries
	int raySize = 45;
	dictArray *myGrams = NULL;
	myGrams = createDictionary(raySize);
	myGrams->size = raySize;

	//open the dictionary for read-only access
	FILE* dict = fopen("words", "r");
	//FILE* dict = fopen("the2k", "r");

	if(dict == NULL)
		return 0;

	// Read each line of the file, and print it to screen
	char word[46];
	int i = 0;
	while(fgets(word, sizeof(word), dict) != NULL)
	{
		size_t length = strlen(word) - 1;
		if(word[length] == '\n')
    		word[length] = '\0';
		if(word[length-2] != '\'')//apostrophes just create duplicates
			//we don't need duplicates
			insert_entry(word, myGrams);
		i++;
	}
	//---- END DATA RETRIEVAL ----//
	printf(" Done. \n");
	printf("Max list size: %d \n", myGrams->maxSize);

	if(argc == 1)
	{
		int signal = 0;
		do
		{
			printf("\nType in a word to search for anagrams of it or 0 to quit: \n");
			char search[45];//assume the input is meaningful
			scanf("%s", search);
			//ALLOW USER TO SEARCH FOR ANAGRAMS OF A WORD
			if(search[0] == '0')
				signal = -1;
			else
			{
				char **chars = get_word(search, myGrams);
				for(i = 0; chars[i] != NULL; i++)
					printf("Anagrams for %s: %s\n", search, chars[i]);
			}
		} while(signal == 0);
	}
	else
	{
		if(argc == 2)//default run
		{
			char **chars = get_word(argv[1], myGrams);
			for(i = 0; chars[i] != NULL; i++)
				printf("Anagrams for %s: %s\n", argv[1], chars[i]);
		}
		else//scrabble run
		{
			char letter = argv[2][0];
			int place = atoi(argv[3]);
			//format has scrabble(lettersAvailable, restriction, place)
			char **chars = scrabble(argv[1], letter, place, myGrams);
			for(i = 0; chars[i] != NULL; i++)
				printf("Scrabble results for %s: %s\n", argv[1], chars[i]);
		}
	}


	//GIVING MEMORY BACK TO SYSTEM
	for(i = 0; i < myGrams->size; i++)
	{
		if(myGrams->dicts[i] != NULL)
			deleteDict(myGrams->dicts[i]);
	}
	free(myGrams->dicts);
	free(myGrams);
}
