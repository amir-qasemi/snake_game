/*
 ============================================================================
 Name        : snake.c
 Author      : Amirhossein Ghasemi
 Version     : 1.0.0
 MIT License

 Copyright (c) 2015 AmirQasemi

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE
 ============================================================================
 */


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snakeLibrary.h"



int main(){
    int i, j, lastKeyPressed = KEY_RIGHT, prevMove = RIGHT_DIR , maxWidth, maxHeight, passedTime = 0; 
	unsigned movementCounter, timeCounter = 0;
	unsigned **myScreenCoordinate;
	struct snake mySnake = {3, '#', '.', 0, {4, 5}, {10, 17} , OFF};
	struct seedCoordinate mySeedCoordinate ={0, 0 , '*', NO};
	struct player player1 = {0, 0, "player"};
	struct poisonousFruit myPoisonousFruit = {0 , 0 , '*', 8, 0,  0, NO};
	struct inefficaciousFruit myInefficaciousFruit = {0 , 0, '*', 10, 0 , NO};
	struct hotFruit myHotFruit = {0 , 0, '*', 11, 0, 1 , NO};
	struct portal myPortal = {{0, 0}, {0, 0} ,'O' , 23, NO, };
	enum baseSpeed myBaseSpeed = HARD_BASE_SPEED;
	enum maxSpeed myMaxSpeed = HARD_MAX_SPEED;
	enum difficultyRatio seedDifficultyRatio = HARD_RATIO;
	int speedCounter;
	char *difficulties[3] = {"Easy", "Normal", "Hard"};
	int difficulty;
	float **mapCoordinate;
    int numOfWalls;
	srand(time(NULL));
 
	WINDOW *mainStageWin, *timeAndScoreWin;


    initNCursesTerminal();
	getmaxyx(stdscr, maxHeight, maxWidth);

	mainStageWin = newwin(maxHeight, maxWidth - 18, 0, 0);
	timeAndScoreWin = newwin(maxHeight, 18, 0, maxWidth - 17);

	getmaxyx(mainStageWin, maxHeight, maxWidth);
	wbkgd(timeAndScoreWin, COLOR_PAIR(1));
	wbkgd(mainStageWin, COLOR_PAIR(3));



	switch( menu(0) ){
		case 1:
			difficulty = determineDifficulty(&seedDifficultyRatio, &myMaxSpeed, &myBaseSpeed);
			getPlayerName(&player1);
            mapCoordinate = selectMap(difficulty , &numOfWalls, maxHeight, maxWidth);
			bringTheInstruction(maxHeight, maxWidth);					
			break;
		case 2:
			endwin();
			return;
	}




    myScreenCoordinate = malloc( sizeof(unsigned *) * (maxHeight) );
    for(i= 0; i < maxHeight; i++)
        myScreenCoordinate[i] = malloc( sizeof(unsigned) * (maxWidth) );

	intitScreenCoordinate(myScreenCoordinate, maxHeight, maxWidth);
	initPrintingSnake(&mySnake, myScreenCoordinate, mainStageWin);    
	

    mySnake.mySnakeTailCoordinate.row = mySnake.mySnakeCoordinate.row;
    mySnake.mySnakeTailCoordinate.column = mySnake.mySnakeCoordinate.column - mySnake.snakeSize;

	wattron(timeAndScoreWin, COLOR_PAIR(6) );
		mvwprintw(timeAndScoreWin, 1, 1, "PLAYER'S NAME:");
		mvwprintw(timeAndScoreWin, 7 , 1,"PASSED TIME:");
		mvwprintw(timeAndScoreWin, 10, 1, "HIGHEST SCORE:");
		mvwprintw(timeAndScoreWin, 4, 1,"SCORE:" );
		mvwprintw(timeAndScoreWin, maxHeight - 2 , 1,"%s" , difficulties[difficulty] );
		
	wattroff(timeAndScoreWin, COLOR_PAIR(6) );

	mvwprintw(timeAndScoreWin, 2, 1, "%s", player1.name);
	mvwprintw(timeAndScoreWin, 11, 1, "%d", findTheHighestScore() );

	createPortal(&myPortal, maxHeight, maxWidth, mapCoordinate, numOfWalls);


	wattron(mainStageWin,COLOR_PAIR(7));
    //printing the walls of stage
	for(i = 0; i < numOfWalls; i++){
		if(mapCoordinate[i][0] == mapCoordinate[i][2]){
			for(j = (int)(mapCoordinate[i][1]) ; j <= mapCoordinate[i][3] ; j++)
				mvwprintw(mainStageWin, (int)(mapCoordinate[i][0]), j, "H");
		}

		else if(mapCoordinate[i][1] == mapCoordinate[i][3]){
			for(j = (int)(mapCoordinate[i][0]) ; j <= mapCoordinate[i][2]; j++)
				mvwprintw(mainStageWin, j, (int)(mapCoordinate[i][1]), "H");
		}
	}
	wattroff(mainStageWin, COLOR_PAIR(7));

	movementCounter = 0;
	generateHotFruit(&myHotFruit, maxHeight, maxWidth, mySnake, myScreenCoordinate, myPoisonousFruit , myInefficaciousFruit, mySeedCoordinate, mapCoordinate, numOfWalls, myPortal);	

    while(1){

		
		/*increasing snake's speed. starts with one position per 0.1 second and with every point the player aquires */
		/*the time which snake crosses one position will be subtracted by 0.001 up until the score*/
		/*is 90. afterward the speed will be fixed to one position per 0.001 second*/  
		if(mySnake.snakeTurbo == ON)
			speedCounter = myBaseSpeed - 15 - (player1.speedScore <=  myBaseSpeed - myMaxSpeed ? player1.speedScore : myBaseSpeed - myMaxSpeed);
		else
			speedCounter = myBaseSpeed - (player1.speedScore <=  myBaseSpeed - myMaxSpeed ? player1.speedScore : myBaseSpeed - myMaxSpeed);

		if(movementCounter == speedCounter){


			movementCounter = 0;
			lastKeyPressed = getch();		
   
			changeTheDirection(lastKeyPressed, &prevMove);

   
			if (mySnake.mySnakeCoordinate.row == mySeedCoordinate.row && mySnake.mySnakeCoordinate.column == mySeedCoordinate.column && mySeedCoordinate.seedExistenceStatus == YES){
				mySnake.snakeSize++;
				mySeedCoordinate.seedExistenceStatus = NO;
				(player1.speedScore)++;
				(player1.score) += seedDifficultyRatio;
			}

			if(mySeedCoordinate.seedExistenceStatus == NO)
				generateSeed(&mySeedCoordinate, maxHeight, maxWidth, mySnake, myScreenCoordinate, mapCoordinate, numOfWalls, myPortal);

 			if(mySeedCoordinate.seedExistenceStatus == YES){
				wattron(mainStageWin,COLOR_PAIR(3) | A_BLINK);
				mvwprintw(mainStageWin, mySeedCoordinate.row, mySeedCoordinate.column, "%c", mySeedCoordinate.seedShape);        
				wattroff(mainStageWin,COLOR_PAIR(3) | A_BLINK);
			}




			if (mySnake.mySnakeCoordinate.row == myPoisonousFruit.row && mySnake.mySnakeCoordinate.column == myPoisonousFruit.column && myPoisonousFruit.pFruitExistenceStatus == YES){
				myPoisonousFruit.terminationCounter++;
				myPoisonousFruit.pFruitExistenceStatus = NO;
			}

			if(myPoisonousFruit.pFruitExistenceStatus == NO){
				mvwprintw(mainStageWin, myPoisonousFruit.row, myPoisonousFruit.column , " ");	
				generatePFruit(&myPoisonousFruit ,mySeedCoordinate, maxHeight, maxWidth, mySnake, myScreenCoordinate, mapCoordinate, numOfWalls, myPortal);
			}

 			if(myPoisonousFruit.pFruitExistenceStatus == YES){
				wattron(mainStageWin,COLOR_PAIR(myPoisonousFruit.colorID) );
				mvwprintw(mainStageWin, myPoisonousFruit.row, myPoisonousFruit.column, "%c",  myPoisonousFruit.shape);        
				wattroff(mainStageWin,COLOR_PAIR(myPoisonousFruit.colorID) );
			}




			if (mySnake.mySnakeCoordinate.row == myInefficaciousFruit.row && mySnake.mySnakeCoordinate.column == myInefficaciousFruit.column && myInefficaciousFruit.iFruitExistenceStatus == YES){
				mySnake.snakeSize++;
				(player1.speedScore)++;
				myInefficaciousFruit.iFruitExistenceStatus = NO;
			}

			if(myInefficaciousFruit.iFruitExistenceStatus == NO){
				mvwprintw(mainStageWin, myInefficaciousFruit.row, myInefficaciousFruit.column , " ");
				generateInefficaciousFruit(&myInefficaciousFruit, maxHeight, maxWidth, mySnake, myScreenCoordinate, myPoisonousFruit ,mySeedCoordinate, mapCoordinate, numOfWalls, myPortal);	
			}

 			if(myInefficaciousFruit.iFruitExistenceStatus == YES){
				wattron(mainStageWin,COLOR_PAIR(myInefficaciousFruit.colorID) );
				mvwprintw(mainStageWin, myInefficaciousFruit.row, myInefficaciousFruit.column, "%c",  myInefficaciousFruit.shape);        
				wattroff(mainStageWin,COLOR_PAIR(myInefficaciousFruit.colorID) );
			}


			if (mySnake.mySnakeCoordinate.row == myHotFruit.row && mySnake.mySnakeCoordinate.column == myHotFruit.column && myHotFruit.hFruitExistenceStatus == YES){
				mySnake.snakeTurbo = ON;
				myHotFruit.hFruitExistenceStatus = NO;
				myHotFruit.removingCounter = 0;
			}

 			if(myHotFruit.hFruitExistenceStatus == YES){
				wattron(mainStageWin,COLOR_PAIR(myHotFruit.colorID) );
				mvwprintw(mainStageWin, myHotFruit.row, myHotFruit.column, "%c",  myHotFruit.shape);        
				wattroff(mainStageWin,COLOR_PAIR(myHotFruit.colorID) );
			}


			//printing the portal
			wattron(mainStageWin, COLOR_PAIR(myPortal.colorID)); 
				mvwprintw(mainStageWin, myPortal.row[0], myPortal.column[0], "%c", myPortal.shape);
				mvwprintw(mainStageWin, myPortal.row[1], myPortal.column[1], "%c", myPortal.shape);
			wattroff(mainStageWin, COLOR_PAIR(myPortal.colorID));


	    	mvwprintw(mainStageWin, mySnake.mySnakeTailCoordinate.row, mySnake.mySnakeTailCoordinate.column , " ");
			wattron(mainStageWin, COLOR_PAIR(21)); 
	    	mvwprintw(mainStageWin, mySnake.mySnakeCoordinate.row , mySnake.mySnakeCoordinate.column, "%c", mySnake.snakeBodyShape);
			wattroff(mainStageWin, COLOR_PAIR(21));

			findTheNextTail(myScreenCoordinate, &mySnake, maxHeight, maxWidth, myPortal);		
			moveSnakeOneStep(prevMove, &mySnake); 
			bringSnakeFromOtherSide(maxHeight, maxWidth, &mySnake, myPortal, prevMove);


			wattron(mainStageWin, COLOR_PAIR(21));
    		    mvwprintw(mainStageWin, mySnake.mySnakeCoordinate.row , mySnake.mySnakeCoordinate.column, "%c", mySnake.snakeHeadShape);
			wattroff(mainStageWin, COLOR_PAIR(21));

			mySnake.moveNum++;
			if (checkCollision(&mySnake, myScreenCoordinate, mapCoordinate, numOfWalls) == LOST || myPoisonousFruit.terminationCounter >= 10000){

				wattron(mainStageWin, COLOR_PAIR(5));
				mvwprintw(mainStageWin, mySnake.mySnakeCoordinate.row , mySnake.mySnakeCoordinate.column, "%c", mySnake.snakeHeadShape);
				wattroff(mainStageWin, COLOR_PAIR(5)); 

				wrefresh(mainStageWin);
				sleep(1);
				break;
			}

		    myScreenCoordinate[mySnake.mySnakeCoordinate.row ][mySnake.mySnakeCoordinate.column] = mySnake.moveNum;
		    
			mvwprintw(timeAndScoreWin, 5, 1,"%d", player1.score);
			mvwprintw(timeAndScoreWin, 8 , 1, "%s%d:%s%d", (timeCounter / 1000 )/60 >= 10 ? "": "0", (timeCounter /1000)/ 60,(timeCounter / 1000 )%60 >= 10 ? "": "0", (timeCounter/1000) % 60);


			if (myPoisonousFruit.terminationCounter != 0){
				wattron(timeAndScoreWin, COLOR_PAIR(9) );
				mvwprintw(timeAndScoreWin, 13 , 1, "GAME'LL END IN:");
				mvwprintw(timeAndScoreWin, 14 , 1, "  ");
				mvwprintw(timeAndScoreWin, 14 , 1, "%d",10 - (myPoisonousFruit.terminationCounter/1000) % 60);
				wattroff(timeAndScoreWin, COLOR_PAIR(9) );
			}

			wrefresh(mainStageWin);
			wrefresh(timeAndScoreWin);
			
		}
	
		if(lastKeyPressed == 'q' || lastKeyPressed == 'Q')
			switch( menu(1) ){
				case 0:
					wbkgd(mainStageWin, COLOR_PAIR(3));
					wrefresh(mainStageWin);
					lastKeyPressed = RIGHT_DIR;
					break;
				case 2:
					endwin();
					return;
			}

		movementCounter++;
		timeCounter++;

		if(myPoisonousFruit.terminationCounter != 0)
			myPoisonousFruit.terminationCounter++;
	
			
		myPoisonousFruit.changeCounter++;
		if(myPoisonousFruit.changeCounter == 5000){
			myPoisonousFruit.changeCounter = 0;
			myPoisonousFruit.pFruitExistenceStatus = NO;
		}

		myInefficaciousFruit.changeCounter++;
		if(myInefficaciousFruit.changeCounter == 2000){
			myInefficaciousFruit.changeCounter = 0;
			myInefficaciousFruit.iFruitExistenceStatus = NO;
		}
	

		//increasing snake's speed for ten second and then bringing it back to normal
		if(mySnake.snakeTurbo == ON){
			(myHotFruit.changeCounter)++;
		}

		if( myHotFruit.changeCounter == 10000 ){
			mySnake.snakeTurbo = OFF;
			myHotFruit.changeCounter = 0;
		}

	


		//producing a hot fruit every 15 second and removing it after 5 second
		if( timeCounter % 15000 == 0){
			generateHotFruit(&myHotFruit, maxHeight, maxWidth, mySnake, myScreenCoordinate, myPoisonousFruit , myInefficaciousFruit, mySeedCoordinate, mapCoordinate, numOfWalls, myPortal);	
		}

		if(myHotFruit.hFruitExistenceStatus == YES)
			myHotFruit.removingCounter++;

		if(myHotFruit.removingCounter == 5000){
			myHotFruit.removingCounter = 0;
			myHotFruit.hFruitExistenceStatus = NO;
			mvwprintw(mainStageWin, myHotFruit.row, myHotFruit.column, " ");
		}



        usleep(DELAY);
	}
	keypad(mainStageWin, TRUE);		
	mvwprintw(mainStageWin,maxHeight / 2, maxWidth / 2 - 10,"Game Over, Press any Key to continue...");
	wgetch(mainStageWin);
	addTopScore(player1);
    endwin();
    return 0;
}

	

