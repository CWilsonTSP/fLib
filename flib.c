#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

struct fileData {
	char* string;
	unsigned int size;
	unsigned int date;
};

// use in filtered list
int sprefix(const char* prefix, const char* str){
	if (strncmp(prefix, str, strlen(prefix)) == 0)
		return 1; // matched, return true
	return 0; // no match, return false
}
int ssuffix(const char* suffix, const char* str) {
	
	// if a string is null
	if( str == NULL || suffix == NULL )
		return 0;

	int str_len = strlen(str);
	int suffix_len = strlen(suffix);

	if(suffix_len > str_len)
		return 0;

	return (0 == strncmp( str + str_len - suffix_len, suffix, suffix_len ));
}

//count the number of file names in files
int fl_count(char** files){
	int rv = 0;
	while (files[rv] != 0){
		++rv;
	}return rv;
}
//free all of the memory associated with files
void fl_delete(char** files){
	for (int i = 0; i < fl_count(files); ++i){
		free(files[i]);
	}free(files);
}


//return a pointer to an array of c strings, the last element should be zero
// this will list the name of each file in the path

char** fl_list(const char* path){
	// open direction
	DIR* d = opendir(path);
	struct dirent* dir;

	// loop through to find count
	int count = 0;
	while((dir = readdir(d)) != NULL){	
		++count;	
	}rewinddir(d); // back up to start


	// declare rv to return 
	char** rv = (char**) malloc(sizeof(char*) * count+1);
	count = 0;
	// loop through to malloc memory to rv
	while((dir = readdir(d)) != NULL){
		rv[count] = (char*) malloc(sizeof(char*) *  strlen(dir->d_name));
		++count;
	}rewinddir(d); // back up again
	count = 0;

	// loop through to add strings to rv
	while((dir = readdir(d)) != NULL){
		strcpy(rv[count], dir->d_name);
		++count;
	}rv[count] = 0; // make last one a null terminator

	closedir(d); // close directory
	return rv; // return

}
//same as fl_list except the files returned must start with prefix
// and end with suffix
char** fl_list_filter(const char* path, const char* prefix, const char* suffix){
	// open direction
	DIR* d = opendir(path);
	struct dirent* dir;

	// loop through to find count
	int count = 0;
	while((dir = readdir(d)) != NULL){	
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1 && dir->d_name[0] != '.')
			++count;	
	}rewinddir(d); // back up to start


	// declare rv to return 
	char** rv = (char**) malloc(sizeof(char*) * count+1);
	count = 0;
	// loop through to malloc memory to rv
	while((dir = readdir(d)) != NULL){
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1 && dir->d_name[0] != '.'){
			rv[count] = (char*) malloc(sizeof(char*) *  strlen(dir->d_name));
			++count;
		}
	}rewinddir(d); // back up again
	count = 0;
	// loop through to add strings to rv
	while((dir = readdir(d)) != NULL){
		// check prefix and suffix, then add if both are true
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1 && dir->d_name[0] != '.'){
			strcpy(rv[count], dir->d_name);
			++count;
		}
	}rv[count] = 0; // make last one a null terminator



	closedir(d); // close directory
	return rv; // return


}

