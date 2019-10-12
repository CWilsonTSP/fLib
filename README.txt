Last Edited: 25 Mar. 2019

Authors: J. Dyamacek (Makefile, flib.h)
		 C. Wilson (implementation of flib.c)

The betterls function will list the files in the directory it is
run in. The program can be run with the flags "-cdsrh -P x -S x"
where in:
	c - colour
	d - sorted by date
	s - sorted by size
	r - directory size
	h - show hidden files
	P - prefix, where x is replaced by a string
	S - suffix, where x is replaced by a string
	
Things of Note:
	~ Added a function fl_list_filtered_i. This will list hidden
	files while using whatever flags are sent
