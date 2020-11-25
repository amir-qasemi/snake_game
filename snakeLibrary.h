#define RIGHT_DIR 0
#define LEFT_DIR 1
#define UP_DIR 2
#define DOWN_DIR 3
#define DELAY 1000



enum gameStatus{
	LOST , CONTINUE
}myGameStatus;

enum existenceStatus{
	YES, NO
};

enum baseSpeed{
	EASY_BASE_SPEED = 100, NORMAL_BASE_SPEED = 80, HARD_BASE_SPEED = 60
}; 

enum maxSpeed{
	EASY_MAX_SPEED = 42, NORMAL_MAX_SPEED = 32, HARD_MAX_SPEED = 17
}; 

enum turbo{
	OFF, ON
};

enum difficultyRatio{
	EASY_RATIO = 1, NORMAL_RATIO = 2, HARD_RATIO = 3
};

struct portal{
	int row[2];
	int column[2];
	int shape;
	int colorID;
	enum existenceStatus portalExistenceStatus;
};

struct hotFruit{
	int row;
	int column;
	int shape;
	int colorID;
	unsigned removingCounter;
	unsigned changeCounter;
	enum existenceStatus hFruitExistenceStatus;
};

struct poisonousFruit{
	int row;
	int column;
	int shape;
	int colorID;
	int terminationCounter;
	int changeCounter;
	enum existenceStatus pFruitExistenceStatus;
}; 

struct inefficaciousFruit{
	int row;
	int column;
	int shape;
	int colorID;
	int changeCounter;
	enum existenceStatus iFruitExistenceStatus;
};
	

struct player{
	int speedScore;
	int score;
	char name[8];
};


struct snakeTailCoordinate{
	int row;
	int column;
};

struct seedCoordinate{
	int row;
	int column;
	int seedShape;
	enum existenceStatus seedExistenceStatus;

};


struct snakeCoordinate{
    int row;
    int column;
};

struct snake{
    int snakeSize;//head of the snake doesn't count in snake's size
    int snakeHeadShape;
    int snakeBodyShape;
    unsigned moveNum;
    struct snakeCoordinate mySnakeCoordinate;
	struct snakeTailCoordinate mySnakeTailCoordinate;
	enum turbo snakeTurbo;
};





int initNCursesTerminal(void){
	initscr();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(FALSE);
	noecho();
	start_color();

	init_pair(2, COLOR_WHITE, COLOR_YELLOW);
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_YELLOW,COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLUE);
	init_pair(6, COLOR_BLACK, COLOR_GREEN);
	init_pair(7, COLOR_BLACK, COLOR_BLUE);
	init_pair(8, COLOR_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_GREEN);
	init_pair(10, COLOR_WHITE, COLOR_WHITE);
	init_pair(11, COLOR_RED, COLOR_RED);
	init_pair(20, COLOR_BLACK, COLOR_WHITE);
	init_pair(21, COLOR_WHITE, COLOR_BLUE);
	init_pair(23, COLOR_GREEN, COLOR_BLUE);
	init_pair(24, COLOR_YELLOW, COLOR_WHITE);
	init_pair(25, COLOR_GREEN, COLOR_WHITE);

	return 0;
}




int intitScreenCoordinate(unsigned **myScreenCoordinate , int maxHeight, int maxWidth){
	int i, j;

	for(i = 0; i < maxHeight; i++){
		for(j = 0; j < maxWidth; j++){
			myScreenCoordinate[i][j] = 0;
		}
	}

	return 0;
}




int initPrintingSnake(struct snake *mySnake, unsigned **myScreenCoordinate, WINDOW *mainStageWin){
	int j;

	for(j = mySnake->snakeSize ; j >= 0 ; j--){
		(mySnake->moveNum)++;
        myScreenCoordinate[mySnake->mySnakeCoordinate.row ][mySnake->mySnakeCoordinate.column - j] =  mySnake->moveNum;
		if (j != 0)		
			mvwprintw(mainStageWin, mySnake->mySnakeCoordinate.row, mySnake->mySnakeCoordinate.column - j, "%c", mySnake->snakeBodyShape);
		else
			mvwprintw(mainStageWin, mySnake->mySnakeCoordinate.row, mySnake->mySnakeCoordinate.column, "%c", mySnake->snakeHeadShape);
    }

	return 0;
}





