/*
 ============================================================================
 Name        : problem1.c
 Author      : Tuan Nguyen
 Description : CS283 - Homework 4, search and append string to files' content
 ============================================================================
 */

#include "csapp.c"
#include <dirent.h>
#include <string.h>


void helper(char *nameOfFile, int sizeOfFile, char *pattern, char *cached, int mode) {
	size_t patternLen = strlen(pattern);
	int fd = Open(nameOfFile, O_RDONLY, 0);
	char *updatedBuffer, *oldBuffer;
	updatedBuffer = Malloc((patternLen + sizeOfFile) * sizeof(char));
	oldBuffer = Malloc(sizeOfFile * sizeof(char));
	int i;
	while((i = Rio_readn(fd, oldBuffer, sizeOfFile)) != 0) {
		strcat(updatedBuffer, oldBuffer);
	}
	char *lastPosition = strstr(updatedBuffer, cached);
	i = 0;
	while(i < sizeOfFile && &updatedBuffer[i] != lastPosition) {
		i++;
	}
	memmove(updatedBuffer + i + patternLen, lastPosition, sizeOfFile - (i + 2));
	i = 0;
	while(i < patternLen) {
		lastPosition[i] = pattern[i++];
	}
	Rio_writen(Open(nameOfFile, O_TRUNC|O_WRONLY, mode), updatedBuffer, sizeOfFile);
	free(updatedBuffer);
	free(oldBuffer);
}


int main(int argc, char **argv) {
	if(argc != 3 && argc != 4) {
		printf("Invalid number of arguments\n");
		exit(0);
	}
	DIR *directory;
	int optionFlag = argc == 4 ? 0 : 1;
	size_t patternLen, travelSoFar;
	struct dirent *entry;
	char *patternString, *lastPosition, *currentBuffer, *temp;
	patternString = Malloc(travelSoFar *sizeof(char));
	lastPosition = Malloc(patternLen *sizeof(char));
	currentBuffer = Malloc(MAXLINE * sizeof(char));
	strcpy(patternString, argv[1]);
	strcpy(lastPosition, argv[2]);
	// validate current directory
	if( (directory = opendir(".")) == NULL) {
		printf("Invalid directory\n");
		exit(0);
	}
	printf("Start searching files that match pattern in the directory\n");
	while((entry = readdir(directory)) != NULL) {
		struct stat currentFile;
		Stat(entry->d_name, &currentFile);
		// Skip '.' and '..'
		if(!S_ISDIR(currentFile.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			if(optionFlag || (temp = strstr(entry->d_name, argv[3])) != NULL) {
				printf("Looking into the content of %s to insert '%s' before '%s'\n", entry->d_name, patternString, lastPosition);
				char *appendPointer = NULL;
				char *patternPointer = NULL;
				struct stat currentFIle;
				rio_t rio;
				int fd, n = 0;
				fd = Open(entry->d_name, O_RDONLY, 0);
				Rio_readinitb(&rio, fd);
				Stat(entry->d_name, &currentFIle);
				size_t sizeOfFile = currentFIle.st_size;
				char buf[sizeOfFile];
				while((n = Rio_readlineb(&rio, buf, sizeOfFile)) != 0) {
					char *a = NULL, *b = NULL;
					if((a = strstr(buf, lastPosition)) != NULL) {
						appendPointer = a;
						a = NULL;
					} if((b = strstr(buf, patternString)) != NULL) {
						patternPointer = b;
						b = NULL;
					}
				}
				if(patternPointer != NULL) {
					printf("Pattern already exists in %s\n", entry->d_name);
				} else if(appendPointer != NULL) {
					printf("No pattern exists and an insertion point is found\n");
					helper(entry->d_name, sizeOfFile, patternString, lastPosition, currentFIle.st_mode);
				}
			}
		}
	}
	free(currentBuffer);
	free(patternString);
	free(lastPosition);
}