char** fl_list_filter_i(const char* path, const char* prefix, const char* suffix){
	// open direction
	DIR* d = opendir(path);
	struct dirent* dir;

	// loop through to find count
	int count = 0;
	while((dir = readdir(d)) != NULL){	
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1)
			++count;	
	}rewinddir(d); // back up to start


	// declare rv to return 
	char** rv = (char**) malloc(sizeof(char*) * count+1);
	count = 0;
	// loop through to malloc memory to rv
	while((dir = readdir(d)) != NULL){
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1){
			rv[count] = (char*) malloc(sizeof(char*) *  strlen(dir->d_name));
			++count;
		}
	}rewinddir(d); // back up again
	count = 0;
	// loop through to add strings to rv
	while((dir = readdir(d)) != NULL){
		// check prefix and suffix, then add if both are true
		if (sprefix(prefix, dir->d_name) == 1 && ssuffix(suffix, dir->d_name) == 1){
			strcpy(rv[count], dir->d_name);
			++count;
		}
	}rv[count] = 0; // make last one a null terminator

	printf("-- USED _i!\n");

	closedir(d); // close directory
	return rv; // return


}
//is the file a directory
int fl_isDir(const char* name){ //
	int rv;
	struct stat sb;	

	rv = stat(name, &sb);
	if(rv == -1){
		return 0;
	}rv = S_ISDIR(sb.st_mode);
	return rv;
}
//is the file an executable
int fl_isExe(const char* name){
	int rv;
	struct stat sb;	

	rv = stat(name, &sb);
	if(rv == -1){
		return 0;
	}if (rv == 0 && sb.st_mode &S_IXUSR){
		rv = 1;
	}return rv;
}
//is the file a symbolic link
int fl_isSym(const char* name){
	int rv;
	struct stat sb;	

	rv = stat(name, &sb);
	if(rv == -1){
		return 0;
	}rv = S_ISLNK(sb.st_mode);
	return rv;
}
//what is the file's size in bytes
unsigned int fl_size(const char* name){
	int rv;
	struct stat sb;	
	rv = stat(name, &sb);
	rv = sb.st_size;	
	return rv;
}
//same as fl_size except if the file is a directory then it recursively finds
//	the size of directory and all files it contains
unsigned int fl_size_r(const char* name){
	int rv = 0;
	int size;	

	if (fl_isDir(name) == 1){
		char** newlist = fl_list(name);

		for (int i = 0; i < fl_count(newlist); ++i){
//			printf("%s/%s\n",name,newlist[i]);
			if (strcmp("..", newlist[i]) && strcmp(".", newlist[i])){
				char thisisstupid[(strlen(newlist[i]) + strlen(name))];		//(char*)malloc(sizeof(char) * (strlen(newlist[i]) + strlen(name) + 1));
				strcpy(thisisstupid, name);
				strcat(thisisstupid, "/");
				strcat(thisisstupid, newlist[i]);
				//printf("%s\n",thisisstupid);
				rv += fl_size_r(thisisstupid);
				//free(thisisstupid);
			}
		}

	}
	struct stat sb;
	size = stat(name, &sb);
	size = sb.st_size;
	

//	printf("Running Size: %d\n",  rv + size);
	return rv + size;

}

//the last modification time of the file
unsigned int fl_date(const char* name){
	unsigned int rv;

	struct stat sb;
	rv = stat(name, &sb);
	rv = sb.st_mtime;
	return rv;

}

//sort the files descending by fl_size
void fl_sort_bySize(char** files){
	for (int i = 0; i < fl_count(files); ++i){
		for (int j = 0; j < fl_count(files); ++j){
			struct fileData a;
			struct fileData b;

			a.string = files[i];
			a.size = fl_size(files[i]);	
			b.string = files[j];
			b.size = fl_size(files[j]);

			if (a.size < b.size){
				struct fileData c = a;
				
				files[i] = b.string;
				files[j] = c.string;
			}
		}
	}
}
//sort the files descending by fl_date
void fl_sort_byDate(char** files){
	for (int i = 0; i < fl_count(files); ++i){
		for (int j = 0; j < fl_count(files); ++j){
			struct fileData a;
			struct fileData b;

			a.string = files[i];
			a.date = fl_date(files[i]);	
			b.string = files[j];
			b.date = fl_date(files[j]);

			if (a.date < b.date){
				struct fileData c = a;

				
				files[i] = b.string;
				files[j] = c.string;
			}
		}
	}

}

//format the date and store in str
//Mar 09 13:49
void fl_formatDate(const char* filename, char* str){
	unsigned int time = fl_date(filename); // get date

	char mtime[80]; // this will be copied into str. Shouldn't REALLY ever be 80 characters but 'eh'
    time_t t = (time_t)time; /*st_mtime is type time_t */
    struct tm lt; // make the time struct to call localtime
    localtime_r(&t, &lt); // convert to struct tm 
    strftime(mtime, sizeof mtime, "%d %b %H:%M", &lt); //  format the time into a readable string
    //printf("%s\n", mtime);
	
	strcpy(str, mtime);
}

//xxx.xB where the last can be B, K, M, G 
void fl_readableSize(unsigned int size,char* str){

	// char* array to grab suffix for size
	char *suffix[] = {"B", "K", "M", "G"};
	char length = sizeof(suffix) / sizeof(suffix[0]);

	int i = 0; // declare outside so you can use suffix[i] 
	double dblBytes = size;

	if (size > 1024) {
		for (i = 0; (size / 1024) > 0 && i<length-1; i++, size /= 1024)
			dblBytes = size / 1024.0;
	}static char output[80];								// create output string to copy to. Shouldn't REALLY ever be 80 characters, but 'eh'
	sprintf(output, "%6.01lf%s", dblBytes, suffix[i]);	// format output. % starts the string, The first thing to place in
														// .01 designates 1 decimal place, l means to use integer
														// f means make it a decimal and round, %s is the suffix
	strcpy(str,output);

}