int findTheNextTail(unsigned **myScreenCoordinate, struct snake *mySnake, int maxHeight, int maxWidth, struct portal myPortal){
	int i, j;

	for(i = 0; i < maxHeight; i++){
		for(j = 0; j < maxWidth; j++){
			if(myScreenCoordinate[i][j] == mySnake->moveNum - mySnake->snakeSize + 1){
				mySnake->mySnakeTailCoordinate.row = i;
				mySnake->mySnakeTailCoordinate.column = j;
				return 0;
			}
		}
	}

	
	return (-1);
}





int moveSnakeOneStep(int prevMove, struct snake *mySnake){
    if (prevMove == LEFT_DIR){
        mySnake->mySnakeCoordinate.column--;            
	}   
   
    if (prevMove == RIGHT_DIR){
        mySnake->mySnakeCoordinate.column++;             
    }
    
    if (prevMove == UP_DIR){
        (mySnake->mySnakeCoordinate.row--);            
    }    

    if (prevMove == DOWN_DIR){
        (mySnake->mySnakeCoordinate.row++);            
    }  

	return 0;	
	
}





//if the snake has reached to the end or begining of one side, this function causes the snake to come from the opposite side
int bringSnakeFromOtherSide(int maxHeight, int maxWidth, struct snake *mySnake, struct portal myPortal, int prevMove){
	if(mySnake->mySnakeCoordinate.row == myPortal.row[0] && mySnake->mySnakeCoordinate.column == myPortal.column[0])
		switch(prevMove){
			case LEFT_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[1] ;
				mySnake->mySnakeCoordinate.column = myPortal.column[1] - 1;
				break;
			case RIGHT_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[1];
				mySnake->mySnakeCoordinate.column = myPortal.column[1] + 1;
				break;
			case UP_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[1] - 1 ;
				mySnake->mySnakeCoordinate.column = myPortal.column[1];
				break;
			case DOWN_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[1] + 1;
				mySnake->mySnakeCoordinate.column = myPortal.column[1];
				break;
		}

	if(mySnake->mySnakeCoordinate.row == myPortal.row[1] && mySnake->mySnakeCoordinate.column == myPortal.column[1])
		switch(prevMove){
			case LEFT_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[0] ;
				mySnake->mySnakeCoordinate.column = myPortal.column[0] - 1;
				break;
			case RIGHT_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[0];
				mySnake->mySnakeCoordinate.column = myPortal.column[0] + 1;
				break;
			case UP_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[0] - 1 ;
				mySnake->mySnakeCoordinate.column = myPortal.column[0];
				break;
			case DOWN_DIR:
				mySnake->mySnakeCoordinate.row = myPortal.row[0] + 1;
				mySnake->mySnakeCoordinate.column = myPortal.column[0];
				break;
		}


    if (mySnake->mySnakeCoordinate.column == maxWidth) 
        mySnake->mySnakeCoordinate.column -= (maxWidth);
    else if(mySnake->mySnakeCoordinate.column == -1)
        mySnake->mySnakeCoordinate.column = maxWidth - 1;
    if (mySnake->mySnakeCoordinate.row   == maxHeight)
        mySnake->mySnakeCoordinate.row -= (maxHeight);
    else if(mySnake->mySnakeCoordinate.row   == -1)
        mySnake->mySnakeCoordinate.row = maxHeight - 1;
	
	return 0;
}




int changeTheDirection(int lastKeyPressed, int *prevMove){
    if(lastKeyPressed == KEY_DOWN)
		if ( (*prevMove) != UP_DIR)        
			*prevMove = DOWN_DIR;
    if(lastKeyPressed == KEY_UP)
		if ( (*prevMove) != DOWN_DIR)        
			*prevMove = UP_DIR;
    if(lastKeyPressed == KEY_LEFT)
		if ( (*prevMove) != RIGHT_DIR)
        	*prevMove = LEFT_DIR;
    if(lastKeyPressed  == KEY_RIGHT)
		if ( (*prevMove) != LEFT_DIR)
	        *prevMove = RIGHT_DIR;
}





