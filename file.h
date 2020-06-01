#ifndef __FILE_H__
#define __FILE_H__


typedef struct directoryList {
	struct directoryList *nextDir;
	struct Directory *value;	
} directoryList;

typedef struct fileList {
	struct fileList *nextFile;
	struct File *value;
} fileList;

typedef struct Directory {
    // The name of the directory
    char *name;

    // TODO: The list of files of the current directory
    struct fileList *firstFile;
    // TODO: The list of directories of the current directory
    struct directoryList *firstDir;
    // The parent directory of the current directory (NULL for the root directory)
    struct Directory *parentDir;
} Directory;

typedef struct File {
    // The name of the file
    char *name;

    // The size of the file
    int size;

    // The content of the file
    char *data;

    // The directory in which the file is located
    Directory *dir;
    //struct File *nextFile;
} File;

Directory *createFS();
void deleteFS(Directory *root);
void mkdir(Directory *parent, char *dirName);
void ls(Directory *currDir);
void insertDir(Directory *currDir);
directoryList *containsDir(Directory *currDir, char *dirName);
int containsFile(Directory *currDir, char *fileName);
void pwd(Directory *currDir, Directory *root);
void cd(Directory **curDir, char *dirName);
void touch(Directory *currDir, char *fileName, char *content);
void rm(char *fileToRm, Directory *currDir);
void tree(Directory *currDir, int depth);
void rmDir(char *dirNameToRemove, Directory *currDir);
void find(Directory *currDir, int max_depth, int min_size, int max_size, char *content, int depth);

#endif /* __FILE_H__ */
