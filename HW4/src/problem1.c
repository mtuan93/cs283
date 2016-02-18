/*
 ============================================================================
 Name        : problem1.c
 Author      : Tuan Nguyen
 Description : CS283 - Homework 4, replacing matching files' names with new names
 ============================================================================
 */

#include "csapp.c"
#include <dirent.h>
#include <string.h>


int main(int argc, char **argv) {
	if(argc != 3 && argc != 4) {
		printf("Invalid numbers of arguments!");
		exit(0);
	}
	DIR *directory;
	char *patternString, *replaceString, *currentFileNameChar, *temp;
	struct dirent *entry;
	int optionFlag = argc == 4 ? 0 : 1;
	// Initialize
	int patternLen = strlen(argv[1]);
	int replaceLen = strlen(argv[2]);
	patternString = Malloc(patternLen *sizeof(char));
	replaceString = Malloc(replaceLen *sizeof(char));
	strcpy(patternString, argv[1]);
	strcpy(replaceString, argv[2]);
	// Check if the current directory is valid
	if((directory = opendir(".")) == NULL) {
		printf("Invalid directory!");
		exit(0);
	}
	printf("Start searching files that match pattern in the directory\n");
	while(entry = readdir(directory)) {
		struct stat currentFile;
		Stat(entry->d_name, &currentFile);
		// Skip '.' and '..'
		if(!S_ISDIR(currentFile.st_mode) &&
			strcmp(entry->d_name, ".") != 0 &&
			strcmp(entry->d_name, "..") != 0 &&
			(currentFileNameChar = strstr(entry->d_name, patternString)) != NULL) {
			if(optionFlag || (temp = strstr(entry->d_name, argv[3])) != NULL) {
				printf("Found file %s with pattern '%s'\n", entry->d_name, patternString);
				int newNameLen = replaceLen - patternLen + strlen(entry->d_name);
				char *updatedName = Malloc(replaceLen + newNameLen*sizeof(char)+1);
				strcpy(updatedName, entry->d_name);
				int newNamePtr = 0, patternPtr = 0, oldNamePtr = 0;
				while(&entry->d_name[oldNamePtr] != currentFileNameChar && newNamePtr < newNameLen) {
					updatedName[newNamePtr++] = entry->d_name[oldNamePtr++];
				}
				while(patternPtr < replaceLen) {
					updatedName[newNamePtr++] = replaceString[patternPtr++];
				}
				oldNamePtr += patternLen;
				while(newNamePtr < newNameLen) {
					updatedName[newNamePtr++] = entry->d_name[oldNamePtr++];
				}
				updatedName[newNameLen] = '\0';
				rename(entry->d_name, updatedName);
				printf("Successfully replace file name %s with new name %s\n", entry->d_name, updatedName);
				free(updatedName);
			}
		}
	}
	free(patternString);
	free(replaceString);
}