int checkCollision(struct snake *mySnake, unsigned **myScreenCoordinate, float **mapCoordinate, int numOfWalls){
	enum gameStatus myGameStatus = CONTINUE;
	int i, j;

	if(myScreenCoordinate[mySnake->mySnakeCoordinate.row][mySnake->mySnakeCoordinate.column] < mySnake->moveNum && myScreenCoordinate[mySnake->mySnakeCoordinate.row][mySnake->mySnakeCoordinate.column] > mySnake->moveNum - mySnake->snakeSize){
		myGameStatus = LOST;		
		return myGameStatus;
	}



	for(i = 0; i < numOfWalls; i++){
		if(mapCoordinate[i][0] == mapCoordinate[i][2]){
			for(j = (int)(mapCoordinate[i][1]) ; j <= mapCoordinate[i][3] ; j++)
				if(mySnake->mySnakeCoordinate.row == mapCoordinate[i][0] && mySnake->mySnakeCoordinate.column == j){
					myGameStatus = LOST;
					return myGameStatus;
				}
		}

		else if(mapCoordinate[i][1] == mapCoordinate[i][3]){
			for(j = (int)(mapCoordinate[i][0]) ; j <= mapCoordinate[i][2]; j++)
				if(mySnake->mySnakeCoordinate.row == j && mySnake->mySnakeCoordinate.column == mapCoordinate[i][1]){
					myGameStatus = LOST;
					return myGameStatus;
				}
		}
	}

	myGameStatus = CONTINUE;		
	return myGameStatus;

}




int generateSeed(struct seedCoordinate *mySeedCoordinate, int maxHeight, int maxWidth, struct snake mySnake, unsigned **myScreenCoordinate, float **mapCoordinate, int numOfWalls, struct portal myPortal){
	int i = 1, j, z, k;	
	while(i == 1){
		mySeedCoordinate->row = rand() % maxHeight;
		mySeedCoordinate->column = rand() % maxWidth; 
		i = 0;
		for(j = 0; j < mySnake.snakeSize; j++){
			if(mySeedCoordinate->row == mySnake.mySnakeTailCoordinate.row && mySeedCoordinate->column == mySnake.mySnakeTailCoordinate.column)
				i = 1;
			findTheNextTail(myScreenCoordinate, &mySnake, maxHeight,maxWidth, myPortal);
		}


		for(z = 0; z < numOfWalls; z++){
			if(mapCoordinate[z][0] == mapCoordinate[z][2]){
				for(k = (int)(mapCoordinate[z][1]) ; k <= mapCoordinate[z][3] ; k++)
					if(mySeedCoordinate->row == mapCoordinate[z][0] && mySeedCoordinate->column == k){
						i = 1;
					}
			}

			else if(mapCoordinate[z][1] == mapCoordinate[z][3]){
				for(k = (int)(mapCoordinate[z][0]) ; k <= mapCoordinate[z][2]; k++)
					if(mySeedCoordinate->row == k && mySeedCoordinate->column == mapCoordinate[z][1]){
						i = 1;
					}
			}
		}

	}
	mySeedCoordinate->seedExistenceStatus = YES;
	
	return 0;
}



int generatePFruit(struct poisonousFruit *myPoisonousFruit ,struct seedCoordinate mySeedCoordinate,int maxHeight,int maxWidth,struct snake mySnake,unsigned **myScreenCoordinate, float **mapCoordinate, int numOfWalls, struct portal myPortal){
	int i = 1, j, z, k;	
	while(i == 1){
		myPoisonousFruit->row = rand() % maxHeight;
		myPoisonousFruit->column = rand() % maxWidth; 
		i = 0;
		for(j = 0; j < mySnake.snakeSize; j++){
			if( ( myPoisonousFruit->row == mySnake.mySnakeTailCoordinate.row && myPoisonousFruit->column == mySnake.mySnakeTailCoordinate.column ) )
				i = 1;
			if( myPoisonousFruit->row == mySeedCoordinate.row && myPoisonousFruit->column == mySeedCoordinate.column )
				i = 1;
			findTheNextTail(myScreenCoordinate, &mySnake, maxHeight,maxWidth, myPortal);
		}


		for(z = 0; z < numOfWalls; z++){
			if(mapCoordinate[z][0] == mapCoordinate[z][2]){
				for(k = (int)(mapCoordinate[z][1]) ; k <= mapCoordinate[z][3] ; k++)
					if(myPoisonousFruit->row == mapCoordinate[z][0] && myPoisonousFruit->column == k){
						i = 1;
					}
			}

			else if(mapCoordinate[z][1] == mapCoordinate[z][3]){
				for(k = (int)(mapCoordinate[z][0]) ; k <= mapCoordinate[z][2]; k++)
					if(myPoisonousFruit->row == k && myPoisonousFruit->column == mapCoordinate[z][1]){
						i = 1;
					}
			}
		}

	}
	myPoisonousFruit->pFruitExistenceStatus = YES;
	myPoisonousFruit->changeCounter = 0;
	
	return 0;
}




