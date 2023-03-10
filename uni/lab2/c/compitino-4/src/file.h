#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define BUFLEN 	1024

#define CHECK_ERROR(res, v, msg) if (res == v) {perror(msg); exit(EXIT_FAILURE);} 
#define CHECK_FOPEN(res) CHECK_ERROR(res, NULL, "Error fopen")
#define CHECK_OPEND(res) CHECK_ERROR(res, NULL, "Error opendir")
#define CHECK_CLOSED(res) CHECK_ERROR(res, -1, "Error closedir")


#define IS_DIR(file) (file->d_type == DT_DIR)
#define IS_FILE(file) (file->d_type == DT_REG)

#define CONCAT_PATH(path, name, newpath){\
	newpath = realloc(newpath, (strlen(path) + strlen(name) + 4) * sizeof(char));\
	CHECK_ERROR(newpath, NULL, "Not enough memory")\
	strcpy(newpath, path);\
	strcat(newpath, "/");\
	strcat(newpath, name);\
}

#define NOT_THIS_DIR(name) (strcmp(name, ".") != 0)
#define NOT_PREV_DIR(name) (strcmp(name, "..") != 0)


#define CHECK_GETLINE(res)\
	if (res == -1 && !feof(f)) {perror("Error getline"); exit(EXIT_FAILURE);}
