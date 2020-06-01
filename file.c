#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"

Directory *createFS()
{
	Directory *root = (Directory*)malloc(sizeof(Directory));
	root->parentDir = NULL;
	root->name = malloc(2 * sizeof(char));
	root->firstDir = NULL;
	root->firstFile = NULL;
	strcpy(root->name, "/");
	return root;
}

void deleteFS(Directory *root)
{
	free(root->name);
	free(root);
}

void mkdir(Directory *parent, char *dirName)
{
	directoryList *temp = (directoryList*)malloc(sizeof(directoryList));
	temp->value = (Directory*)malloc(sizeof(Directory));
	temp->value->parentDir = parent;
	//dirName[strlen(dirName)] = 0;
	temp->value->name = (char*)malloc((strlen(dirName) + 1)  * sizeof(char));
	strcpy(temp->value->name, dirName);
	//temp->value->name = strdup(dirName);
	temp->value->firstDir = NULL;
	temp->value->firstFile = NULL;
	if(!parent->firstDir)
	{
		parent->firstDir = temp;
		temp->nextDir = NULL;
		return;
	}
	if(strcmp(temp->value->name, parent->firstDir->value->name) < 0)
	{
		directoryList *aux = parent->firstDir;
		parent->firstDir = temp;
		temp->nextDir = aux;
		return;
	}
	directoryList *prev, *aux = parent->firstDir;
	while(aux && strcmp(temp->value->name, aux->value->name) > 0)
	{
		prev = aux;
		aux = aux->nextDir;
	}
	if(!prev->nextDir)
	{
		prev->nextDir = temp;
		temp->nextDir = NULL;
		return;
	}
	prev->nextDir = temp;
	temp->nextDir = aux;
}

directoryList *containsDir(Directory *currDir, char *dirName)
{
	directoryList *tempDir = currDir->firstDir;
	while(tempDir)
	{
		if(!strcmp(tempDir->value->name, dirName)) return tempDir;
		tempDir = tempDir->nextDir;
	}
	return NULL;	
}

int containsFile(Directory *currDir, char *fileName)
{
	fileList *tempFile = currDir->firstFile;
	while(tempFile)
	{
		if(!strcmp(tempFile->value->name, fileName)) return 1;
		tempFile = tempFile->nextFile;
	}
	return 0;
}

void touch(Directory *parent, char *fileName, char *content)
{
	fileList *temp = (fileList*)malloc(sizeof(fileList));
	temp->value = (File*)malloc(sizeof(File));
	temp->value->dir = parent;
	//temp->value->name = strdup(fileName);
	temp->value->name = malloc((strlen(fileName) + 1) * sizeof(char));
	strcpy(temp->value->name, fileName);
	//temp->value->data = strdup(content);
	temp->value->data = malloc((strlen(content) + 1) * sizeof(char));
	strcpy(temp->value->data, content);
	temp->value->size = strlen(content);
	if(!parent->firstFile)
	{
		parent->firstFile = temp;
		temp->nextFile = NULL;
		return;
	}
	if(strcmp(temp->value->name, parent->firstFile->value->name) < 0)
	{
		fileList *aux = parent->firstFile;
		parent->firstFile = temp;
		temp->nextFile = aux;
		return;
	}
	fileList *prev, *aux = parent->firstFile;
	while(aux && strcmp(temp->value->name, aux->value->name) > 0)
	{
		prev = aux;
		aux = aux->nextFile;
	}
	if(!prev->nextFile)
	{
		prev->nextFile = temp;
		temp->nextFile = NULL;
		return;
	}
	prev->nextFile = temp;
	temp->nextFile = aux;
}

void ls(Directory *currDir)
{
	directoryList *tempDir = currDir->firstDir;
	fileList *tempFile = currDir->firstFile;
	if(!tempDir && !tempFile) 
	{
		printf("\n");
		return;
	}
	while(tempFile)
	{
		printf("%s ", tempFile->value->name);
		tempFile = tempFile->nextFile;
	}
	while(tempDir)
	{
		printf("%s ", tempDir->value->name);
		tempDir = tempDir->nextDir;
	}
	printf("\n");
}

void pwd(Directory *currDir, Directory *root)
{
	if(currDir == root) return;
	pwd(currDir->parentDir, root);
	printf("/%s", currDir->name);
}
void cd(Directory **currDir, char *dirName)
{
	if(!strcmp(dirName, "..")) 
	{
		*currDir = (*currDir)->parentDir;
		return;
	}
	if(containsDir(*currDir, dirName)) 
	{
		directoryList *aux = containsDir(*currDir, dirName);
		*currDir = aux->value;
	}
	else printf("Cannot move to '%s': No such directory!\n", dirName);
}