int generateInefficaciousFruit(struct inefficaciousFruit *myInefficaciousFruit,int  maxHeight, int  maxWidth, struct snake mySnake, unsigned **myScreenCoordinate, struct poisonousFruit myPoisonousFruit ,struct seedCoordinate mySeedCoordinate, float **mapCoordinate, int numOfWalls,  struct portal myPortal){
	int i = 1, j, z, k;	
	while(i == 1){
		myInefficaciousFruit->row = rand() % maxHeight;
		myInefficaciousFruit->column = rand() % maxWidth; 
		i = 0;
		for(j = 0; j < mySnake.snakeSize; j++){
			if( ( myInefficaciousFruit->row == mySnake.mySnakeTailCoordinate.row && myInefficaciousFruit->column == mySnake.mySnakeTailCoordinate.column ) )
				i = 1;

			if( myInefficaciousFruit->row == mySeedCoordinate.row && myInefficaciousFruit->column == mySeedCoordinate.column )
				i = 1;

			if( myInefficaciousFruit->row == myPoisonousFruit.row && myInefficaciousFruit->column == myPoisonousFruit.column )
				i = 1;

			findTheNextTail(myScreenCoordinate, &mySnake, maxHeight,maxWidth, myPortal);
		}

		for(z = 0; z < numOfWalls; z++){
			if(mapCoordinate[z][0] == mapCoordinate[z][2]){
				for(k = (int)(mapCoordinate[z][1]) ; k <= mapCoordinate[z][3] ; k++)
					if(myInefficaciousFruit->row == mapCoordinate[z][0] && myInefficaciousFruit->column == k){
						i = 1;
					}
			}

			else if(mapCoordinate[z][1] == mapCoordinate[z][3]){
				for(k = (int)(mapCoordinate[z][0]) ; k <= mapCoordinate[z][2]; k++)
					if(myInefficaciousFruit->row == k && myInefficaciousFruit->column == mapCoordinate[z][1]){
						i = 1;
					}
			}
		}
	}
	myInefficaciousFruit->iFruitExistenceStatus = YES;
	myInefficaciousFruit->changeCounter = 0;
	
	return 0;	
}




int generateHotFruit(struct hotFruit *myHotFruit,int  maxHeight, int  maxWidth, struct snake mySnake, unsigned **myScreenCoordinate, struct poisonousFruit myPoisonousFruit , struct inefficaciousFruit myInefficaciousFruit, struct seedCoordinate mySeedCoordinate, float **mapCoordinate, int numOfWalls, struct portal myPortal){
	int i = 1, j , z, k;	

	while(i == 1){
		myHotFruit->row = rand() % maxHeight;
		myHotFruit->column = rand() % maxWidth; 
		i = 0;
		for(j = 0; j < mySnake.snakeSize; j++){
			if( ( myHotFruit->row == mySnake.mySnakeTailCoordinate.row && myHotFruit->column == mySnake.mySnakeTailCoordinate.column ) )
				i = 1;

			if( myHotFruit->row == mySeedCoordinate.row && myHotFruit->column == mySeedCoordinate.column )
				i = 1;

			if( myHotFruit->row == myPoisonousFruit.row && myHotFruit->column == myPoisonousFruit.column )
				i = 1;

			if (myHotFruit->row == myInefficaciousFruit.row && myHotFruit->column == myInefficaciousFruit.row)
				i = 1;

			findTheNextTail(myScreenCoordinate, &mySnake, maxHeight,maxWidth, myPortal);
		}

		for(z = 0; z < numOfWalls; z++){
			if(mapCoordinate[z][0] == mapCoordinate[z][2]){
				for(k = (int)(mapCoordinate[z][1]) ; k <= mapCoordinate[z][3] ; k++)
					if(myHotFruit->row == mapCoordinate[z][0] && myHotFruit->column == k){
						i = 1;
					}
			}

			else if(mapCoordinate[z][1] == mapCoordinate[z][3]){
				for(k = (int)(mapCoordinate[z][0]) ; k <= mapCoordinate[z][2]; k++)
					if(myHotFruit->row == k && myHotFruit->column == mapCoordinate[z][1]){
						i = 1;
					}
			}
		}
	}
	myHotFruit->hFruitExistenceStatus = YES;
	myHotFruit->changeCounter = 0;
	
	return 0;	
}



	


