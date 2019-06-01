

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

struct gameboard{
	int maxrow;
	int maxcol;
	char** gametable;
};

struct snake{
	int startposcol;
	int startposrow;
	int directionrow;
	int directioncol;
	int posrow;
	int poscol;
	char symbol;
};

struct score{
	int scoreplayer1;
	int scoreplayer2;
};


/*
 *
 *  Game
 *  Display
 *  Snake
 *  KeyInput
 *
 */




// GameBoard
void initialize(gameboard* gb){

	int row = 0;
	int col = 0;

	initscr();
	timeout(0);
	noecho();     // This does not echo input to the screen
	keypad(stdscr, TRUE);

	srand(time(NULL));

	getmaxyx(stdscr, row, col);
	gb->maxrow = row;
	gb->maxcol = col;

	gb->gametable  = new char*[row];
	for (int j = 0; j<col; j++){
		(gb->gametable)[j] = new char[col];
	}
}

// Gameboard
void makeboard(gameboard* gb){
	
	for (int row = 0; row < gb->maxrow; row++){
		for (int col = 0; col < gb->maxcol; col++){
			gb->gametable[row][col] = ' '; // Empty creates the blank canvas
		}
	}
	
	for (int i = 0; i<gb->maxrow; i++){
				
		gb->gametable[i][0] = 178; // Right Side Border
		gb->gametable[i][gb->maxcol - 1] = 178; // Left Side Border
	}


	for (int i = 0; i<gb->maxcol; i++){
				
		gb->gametable[0][i] = 178; // Top Border
		gb->gametable[gb->maxrow -1][i] = 178; // Bottom Border

	}

}

// Gameboard
void cleanup(gameboard* gb){


	for (int j = 0; j<gb->maxcol; j++){
		delete((gb->gametable)[j]);
	}
	delete(*((gb)->gametable));

}



// Gameboard
void paint(gameboard* gb){

	
	for (int row = 0; row < gb->maxrow; row++){
		for (int col = 0; col < gb->maxcol; col++){
			move(row, col);
			addch(gb->gametable[row][col]);
		}
	}
	refresh();

}


// Snake
void generatestartdirection(int* directionrow, int* directioncol){

	int r = rand() %4;

	if(r == 0){
		*directionrow = 1;
		*directioncol = 0;
	}

	else if(r == 1){
		*directionrow = 0;
		*directioncol = 1;
	}
	    
	else if(r == 2){
		*directionrow = -1;
		*directioncol = 0;
	}

	else{
		*directionrow = 0;
		*directioncol = -1;
	}

}

//??
int generatenewdirection(int upper, int lower){

	int difference = upper - lower;

	int answer = (rand() %difference) + lower;
	if (answer == 2){

		answer = -1;

	}

	
	return(answer);

}

// Snake
void initializesnake(snake* snake, int startposrow, int startposcol, char symbol){
	
	snake->startposcol = startposcol;
	snake->startposrow = startposrow;
	snake->symbol = symbol;

	generatestartdirection(&(snake->directionrow), &(snake->directioncol));
	snake->poscol = startposcol;
	snake->posrow = startposrow;

}

// Game
int displayscore(gameboard gb, score score){
	
	usleep(3000000);

	int moreinput = 1;
	while(moreinput){

		int checkinput = getch();
		if (checkinput == ERR){
			moreinput = 0;
		}
	}

	clear();
	move(gb.maxrow/2, gb.maxcol/2 - 5);
	printw("Game Over\n");

	move(gb.maxrow/2 + 1, gb.maxcol/2 - 13);
	printw("Player 1: %d | Player 2: %d", score.scoreplayer1, score.scoreplayer2);

	move(gb.maxrow/2 + 2, gb.maxcol/2 - 12);
	printw("Press any key to continue\n");

	move(gb.maxrow/2 + 3, gb.maxcol/2 - 7);
	printw("Press q to exit\n");
	
	move(gb.maxrow, 0);	
	printw("\n");

	char c;

	timeout(-1);
	c = getch();
	if (c != 'q'){
		return(1);
	}
	else{
		exit(1);
	}
	
}


