#include "flib.h"
#include "dcurses/dcurses.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void printtest(char** filelist, int rsize, int color){

	if (fl_count(filelist) == 0){
		printf("There are no files in the current directory.\n");
		return;
	}	
	printf("SIZE\t\tLAST MODIFIED\t\tTYPE\t\tNAME\n");
	printf("====================================================================\n");
	for(int i = 0; i < fl_count(filelist); ++i){
		// viariable for file type -- isDir isExe isSym
		// 0 for reg, 1 for dir, 2 for exe, 3 for sym
		char* x = "";
		int a = fl_isDir(filelist[i]);
		int b = -1;
		if (a == 1){
			x = "[DIR]";
			b = 0;
		}else if (a != 1){
			a = fl_isExe(filelist[i]);
			if (a == 1){
				x = "[EXE]";
				b = 1;
			}
		}else if (a != 1){
			a = fl_isSym(filelist[i]);
			if (a == 1){
				x = "[SYM]";
				b = 2;
			}
		}
		
		char strDate[80];
		fl_formatDate(filelist[i], strDate);

		char strSize[80];
		if (rsize == 1)
			fl_readableSize(fl_size_r(filelist[i]), strSize);
		else
			fl_readableSize(fl_size(filelist[i]), strSize);
		dcurses_reset();
		printf("%s\t\t%s\t\t%s\t\t",strSize,strDate,x);
		
		if (b == 0 && color == 1)
			dcurses_fg_pallet(12);
		else if (b == 1 && color == 1)
			dcurses_fg_pallet(10);
		printf("%s\n",filelist[i]);
		dcurses_reset();
	}
	printf("\n");
}

int main(int argc, char* argv[]){

	// sort by date, size, name
	// recursive size?
	// prefix suffix
	// colour?

	int date = 0;
	int name = 1;
	int size = 0;
	int rsize = 0;
	int color = 1;
	int hidden = 0;
	char prefix[256];
	char suffix[256];

	strcpy(prefix,"");
	strcpy(suffix,"");

	int opt;

	while((opt = getopt(argc, argv, "dsrchP:S:")) != -1)  {  
			switch(opt){  
		        case 'd':
					date = 1;
					size = 0;
					break;  
			    case 's':
					size = 1;
					date = 0;  
					break;
				case 'r':  
					rsize = 1;
					break;  
		        case 'c':  
				    color = 0;
					break;  
				case 'n':
					name = 0;
					break;
				case 'P':
					strcpy(prefix, optarg);
					break;
				case 'S':
					strcpy(suffix, optarg);
					break;
				case 'h':
//					strcpy(prefix,"");
//					strcpy(suffix,"");
					hidden = 1;
				case '?':
					break;
				default:  
		            break;  
			}  
	}
   
	//char** filelist = fl_list(".");
 
	if (hidden == 0){
		char** filelist = fl_list_filter(".", prefix, suffix);
	
		if (date == 1){
			fl_sort_byDate(filelist);
		}if (size == 1){
			fl_sort_bySize(filelist);
		}
		
		printtest(filelist, rsize, color);

		fl_delete(filelist);

	}else{
		char** filelist = fl_list_filter_i(".", prefix, suffix);

		if (date == 1){
			fl_sort_byDate(filelist);
		}if (size == 1){
			fl_sort_bySize(filelist);
		}
		
		printtest(filelist, rsize, color);

		fl_delete(filelist);

	}
	
	return 0;

}