int createPortal(struct portal *myPortal,int  maxHeight, int  maxWidth, float **mapCoordinate, int numOfWalls){
	int i = 1, j, z, k;

	for(j = 0; j < 2; j++){
		while(i == 1){
			myPortal->row[j] = rand() % maxHeight;
			myPortal->column[j] = rand() % maxWidth;	
			i = 0;

			for(z = 0; z < numOfWalls; z++){
				if(mapCoordinate[z][0] == mapCoordinate[z][2]){
					for(k = (int)(mapCoordinate[z][1]) ; k <= mapCoordinate[z][3] ; k++)
						if(myPortal->row[j] == mapCoordinate[z][0] && myPortal->column[j] == k){
							i = 1;
						}
				}
	
				else if(mapCoordinate[z][1] == mapCoordinate[z][3]){
					for(k = (int)(mapCoordinate[z][0]) ; k <= mapCoordinate[z][2]; k++)
						if(myPortal->row[j] == k && myPortal->column[j] == mapCoordinate[z][1]){
							i = 1;
						}
				}
			}
		}
		i = 1;
	}

	myPortal->portalExistenceStatus = YES;

	return 0;
}






void addTopScore(struct player player1){
	FILE *topScoresfPtr;
	WINDOW *endOfGameWin;
	int maxHeight, maxWidth;
	int tempScore = 0,temp2Score, i, counter = 0;
	char tempName[8], temp2Name[6];
	int defualtScore = -1;
	char defualtName[8] = "aaaaaaa";

	usleep(50000);//just for slowing down the procces


	getmaxyx(stdscr, maxHeight, maxWidth);
	endOfGameWin = newwin(maxHeight, maxWidth, 0, 0);
	wbkgd(endOfGameWin, COLOR_PAIR(4));

	//if the file doesn't exists, a file would be created and would be initilized with defualt values
	if( ( topScoresfPtr = fopen("topScores.dat", "rb+") ) == NULL){
		topScoresfPtr = fopen("topScores.dat", "wb+");
		for(i = 1; i <= 5; i++){
			fwrite(&defualtScore,sizeof(int), 1, topScoresfPtr);
			fwrite(defualtName, sizeof(char), 7, topScoresfPtr);
		}
	}

	rewind(topScoresfPtr);

	while(!feof(topScoresfPtr)){
		counter++;
		fread(&tempScore,sizeof(int),1 , topScoresfPtr);
		fread(tempName,sizeof(char), 7 , topScoresfPtr);
		if(player1.score >= tempScore ){
			fseek(topScoresfPtr, -(sizeof(int) + sizeof(char) * 7), SEEK_CUR);
			fwrite(&(player1.score), sizeof(int), 1, topScoresfPtr);
			fwrite(player1.name, sizeof(char), 7, topScoresfPtr);			
			break;
		}
		

	}
	
	if(counter == 1){
		mvwprintw(endOfGameWin, maxHeight / 2 - 2, maxWidth / 2 - 15, "Congratulation, a new record!!!");		
	}

	mvwprintw(endOfGameWin, maxHeight / 2 - 1, maxWidth / 2 - 15, "your score is: %d", player1.score);


	while(!feof(topScoresfPtr) && tempScore != -1 && counter < 5){
		counter++;
		fread(&temp2Score,sizeof(int),1 , topScoresfPtr);
		fread(temp2Name,sizeof(char), 7 , topScoresfPtr);		
		fseek(topScoresfPtr, -(sizeof(int) + sizeof(char) * 7), SEEK_CUR);
		fwrite(&tempScore, sizeof(int), 1, topScoresfPtr);
		fwrite(tempName, sizeof(char), 7, topScoresfPtr);	
		tempScore = temp2Score;
		strcpy(tempName, temp2Name);
	
	}



	fseek(topScoresfPtr,0,SEEK_SET);
	
	wattron(endOfGameWin, A_BOLD );
	mvwprintw(endOfGameWin,maxHeight / 2 + 1, maxWidth / 2 - 15, "Last 5 top scores:");
	wattroff(endOfGameWin, A_BOLD );

	for(i = 0; i < 5; i++){
		fread(&tempScore,sizeof(int),1 , topScoresfPtr);
		fread(tempName,sizeof(char), 7 , topScoresfPtr);

		if(tempScore != -1)
			mvwprintw(endOfGameWin,maxHeight / 2 + i + 2, maxWidth / 2 - 15,"%d.name: %-7s\tscore: %3d\n",i + 1, tempName, tempScore);
	}

	fclose(topScoresfPtr);


	keypad(endOfGameWin, TRUE);
	mvwprintw(endOfGameWin,maxHeight / 2 + 8, maxWidth / 2 - 15, "press any key to quit...");
	usleep(100000);//just for slowing down the procces
	wgetch(endOfGameWin);
	wrefresh(endOfGameWin);

}


