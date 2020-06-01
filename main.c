#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "file.h"

int main()
{
	Directory *root, *currDir;
	char buffer[100], command[10], param1[20], param2[20], param3[10], param4[10];
	int index;
	while(1)
	{
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer) - 1] = 0;
		if(!strcmp(buffer, "delete fs")) break;
		index = 0;
		char *word = strtok(buffer," ");
		while(word)
		{
			if(index == 0) strcpy(command, word);
			if(index == 1) strcpy(param1, word);
			if(index == 2) strcpy(param2, word);
			if(index == 3) strcpy(param3, word);
			if(index == 4) strcpy(param4, word);
			index++;
			word = strtok(NULL, " ");
		}
		if(!strcmp(command, "create")) 
		{
			root = createFS();
			currDir = root;
		} 
		if(!strcmp(command, "touch")) touch(currDir, param1, param2);
		if(!strcmp(command, "ls")) ls(currDir); 
		if(!strcmp(command, "mkdir")) mkdir(currDir, param1);
		if(!strcmp(command, "pwd")) 
		{
			if(currDir == root) {printf("/\n"); continue;} 
			pwd(currDir, root); 
			printf("\n");
		}
		if(!strcmp(command, "cd")) cd(&currDir, param1);
		if(!strcmp(command, "tree")) 
			tree(currDir, 0);
		if(!strcmp(command, "rm")) rm(param1, currDir);
		if(!strcmp(command, "rmdir")) rmDir(param1, currDir);
		if(!strcmp(command, "find")) 
		{
			find(currDir, atoi(param1), atoi(param2), atoi(param3), param4, 0);
			printf("\n");
		}
	}
	deleteFS(root);
}