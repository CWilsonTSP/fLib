#ifndef FLIB__H
#define FLIB__H
#include <stdlib.h>
#include <stdio.h>

//return a pointer to an array of c strings, the last element should be zero
// this will list the name of each file in the path
char** fl_list(const char* path);
//same as fl_list except the files returned must start with prefix
// and end with suffix
char** fl_list_filter(const char* path, const char* prefix, const char* suffix);
char** fl_list_filter_i(const char* path, const char* prefix, const char* suffix);

//count the number of file names in files
int fl_count(char** files);
//is the file a directory
int fl_isDir(const char* name);
//is the file an executable
int fl_isExe(const char* name);
//is the file a symbolic link
int fl_isSym(const char* name);
//what is the file's size in bytes
unsigned int fl_size(const char* name);
//same as fl_size except if the file is a directory then it recursively finds
//	the size of directory and all files it contains
unsigned int fl_size_r(const char* name);

//the last modification time of the file
unsigned int fl_date(const char* name);

//sort the files descending by fl_size
void fl_sort_bySize(char** files);
//sort the files descending by fl_date
void fl_sort_byDate(char** files);

//free all of the memory associated with files
void fl_delete(char** files);

//format the date and store in str
//Mar 09 13:49
void fl_formatDate(const char* filename, char* str);

//XXX.XB where the last can be B, K, M, G 
void fl_readableSize(unsigned int size,char* str);

#endif