int findTheHighestScore(){
	FILE *topScoresfPtr;	
	int highScore;

	if ( ( topScoresfPtr = fopen("topScores.dat", "rb+") ) == NULL)
		return 0;

	rewind(topScoresfPtr);
	fread(&highScore,sizeof(int), 1, topScoresfPtr);

	if(highScore == -1)
		return 0;
	else
		return highScore;

}
	

int menu(const int i){
	WINDOW *menuWin;
	int currentChoice, counter, counter2;
	const char *menuChoices[ 3 ] = {"Resume", "Play", "Quit"};
	int c, maxHeight, maxWidth; 
	
	
	getmaxyx(stdscr, maxHeight, maxWidth);
	if(i == 0){
		menuWin = newwin(maxHeight, maxWidth, 0 , 0);
		wborder(menuWin,'*','*','*','*','*','*','*','*');
	}
	else{
		menuWin = newwin( (int)(maxHeight / 2), (int)(maxWidth / 2), (int)(maxHeight / 4), (int)(maxWidth / 4) - 7);
		getmaxyx(menuWin, maxHeight, maxWidth);
		wborder(menuWin,'$','$','$','$','$','$','$','$');
	}


	keypad(menuWin, TRUE);
	nodelay(menuWin, TRUE);
	
	if(i == 0)
		currentChoice = 1;
	else
		currentChoice = 0;


	while(1){
		c = wgetch(menuWin);
		
		if(i == 0){

			if(c == KEY_UP){
				if(currentChoice == 1)
					currentChoice = 2;
				else
					currentChoice--;
			}

			else if(c == KEY_DOWN){
				if(currentChoice == 2)
					currentChoice = 1;
				else
					currentChoice++;
			}
		}

		else{	
		
			if(c == KEY_UP){
				if(currentChoice == 2)
					currentChoice -= 2;
				else if(currentChoice == 0)
					currentChoice = 2;
				else
					currentChoice--;
			}

			else if(c == KEY_DOWN){
				if(currentChoice == 0)
					currentChoice += 2;
				else if(currentChoice == 2)
					currentChoice = 0;
				else
					currentChoice++;							
			}
		}
			


		if(i == 0){
			for(counter = 1; counter <= 2; counter++){
				if(currentChoice == counter){
					wattron(menuWin,COLOR_PAIR(1));
					mvwprintw(menuWin, maxHeight / 2 - 2 + counter, maxWidth / 2 - 5 , "%s", menuChoices[counter]);
					wattroff(menuWin, COLOR_PAIR(1));
				}
				
				else{
					mvwprintw(menuWin , maxHeight / 2 - 2 + counter, maxWidth / 2 - 5 , "%s", menuChoices[counter]);
				}
			}
		}

		
		else{
			counter2 =0;
			for(counter = 0 ; counter <= 2; counter++){
				if(counter == 1)
					continue;
				counter2++;				
				if(currentChoice == counter){
					wattron(menuWin,A_REVERSE | A_BLINK);
					mvwprintw(menuWin,maxHeight / 2 - 2 + counter2, maxWidth / 2 - 5 , "%s", menuChoices[counter]);
					wattroff(menuWin,A_REVERSE | A_BLINK);
				}
				
				else{
					mvwprintw(menuWin,maxHeight / 2 - 2 + counter2, maxWidth / 2 - 5 , "%s", menuChoices[counter]);
				}
			}
		}

		wrefresh(menuWin);
		

		if (c == 'q' || c == 'Q'){
			wclear(menuWin);
			delwin(menuWin);
			return 0;
		}		

		//entering the return values of the chosen choice			
		if (c == '\n'){
			wclear(menuWin);
			delwin(menuWin);
			return currentChoice;
		}
	}

}


