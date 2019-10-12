#include "dcurses.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

//Enter full screen mode
//disable echo and buffered input

struct termios term;

void dcurses_start_fullscreen(){
	
	printf("\033[?1049h");
	tcgetattr(STDIN_FILENO, &term);
    struct termios fullterm = term;
	cfmakeraw(&fullterm);
    fullterm.c_lflag &=~ICANON;
    fullterm.c_lflag &=~ECHO;
	fullterm.c_lflag |=ISIG;
    tcsetattr(STDIN_FILENO, TCSANOW, &fullterm);
}
//return to the previous mode
void dcurses_end_fullscreen(){

	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	printf("\033[?1049l");
}

//clear the screen
void dcurses_clearScreen(){
	printf("\033[2J");
}

//set the current font
void dcurses_setFont(DcursesFont font){
	if (font != DCURSES_FONT_NORMAL &&
		font != DCURSES_FONT_ITALICS &&
		font != DCURSES_FONT_BOLD &&
		font != DCURSES_FONT_FAINT &&
		font != DCURSES_FONT_UNDERLINE &&
		font != DCURSES_FONT_STRIKE){
			printf("~ dCurses font %d is not a valid color.\n", font);
			printf("~ Continuing without font change.\n");
		}
	// change to font
	printf("\033[%dm", font);
	
}

//set the foreground color to 3/4 bit color
void dcurses_fg(DcursesColor color ){
	if (color != DCURSES_COLOR_BLACK &&
		color != DCURSES_COLOR_RED &&
		color != DCURSES_COLOR_GREEN &&
		color != DCURSES_COLOR_YELLOW &&
		color != DCURSES_COLOR_BLUE &&
		color != DCURSES_COLOR_MAGENTA &&
		color != DCURSES_COLOR_CYAN &&
		color != DCURSES_COLOR_WHITE &&
		color != DCURSES_COLOR_BRIGHT){
			// print that color is invalid
			printf("~ dCurses color %d is not a valid color.\n", color);
			printf("~ Continuing without color change.\n");
			return;
		} 
	// change to color
	int change = 30 + color;
	printf("\033[%dm", change);
	
}
//set the background color to 3/4 bit color
void dcurses_bg(DcursesColor color ){
	if (color != DCURSES_COLOR_BLACK &&
		color != DCURSES_COLOR_RED &&
		color != DCURSES_COLOR_GREEN &&
		color != DCURSES_COLOR_YELLOW &&
		color != DCURSES_COLOR_BLUE &&
		color != DCURSES_COLOR_MAGENTA &&
		color != DCURSES_COLOR_CYAN &&
		color != DCURSES_COLOR_WHITE &&
		color != DCURSES_COLOR_BRIGHT){
			// print that color is invalid
			printf("~ dCurses color %d is not a valid color.\n", color);
			printf("~ Continuing without color change.\n");
			return;
		} 
	// change to color
	int change = 40 + color;
	printf("\033[%dm", change);
}
//set the background color to a pallet color 0-255
void dcurses_bg_pallet(int color){
	if (color < 0 || color > 255){
		// print that color is invalid
		printf("~ dCurses color %d is not a valid color.\n", color);
		printf("~ Value must be from 0-255, inclusive.\n");
		printf("~ Continuing without color change.\n");
		return;
	}

	// change to color
	int change = color;
	printf("\033[48;5;%dm", change);
}
//set the foreground color to a pallet color 0-255
void dcurses_fg_pallet(int color){
	if (color < 0 || color > 255){
		// print that color is invalid
		printf("~ dCurses color %d is not a valid color.\n", color);
		printf("~ Value must be from 0-255, inclusive.\n");
		printf("~ Continuing without color change.\n");
		return;
	}

	// change to color
	int change = color;
	printf("\033[38;5;%dm", change);
}

//enable/disable cursor
void dcurses_cursor(int val){
	if (val == 0){
		printf("\033[?25l");
	}else{
		printf("\033[?25h");
	}
	
}

//enable/disable blink
void dcurses_blink(int val){
	if (val == 0){
		printf("\033[25m");
	}else{
		printf("\033[5m");
	}
}

//enable/disable reverse;
void dcurses_reverse(int val){
	if (val == 0){
		printf("\033[27m");
	}else{
		printf("\033[7m");
	}
}

//reset attributes? just fonts and colors i believe
void dcurses_reset(){
	printf("\033[0m");
}

//get the cursor location
void dcurses_location(int* width,int* height){
	
	struct termios term, restore;
	
	tcgetattr(STDIN_FILENO, &restore);
    term = restore;
    term.c_lflag &=~ICANON;
    term.c_lflag &=~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
	
	printf("\033[s");
	printf("\033[6n");

	//scanf("%d,%d",&width,&height);
	scanf("\033[%d;%dR", height, width);
	
	printf("\033[u");
	
	tcsetattr(STDIN_FILENO, TCSADRAIN, &restore);
	
}

//get the screen bounds
void dcurses_bounds(int* width,int* height){
	
	int iWidth, iHeight;
	dcurses_location(&iWidth, &iHeight);
	printf("\033[999;999H");
	
	int rWidth, rHeight;
	dcurses_location(width, height);
	printf("\033[%d;%dH",iHeight,iWidth);
	
}

//move the cursor to the location
void dcurses_moveTo(int x,int y){
	printf("\033[%d;%dH",y,x);
	//printf("\033[%dC", x);
	//printf("\033[%dB", y);	
}