int main(int argc, char* argv[])
{

	int newround = 1;
	int restart = 0;
	int gamespeed = 50000;

	char gametype = 0;

	gameboard gb;
	score score;

	score.scoreplayer1 = 0;
	score.scoreplayer2 = 0;


	snake player1;
	snake player2;

	initialize(&gb);

	
	move(gb.maxrow/2, gb.maxcol/2 -8);
	printw("Welcome to jSnake");

	move(gb.maxrow/2 + 1, gb.maxcol/2 - 10);
	printw("Resolution: %d x %d", gb.maxrow, gb.maxcol);

	move(gb.maxrow/2 + 2, gb.maxcol/2 -5);
	printw("Version 1.2");

	move(gb.maxrow/2 + 3, gb.maxcol/2 -16);
	printw("1) Single Player  2) Two Player");

	timeout(-1);
	gametype = getch();

	//move(gb.maxrow/2 + 4, gb.maxcol/2 -1);
	//printw("%d", gametype);

	refresh();

	//usleep(5000000);
	
	while(newround){
		
		makeboard(&gb);
		
		timeout(0);
		initializesnake(&player1, 20, 114, 'X');
		initializesnake(&player2, 20, 57, 'H');

		gb.gametable[player1.posrow][player1.poscol] = player1.symbol;
		gb.gametable[player2.posrow][player2.poscol] = player2.symbol;
		
		paint(&gb);
		usleep(gamespeed);
			

		restart = 0;
		while(!restart){	
			
			int moreinput = 1;
			int inputplayer1 = 0;
			int inputplayer2 = 0;

			while(moreinput){

				int checkinput = getch();
				if (checkinput == ERR){
					moreinput = 0;
				}
				else{
					if(checkinput == KEY_LEFT || checkinput == KEY_RIGHT || checkinput == KEY_UP || checkinput == KEY_DOWN){
						inputplayer1 = checkinput;
					}
					
					if(checkinput == 'a' || checkinput == 'd'|| checkinput == 'w' || checkinput == 's'){
						inputplayer2 = checkinput;
					}
				}
			}
			
			if(inputplayer1 == KEY_LEFT && player1.directioncol != 1){			
				player1.directioncol = -1;
				player1.directionrow = 0;
			}

			else if(inputplayer1 == KEY_RIGHT && player1.directioncol != -1){			
				player1.directioncol = 1;
				player1.directionrow = 0;
			}


			else if(inputplayer1 == KEY_UP && player1.directionrow != 1){			
				player1.directioncol = 0;
				player1.directionrow = -1;
			}

			else if(inputplayer1 == KEY_DOWN && player1.directionrow != -1){			
				player1.directioncol = 0;
				player1.directionrow = 1;
			}

			if(gametype == '2'){
				
				if(inputplayer2 == 'a' && player2.directioncol != 1){			
					player2.directioncol = -1;
					player2.directionrow = 0;
				}

				else if(inputplayer2 == 'd' && player2.directioncol != -1){			
					player2.directioncol = 1;
					player2.directionrow = 0;
				}


				else if(inputplayer2 == 'w' && player2.directionrow != 1){			
					player2.directioncol = 0;
					player2.directionrow = -1;
				}

				else if(inputplayer2 == 's' && player2.directionrow != -1){			
					player2.directioncol = 0;
					player2.directionrow = 1;
				}
			}
			else if(gametype == '1') {

				if (player2.directioncol == 1 && gb.gametable[player2.posrow][player2.poscol + 1] != ' '){
					do{

						player2.directionrow = generatenewdirection(3, 1);
						player2.directioncol = 0;
					
					}while(player2.directionrow == 0 || gb.gametable[player2.posrow + player2.directionrow][player2.poscol] != ' ');
				
				}	

				else if (player2.directioncol == -1 && gb.gametable[player2.posrow][player2.poscol - 1] != ' '){
					do{

						player2.directionrow = generatenewdirection(3, 1);
						player2.directioncol = 0;
					
					}while(player2.directionrow == 0 || gb.gametable[player2.posrow + player2.directionrow][player2.poscol] != ' ');
				}	

				else if (player2.directionrow == 1 && gb.gametable[player2.posrow + 1][player2.poscol] != ' '){
					do{

						player2.directioncol = generatenewdirection(3, 1);
						player2.directionrow = 0;
					
					}while(player2.directioncol == 0 || gb.gametable[player2.posrow][player2.poscol + player2.directioncol] != ' ');
				}

				else if (player2.directionrow == -1 && gb.gametable[player2.posrow - 1][player2.poscol] != ' '){
					do{

						player2.directioncol = generatenewdirection(3, 1);
						player2.directionrow = 0;
					
					}while(player2.directioncol == 0 || gb.gametable[player2.posrow][player2.poscol + player2.directioncol] != ' ');
				}	
			}

			player1.posrow = player1.posrow + player1.directionrow;
			player1.poscol = player1.poscol + player1.directioncol;


			
			player2.posrow = player2.posrow + player2.directionrow;
			player2.poscol = player2.poscol + player2.directioncol;

			if (gb.gametable[player1.posrow][player1.poscol] != ' ' && gb.gametable[player2.posrow][player2.poscol] != ' '){
			
				restart = displayscore(gb, score);
			
			}
			else{

				if (gb.gametable[player1.posrow][player1.poscol] != ' '){

					score.scoreplayer2 = score.scoreplayer2 + 1;
					restart = displayscore(gb, score);

				}


				if (gb.gametable[player2.posrow][player2.poscol] != ' '){

					score.scoreplayer1 = score.scoreplayer1 + 1;
					restart = displayscore(gb, score);
				}
			}

			if (restart != 1){
				
				gb.gametable[player1.posrow][player1.poscol] = player1.symbol;
				gb.gametable[player2.posrow][player2.poscol] = player2.symbol;

				

				paint(&gb);
				usleep(gamespeed);
		
			}

		}
	}

	endwin();
	cleanup(&gb);
	return 0;

}