int determineDifficulty(enum difficultyRatio *seedDifficultyRatio,enum  maxSpeed *myMaxSpeed,enum baseSpeed *myBaseSpeed){
	char *difficulties[3] = {"Easy", "Normal", "Hard"};
	int maxHeight, maxWidth , currentChoice = 0, selectedChoice;
	WINDOW *difficultyWin;
	int c , i;
	
	getmaxyx(stdscr, maxHeight, maxWidth);
	
	difficultyWin = newwin(maxHeight, maxWidth, 0, 0);
	keypad(difficultyWin, TRUE);
	nodelay(difficultyWin, TRUE);
	wbkgd(difficultyWin, COLOR_PAIR(20));

	while(1){	
		c = wgetch(difficultyWin);	

	
		for(i = 0; i < 3; i++){
			if (i == currentChoice){
				//just for adjusting hard in screen in a better way				
				if(i == 2){
					wattron(difficultyWin, A_REVERSE);
					mvwprintw(difficultyWin, maxHeight / 2, maxWidth / 2 - 15 + i  * 10 + 2,"%s",  difficulties[i]);
					wattroff(difficultyWin, A_REVERSE);
				}
				else{
					wattron(difficultyWin, A_REVERSE);
					mvwprintw(difficultyWin, maxHeight / 2, maxWidth / 2 - 15 + i  * 10 ,"%s",  difficulties[i]);
					wattroff(difficultyWin, A_REVERSE);
				}
			}
			else{
				//just for adjusting hard in screen in a better way
				if(i == 2)
					mvwprintw(difficultyWin, maxHeight / 2, maxWidth / 2 - 15 + i * 10 + 2 ,"%s", difficulties[i]);
				else
					mvwprintw(difficultyWin, maxHeight / 2, maxWidth / 2 - 15 + i * 10 ,"%s", difficulties[i]);
			}
		}
	
	
		if(c == KEY_LEFT){
			if(currentChoice == 0)
				currentChoice = 2;
			else
				currentChoice--;
		}
	
		if(c == KEY_RIGHT){
			if(currentChoice == 2)
				currentChoice = 0;
			else
				currentChoice++;
		}
	
		if(c == '\n'){
			selectedChoice = currentChoice;
			break;
		}
		wrefresh(difficultyWin);

	}// while end

	switch (selectedChoice){
		case 0:
			*seedDifficultyRatio = EASY_RATIO;
			*myMaxSpeed = EASY_MAX_SPEED;
			*myBaseSpeed = EASY_BASE_SPEED;
			wclear(difficultyWin);
			delwin(difficultyWin);
			return selectedChoice;
			break;

		case 1:
			*seedDifficultyRatio = NORMAL_RATIO;
			*myMaxSpeed = NORMAL_MAX_SPEED;
			*myBaseSpeed = NORMAL_BASE_SPEED;
			wclear(difficultyWin);
			delwin(difficultyWin);
			return selectedChoice;
			break;

		case 2:
			*seedDifficultyRatio = HARD_RATIO;
			*myMaxSpeed = HARD_MAX_SPEED;
			*myBaseSpeed = HARD_BASE_SPEED;
			wclear(difficultyWin);
			delwin(difficultyWin);
			return selectedChoice;
			break;
	}
			
}

void getPlayerName(struct player *player1){
	int maxHeight, maxWidth;
	WINDOW *playerNameWin;
	curs_set(TRUE);
	echo();

	getmaxyx(stdscr, maxHeight, maxWidth);

	playerNameWin = newwin(maxHeight, maxWidth, 0, 0);
	wbkgd(playerNameWin, COLOR_PAIR(20));

	wattron(playerNameWin, A_BOLD);
	mvwprintw(playerNameWin, maxHeight / 2, maxWidth / 2 - 15, "Please enter your name: ");
	wattroff(playerNameWin, A_BOLD);

	wscanw(playerNameWin,"%7s", player1->name);

	curs_set(FALSE);
	noecho();

	wclear(playerNameWin);
	delwin(playerNameWin);


}