void rm(char *fileToRm, Directory *currDir)
{
	if(containsFile(currDir, fileToRm))
	{
		fileList *temp = currDir->firstFile;
		if(!strcmp(currDir->firstFile->value->name, fileToRm))
		{
			currDir->firstFile = temp->nextFile;
			free(temp->value->name);
			free(temp->value->data);
			free(temp->value);
			free(temp);
			return;
		}
		fileList *prev;
		while(strcmp(temp->value->name, fileToRm) && temp->nextFile)
		{
			prev = temp;
			temp = temp->nextFile;
		}
		if(!temp->nextFile && !strcmp(temp->value->name, fileToRm))
		{
			prev->nextFile = NULL;
			free(temp->value->data);
			free(temp->value->name);
			free(temp->value);
			free(temp);
			return;	
		}
		prev->nextFile = temp->nextFile;
		free(temp->value->data);
		free(temp->value->name);
		free(temp->value);
		free(temp);
	}
	else 
		printf("Cannot remove '%s': No such file!\n", fileToRm);
}

void tree(Directory *currDir, int depth)
{
	int nSpaces = depth * 4;
	for(int i = 1; i <= nSpaces; i++)
		printf(" ");
	printf("%s\n", currDir->name);
	depth++;
	nSpaces = depth * 4;
	fileList *tempFile = currDir->firstFile;
	while(tempFile)
	{
		for(int i = 1; i <= nSpaces; i++)
			printf(" ");
		printf("%s\n", tempFile->value->name);
		tempFile = tempFile->nextFile;
	}
	if(!currDir->firstDir) return;
	directoryList *tempDir = currDir->firstDir;
	while(tempDir)
	{
		tree(tempDir->value, depth);
		tempDir = tempDir->nextDir;
	}
}

void rmDir(char *dirNameToRemove, Directory *currDir)
{
	directoryList *auxParent = containsDir(currDir, dirNameToRemove);
	if(!auxParent)
	{
		printf("Cannot remove '%s': No such directory!\n", dirNameToRemove);
		return;
	}
	directoryList *tempDir = auxParent->value->firstDir;
	directoryList *dirToRemove = auxParent;
	directoryList *prev = dirToRemove->value->parentDir->firstDir;
	while(dirToRemove->value->firstDir)
	{
		while(tempDir->value->firstDir)
		{
			auxParent = tempDir;
			tempDir = tempDir->value->firstDir;
		}
	fileList *tempFile = tempDir->value->firstFile;
	while(tempFile)
	{
		fileList *auxFile = tempFile;
		tempFile = tempFile->nextFile;
		tempDir->value->firstFile = tempFile;
		free(auxFile->value->name);
		free(auxFile->value->data);
		free(auxFile->value);
		free(auxFile);
	}
	auxParent->value->firstDir = tempDir->nextDir;
	free(tempDir->value->name);
	free(tempDir->value);
	free(tempDir);
	auxParent = dirToRemove;
	tempDir = auxParent->value->firstDir;
	}
	fileList *tempFile = dirToRemove->value->firstFile;
	while(tempFile)
	{
		fileList *auxFile = tempFile;
		tempFile = tempFile->nextFile;
		dirToRemove->value->firstFile = tempFile;
		free(auxFile->value->name);
		free(auxFile->value->data);
		free(auxFile->value);
		free(auxFile);
	}
	if(dirToRemove == prev)
	{
		dirToRemove->value->parentDir->firstDir = dirToRemove->nextDir;
		free(dirToRemove->value->name);
		free(dirToRemove->value);
		free(dirToRemove);
		return;
	}
	while(prev->nextDir != dirToRemove)
		prev = prev->nextDir;
	prev->nextDir = dirToRemove->nextDir;
	free(dirToRemove->value->name);
	free(dirToRemove->value);
	free(dirToRemove);
}

void find(Directory *currDir, int max_depth, int min_size,
		int max_size, char *content, int depth)
{
	if(depth > max_depth) return;
	fileList *tempFile = currDir->firstFile;
	while(tempFile)
	{
		if(strstr(tempFile->value->data, content) && tempFile->value->size >= min_size
			&& tempFile->value->size <= max_size) 
			printf("%s ", tempFile->value->name);
		tempFile = tempFile->nextFile;
	}
	if(!currDir->firstDir) return;
	directoryList *tempDir = currDir->firstDir;
	while(tempDir)
	{
		find(tempDir->value, max_depth, min_size, max_size, content, depth + 1);
		tempDir = tempDir->nextDir;
	}
}