float ** selectMap(int difficulty, int *numOfWalls,int maxHeight,int maxWidth){
	int i, j, mapNum;
	float **mapCoordinate;
	FILE *mapfPtr;
	

    if( (mapfPtr = fopen("maps.dat", "r+")) == NULL){
        perror("Error: ");
        exit(-1);
    }

	switch(difficulty){
		case 0:
			mapCoordinate = malloc(sizeof(int *) * 2);
			for(i = 0; i < 2; i++){
				mapCoordinate[i] = malloc(sizeof(int) * 4);
				for(j = 0; j < 4; j++)				
					mapCoordinate[i][j] = -100;
			}
			*numOfWalls = 0;
			return mapCoordinate;
			break;
		case 1:
			mapNum = 97 + rand() % 5;
			break;
		case 2:
			mapNum = 102 + rand() % 3;
			break;
	}

	rewind(mapfPtr);
	while(fgetc(mapfPtr) != mapNum);
	fgetc(mapfPtr);
	fscanf(mapfPtr, "%d", &(*numOfWalls));

	mapCoordinate = malloc(sizeof(int *) * (*numOfWalls));
	for(i = 0; i < (*numOfWalls); i++)
		mapCoordinate[i] = malloc(sizeof(int) * 4);

	for(j = 0; j < (*numOfWalls); j++){
		fscanf(mapfPtr, "%f%f%f%f", &mapCoordinate[j][0], &mapCoordinate[j][1], &mapCoordinate[j][2], &mapCoordinate[j][3]);
		fgetc(mapfPtr);
	}

	
	for(j = 0; j < (*numOfWalls); j++)	
		for(i = 0; i < 4; i++){
			if(i % 2 == 0){
				if (mapCoordinate[j][i] == 1)
					mapCoordinate[j][i] = maxHeight - 1;
				else if(mapCoordinate[j][i] == 0)
					mapCoordinate[j][i] = 0;
				else				
					mapCoordinate[j][i] = (int)(mapCoordinate[j][i] * maxHeight -1);
			}	
			else{
				if (mapCoordinate[j][i] == 1)
					mapCoordinate[j][i] = maxWidth - 1;
				else if(mapCoordinate[j][i] == 0)
					mapCoordinate[j][i] = 0;
				else
				mapCoordinate[j][i] = (int)(mapCoordinate[j][i] * maxWidth -1);
			}
		}


	return mapCoordinate;

}



void bringTheInstruction(int maxHeight,int maxWidth){
	WINDOW *instructionWin;
	int i = 0;
	nodelay(instructionWin, FALSE);
	
	instructionWin = newwin(maxHeight, maxWidth, 0 ,0);
	wbkgd(instructionWin, COLOR_PAIR(20));

	mvwprintw(instructionWin, maxHeight / 2 - 5, maxWidth / 2 - 15, "Use arrow keys to move in the screen,");
	mvwprintw(instructionWin, maxHeight / 2 - 4, maxWidth / 2 - 15, "Press Q to stop the game,");
	mvwprintw(instructionWin, maxHeight / 2 - 3, maxWidth / 2 - 15, "Try to achieve as high as a score as you can,");
	mvwprintw(instructionWin, maxHeight / 2 , maxWidth / 2 - 15, "# ----> Snake's Head");
	mvwprintw(instructionWin, maxHeight / 2 + 1, maxWidth / 2 - 15, ". ----> Snake's Body");
	wattron(instructionWin, COLOR_PAIR(24) );
	mvwprintw(instructionWin, maxHeight / 2 + 2, maxWidth / 2 - 15, "*");
	wattroff(instructionWin, COLOR_PAIR(24) );
	mvwprintw(instructionWin, maxHeight / 2 + 2, maxWidth / 2 + 1 - 15, " ----> Score");
	wattron(instructionWin, COLOR_PAIR(8));
	mvwprintw(instructionWin, maxHeight / 2 + 3, maxWidth / 2 - 15, "*");
	wattroff(instructionWin, COLOR_PAIR(8));
	mvwprintw(instructionWin, maxHeight / 2 + 3, maxWidth / 2 - 15 + 1, " ----> Poisonous Fruit");
	wattron(instructionWin, COLOR_PAIR(10));
	mvwprintw(instructionWin, maxHeight / 2 + 4, maxWidth / 2 - 15, "*");
	wattroff(instructionWin, COLOR_PAIR(10));
	mvwprintw(instructionWin, maxHeight / 2 + 4, maxWidth / 2 - 15 + 1, "----> Inefficiance Fruit");
	wattron(instructionWin, COLOR_PAIR(11));
	mvwprintw(instructionWin, maxHeight / 2 + 5, maxWidth / 2 - 15 , "*");
	wattroff(instructionWin, COLOR_PAIR(11));
	mvwprintw(instructionWin, maxHeight / 2 + 5, maxWidth / 2 - 15 + 1, "----> Hot Fruit");
	mvwprintw(instructionWin, maxHeight / 2 + 6, maxWidth / 2 - 15 , "# ----> Walls");
	wattron(instructionWin, COLOR_PAIR(25));
	mvwprintw(instructionWin, maxHeight / 2 + 7 , maxWidth / 2 - 15 , "O");
	wattroff(instructionWin, COLOR_PAIR(25));
	mvwprintw(instructionWin, maxHeight / 2 + 7 , maxWidth / 2 - 15 + 1 , " ----> Portals");

	mvwprintw(instructionWin, maxHeight / 2 + 9 , maxWidth / 2 - 15 , "press any key to start the game...");
	wrefresh(instructionWin);
	wgetch(instructionWin);

	wclear(instructionWin);
	delwin(instructionWin);
}





