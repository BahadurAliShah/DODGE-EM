//============================================================================
// Name        : game.cpp
// Author      : Hassan Mustafa
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Dodge 'Em...
//============================================================================

#ifndef DODGE_CPP_
#define DODGE_CPP_
#include "util.h"
#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

int score=0,randomMover,level=1,lives=3,lineSpacing=75;
float opponentCarX[2], opponentCarY[2],speedOfOpponentX[2],speedOfOpponentY[2],ownCarX,ownCarY,speedOfOwnX,speedOfOwnY;
int pointsX[8][8],pointsY[8][8],scores[10]={0,0,0,0,0,0,0,0,0,0};
bool startGame=0,help=0,points=false,parametersReset=0,out=true,gameOver=false,mainmenu=true,escape=false,spacePressed=false,youWon=false,scoresDisplay=false,levelPassed =false;
bool points_display[8][8];



void movecar(int, int, int, int, int);
void ScoreStore(int);


// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


/*
 * Main Canvas drawing function.
 * */
//Board *b;
void GameDisplay()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	// calling some functions from util.cpp file to help students

	//Square at 400,20 position
	//DrawSquare( 400 , 20 ,40,colors[RED]); 
	//Square at 250,250 position
	//DrawSquare( 250 , 250 ,20,colors[GREEN]); 
	//Display Score
	//DrawString( 50, 800, "Score=0", colors[MISTY_ROSE]);
	//Triangle at 300, 450 position
	//DrawTriangle( 300, 450 , 340, 450 , 320 , 490, colors[MISTY_ROSE] ); 
	// Trianlge Vertices v1(300,50) , v2(500,50) , v3(400,250)
	//Circle at 50, 670 position
	//DrawCircle(50,670,10,colors[RED]);
	//Line from 550,50 to 550,480 with width 10
	//DrawLine(int x1, int y1, int x2, int y2, int lwidth, float *color)
	//DrawLine( 550 , 50 ,  550 , 480 , 10 , colors[MISTY_ROSE] );	



	//points reset
	if(!points){
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++)
				points_display[i][j]=true;
		points=true;
	}
	
	//parameters reset
	if(!parametersReset){
		opponentCarX[0]=470;
		opponentCarY[0]=45;
		opponentCarX[1]=470;
		opponentCarY[1]=630;
		speedOfOpponentX[0]=2;
		speedOfOpponentY[0]=0;
		ownCarX=330;
		ownCarY=45;
		speedOfOwnX=-2;
		speedOfOwnY=0;
		score=0;
		speedOfOpponentX[1]=-2;
		speedOfOpponentY[1]=0;
		parametersReset=true;
	}
	//out string
	if(!out){
		DrawRectangle(270,350,300,140,colors[RED]);
		DrawString(400,460,"OUT",colors[WHITE]);
		DrawString(283,400,"press R to restart",colors[BLACK]);
	}

	//MENU 
	float* color=colors[LIGHT_SEA_GREEN];
	if(!startGame){
		DrawRectangle(0,0,840,840,color);
		DrawString(365,600,"Start Game",colors[RED]);
		DrawString(360,500,"High Scores",colors[RED]);
		DrawString(397,400,"Help",colors[RED]);
		DrawString(397,300,"Exit",colors[RED]);
	}

	//mainmenu bool
	if(mainmenu){
		DrawRectangle(0,680,840,60,colors[WHITE]);
		DrawString(380,700,"MAINMENU",colors[BLACK]);
	}

	//Game Over bool
	if(gameOver){
		DrawRectangle(0,680,840,60,colors[WHITE]);
		DrawString(370,700,"Game Over",colors[BLACK]);
		lives=3;
		level=1;
		parametersReset=0;
	}

	//Pause
	if(escape){
		DrawRectangle(0,680,840,60,colors[WHITE]);
		DrawString(380,700,"PAUSE",colors[BLACK]);
		startGame=0;
		mainmenu=0;
		DrawString(380,200,"Continue",colors[RED]);
	}

	//Help
	if(help){
		DrawRectangle(20,20,800,800,colors[WHITE]);
		DrawString(410,770,"HELP",colors[BLACK]);
		DrawString(337,30,"press b to back",colors[BLACK]);
		DrawString(30,740,"The game is played in a racing arena where the player drives his brown colored",colors[BLACK]);
		DrawString(30,710," car to pick the maximum rewards, avoiding a head-on collision with the blue ",colors[BLACK]);
		DrawString(30,680,"opponent car(s) run by the computer. The player car moves clockwise while the",colors[BLACK]);
		DrawString(30,650,"opponent car(s) move counter-clockwise only. When the game starts, the player",colors[BLACK]);
		DrawString(30,620,"is awarded three lives to collect the rewards from the arena. The is awarded",colors[BLACK]);
		DrawString(30,590,"three lives to collect the rewards from the arena. Theplayer gets one reward",colors[BLACK]);
		DrawString(30,560," point when his/her car picks up any rectangular-shaped gift-boxes placed at",colors[BLACK]);
		DrawString(30,530,"equal distances in the arena. When the player car hits the opponent car, one",colors[BLACK]);
		DrawString(30,500," life is lost and the game arena is refilled with the reward gift-boxes. The",colors[BLACK]);
		DrawString(30,470,"score and the number of lives left are visible on the top of the screen",colors[BLACK]);
	}

	//Scores Display
	if(scoresDisplay){

		DrawRectangle(20,20,800,800,colors[WHITE]);

		ifstream file;
		file.open("Scores.txt");

		for(int i=0;i<10;i++){
			file>>scores[i];
		}
		file.close();

		for(int i=0;i<10;i++){
		ostringstream strScores;
		strScores << scores[i];
		string realScores = strScores.str();

		DrawString(410,770-i*50,realScores,colors[BLACK]);
		}
		DrawString(337,30,"press b to back",colors[BLACK]);
	}

	//Level Passed
	if(levelPassed){
		DrawRectangle(200,350,440,140,colors[BLUE]);
		DrawString(360,410,"Level Passed",colors[WHITE]);
		DrawString(250,360,"Press r for same level and n for next level",colors[BLACK]);
	}

	if(startGame&&out&&!escape&&!gameOver&&!levelPassed){

		//For escape menu
		DrawRectangle(0, 710, 840, 50, colors[WHITE]);
		DrawString(100,720,"Press P or p to escape and q to quit",colors[BLACK]);
		// Drawing Arena
		int gap_turn = 100;
		int sx = 20;
		int sy = 30;
		int swidth = 800/2 - gap_turn/2; // half width
		int sheight = 10;
		float *scolor = colors[BROWN];
		for(int i=0;i<4;i++)
		{
			DrawRectangle(i*75, sy+i*75, swidth-i*75+sx, sheight, scolor); // bottom left
			DrawRectangle(sx + swidth + gap_turn, sy+i*75, swidth-i*75+sx, sheight, scolor); // bottom right
			DrawRectangle(sx+swidth*2+gap_turn-i*75, sy+sheight+i*75, sheight*2, swidth/1.4-i*75+5, scolor); // right down
			DrawRectangle(sx+swidth*2+gap_turn-i*75, swidth+gap_turn-15, sheight*2, swidth/1.4-i*75+5, scolor); // right up
			DrawRectangle(sx + swidth + gap_turn, sy+660-i*75, swidth+sx-i*75, sheight, scolor); // top right
			DrawRectangle(i*75, sy+660-i*75, swidth+sx-i*75, sheight, scolor); // top left
			DrawRectangle(i*75, swidth+gap_turn-15, sheight*2, swidth/1.4-i*75+5, scolor); // left up
			DrawRectangle(i*75, sy+sheight+i*75, sheight*2, swidth/1.4-i*75+5, scolor); // left down


		
		}
		
		//For most inner matrix
		DrawRectangle(300, sy+360, 240, sheight, scolor); // top right
		DrawRectangle(300, 330, 240, sheight, scolor); // bottom left
		DrawRectangle(520, 339, sheight*2, 26, scolor); // right down
		DrawRectangle(520, 366, sheight*2, 25, scolor);
		DrawRectangle(300, 339, sheight*2, 26, scolor); // right down
		DrawRectangle(300, 366, sheight*2, 25, scolor);


		//points Display
		color=colors[LIGHT_STEEL_BLUE];

		for(int i=0;i<8;i++){
			
			for(int j=0;j<8;j++)
				{	
				
					j<4?
						pointsX[i][j] =sheight*2+75/2-15+(j)*75:
							pointsX[i][j] =swidth + gap_turn+75/2+(j-3)*75;
				
					i<4?
						pointsY[i][j] = sy+sheight+75/2-10+(i)*75:
							pointsY[i][j] = swidth+gap_turn-15+sy+sheight+75/2-10+(i-5)*75;
				
				}
			}

		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++)
			{
				if(points_display[i][j]){
					if(ownCarX+35>=pointsX[i][j] && pointsX[i][j]>=ownCarX-5 && ownCarY+40>=pointsY[i][j] && pointsY[i][j]>=ownCarY-6){
						points_display[i][j]=false;
						score++;
					}
				}
				if(points_display[i][j]){
					DrawRectangle(pointsX[i][j], pointsY[i][j], sheight, 5 , color);
				}
			}
		}


		// Drawing user car
		float x1 = ownCarX; 
		float y1 = ownCarY; 
		float width = 10; 
		float height = 10;
		color = colors[BLUE_VIOLET]; 
		float radius = 2.5;
		DrawRoundRect(x1,y1,width,height,color,radius); // bottom left tyre
		DrawRoundRect(x1+width*3,y1,width,height,color,radius); // bottom right tyre
		DrawRoundRect(x1+width*3,y1+height*4,width,height,color,radius); // top right tyre
		DrawRoundRect(x1,y1+height*4,width,height,color,radius); // top left tyre
		DrawRoundRect(x1, y1+height*2, width, height, color, radius/2); // body left rect
		DrawRoundRect(x1+width, y1+height, width*2, height*3, color, radius/2); // body center rect
		DrawRoundRect(x1+width*3, y1+height*2, width, height, color, radius/2); // body right rect

		// Drawing opponent car

		color = colors[YELLOW]; 
		for(int i=0;i<2;i++){
			DrawRoundRect(opponentCarX[i],opponentCarY[i],width,height,color,radius); // bottom left tyre
			DrawRoundRect(opponentCarX[i]+width*3,opponentCarY[i],width,height,color,radius); // bottom right tyre
			DrawRoundRect(opponentCarX[i]+width*3,opponentCarY[i]+height*4,width,height,color,radius); // top right tyre
			DrawRoundRect(opponentCarX[i],opponentCarY[i]+height*4,width,height,color,radius); // top left tyre
			DrawRoundRect(opponentCarX[i], opponentCarY[i]+height*2, width, height, color, radius/2); // body left rect
			DrawRoundRect(opponentCarX[i]+width, opponentCarY[i]+height, width*2, height*3, color, radius/2); // body center rect
			DrawRoundRect(opponentCarX[i]+width*3, opponentCarY[i]+height*2, width, height, color, radius/2); // body right rect
			if(level<4)
				break;
		}



		ostringstream strScore;
		strScore << score;
		string realScore = strScore.str();


		ostringstream strlevel;
		strlevel << level;
		string reallevel = strlevel.str();


		ostringstream strlives;
		strlives << lives;
		string reallives = strlives.str();


		movecar(sx, sy, swidth, sheight, gap_turn);
		DrawString(100,800,"Scores = ",colors[RED]);
		DrawString(200,800,realScore,colors[RED]);

		DrawString(400,800,"Level = ",colors[RED]);
		DrawString(500,800,reallevel,colors[RED]);

		DrawString(600,800,"Lives = ",colors[RED]);
		DrawString(700,800,reallives,colors[RED]);
	}

	//You Won
	if(youWon){
		DrawRectangle(0,0,840,840,colors[OLIVE]);
		DrawString(365,408,"YOU WON!!!",colors[BLACK]);
		DrawString(337,20,"press b to back",colors[BLACK]);
		lives=3;
		level=1;
		parametersReset=0;
	}


	glutSwapBuffers(); // do not modify this line.. or draw anything below this line
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {     
	if(ownCarY>=285&&ownCarY<=385)
	{
		if (key
				== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
			// what to do when left key is pressed...
			if(ownCarX>85)
				ownCarX-=75;
		} else if (key
				== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
			if(ownCarX<755&&(ownCarX<300||ownCarX>540))
				ownCarX+=75;
		}
	}

	if(ownCarX>=350&&ownCarX<=450)
	{
		if (key
				== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
			if(ownCarY<600)
				ownCarY+=75;
		}

		else if (key
				== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
			if(ownCarY>110)
				ownCarY-=75;
		}
	}


	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if(startGame){
		if(key=='P'||key=='p'){
			escape=true;
			startGame=false;
		}
	}

	if(levelPassed){
		if (key == 'r' || key == 'R'){
			level--;
			levelPassed=0;
		}
		if (key == 'n' || key == 'N')
			levelPassed=0;
	}

	if (key == 'q'||key == 'Q') {
		exit(1); // exit the program when q key is pressed.
	}

	if(escape){
		if(key=='b'||key==27||key=='B'){
			escape=false;
			startGame=true;
			scoresDisplay=false;
		}
	}

	if (key == 'b' || key == 'B')
			{
		//do something if b is pressed
		cout << "b pressed" << endl;
		help=0;
		scoresDisplay=false;
		if(youWon){
			startGame=0;
			youWon=0;
			mainmenu=true;
			parametersReset=0;
		}
	}

	if (key == 'r' || key == 'R')
			{
		parametersReset=0;
		out=true;
		points=0;

	}

	if(!startGame){
		if(key=='s'||key=='S'){
			startGame=1;
			escape=false;
			mainmenu=false;
			gameOver=false;
		}
	}

	if(key==32){
		ownCarX+=speedOfOwnX*2;
		ownCarY+=speedOfOwnY*2;
	}
	
	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	if(startGame&&!levelPassed){
		ownCarX+=speedOfOwnX;
		ownCarY+=speedOfOwnY;
		for(int i=0;i<2;i++){	
			opponentCarX[i]+=speedOfOpponentX[i];
			opponentCarY[i]+=speedOfOpponentY[i];
			if(level<4)
				break;
		}
		if(level==3){
			opponentCarX[0]=opponentCarX[0]+speedOfOpponentX[0]*2;
			opponentCarY[0]=opponentCarY[0]+speedOfOpponentY[0]*2;
		}
	}
	glutPostRedisplay();
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(10.0, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;		

	glutPostRedisplay();
}
void MouseMoved(int x, int y) {

	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	if(x>397&&x<440&&y>420&&y<450){
		help=true;
		
	}

	if(x>365&&x<475&&y<250&&y>220){
		startGame=1;
		mainmenu=false;
		gameOver=false;
	}
	if(x>397&&x<440&&y>530&&y<550)
		exit(1);

	if(x>360&&x<480&&y<350&&y>320){
		scoresDisplay=true;
	}

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{

	}

	if(escape){
		if(x>380&&x<480&&y>620&&y<650){
			startGame=1;
			escape=false;
		}
		if(x>365&&x<475&&y<250&&y>220){
			points=0;
			parametersReset=0;
			level=1;
			lives=3;
			startGame=1;
			escape=false;
		}
	}
	glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	//b = new Board(60, 60); // create a new board object to use in the Display Function ...

	int width = 840, height = 840; // i have set my window size to be 800 x 600
	//b->InitalizeBoard(width, height);
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("OOP Centipede"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* AsteroidS_CPP_ */

void movecar(int sx, int sy, int swidth, int sheight, int gap_turn)
{

//	Random movement
	for(int i=0;i<2;i++){
		if(level>1)
			if(opponentCarY[i]>=326&&opponentCarY[i]<=329)
			{
				if(opponentCarX[i]<sx+swidth*2+gap_turn-75&&opponentCarX[i]>75)
					randomMover=GetRandInRange(long(-1), long(2));
				else
					randomMover=GetRandInRange(long(0), long(2));
				if(opponentCarX[i]>sx+swidth+gap_turn*0.5){
					opponentCarX[i]=opponentCarX[i]-randomMover*75;
					opponentCarY[i]+=4;
				}
				else{
					opponentCarX[i]=opponentCarX[i]+randomMover*75;
					opponentCarY[i]-=4;
				}
			}
		if(opponentCarX[i]>=420&&opponentCarX[i]<=424)
		{
			if(opponentCarY[i]<sy+swidth*1.4+gap_turn-75&&opponentCarY[i]>sy+75)
				randomMover=GetRandInRange(long(-1), long(2));
			else
				randomMover=GetRandInRange(long(0), long(2));
			if(opponentCarY[i]>sy+gap_turn*0.5+swidth*0.7){
				opponentCarY[i]=opponentCarY[i]-randomMover*75;
				opponentCarX[i]-=4;
			}
			else{
				opponentCarY[i]=randomMover*75+opponentCarY[i];
				opponentCarX[i]+=4;
			}
		}

//	Opponent Car Checks
		if((opponentCarY[i]>sy+300)&&(opponentCarX[i]<sx+swidth*2+gap_turn-300)&&(opponentCarY[i]<=660-300)&&(opponentCarX[i]>sx+300))
			(opponentCarX[i]>300&&opponentCarX[i]<420)?opponentCarX[i]=299:opponentCarX[i]<540&&opponentCarX[i]>420?opponentCarX[i]=541:opponentCarY[i]>330&&opponentCarY[i]<360?opponentCarY[i]=329:opponentCarY[i]<390&&opponentCarY[i]>360?opponentCarY[i]=391:1;

		if(level<4)
			break;
}


//	User Car Checks
	for(int i=3;i>=0;i--){
		if(((ownCarX>=i*75)&&(ownCarY>=sy+i*75))&&((ownCarX<=840-i*75)&&(ownCarY<=sy+swidth*1.6+gap_turn+sheight-i*75))){
			if(ownCarY<=sy+15+i*75){
				speedOfOwnX=-2;
				speedOfOwnY=0;
				ownCarY+=4;}
			else if((ownCarY+i*75)>=(45+swidth*1.4+gap_turn)){
				speedOfOwnX=2;
				speedOfOwnY=0;
				ownCarY-=4;}
			else if(ownCarX<=sx+5+i*75){
				speedOfOwnX=0;
				speedOfOwnY=2;
				ownCarX+=4;}
			else if((ownCarX+45)>=(sx+swidth*2+gap_turn-i*75)){
				speedOfOwnX=0;
				speedOfOwnY= -2;
				ownCarX-=5;}
		}
	}

	for(int j=0;j<2;j++){
		for(int i=3;i>=0;i--){
			if((opponentCarY[j]>sy+i*75)&&(opponentCarX[j]<sx+swidth*2+gap_turn-i*75)&&(opponentCarY[j]<=660-i*75)&&(opponentCarX[j]>sx+i*75)){
				((opponentCarX[j]+47)>=(sx+swidth*2+gap_turn-i*75))?(speedOfOpponentX[j]=0,speedOfOpponentY[j]=2,opponentCarX[j]--):(opponentCarY[j])>=(45+swidth*1.4+gap_turn-i*75)?(speedOfOpponentX[j]=-2,speedOfOpponentY[j]=0,opponentCarY[j]--):(opponentCarX[j]<=sx+5+i*75?(speedOfOpponentX[j]=0,speedOfOpponentY[j]=-2,opponentCarX[j]++):(opponentCarY[j]<=sy+10+i*75?(speedOfOpponentX[j]=2,speedOfOpponentY[j]=0,opponentCarY[j]++):1));
			}
		}

//out
		if(opponentCarX[j]+20>=ownCarX-20 && opponentCarX[j]-20<=ownCarX+20 && opponentCarY[j]+20>=ownCarY-20 && opponentCarY[j]-20<=ownCarY+20)
		{
			ScoreStore(score);
			if(lives--){out = false;
				points=0;
				
			}else{
				startGame=0;
				gameOver=1;
			}
		}
		if(level<4)
			break;
	}

//LEVEL INcriment
	if(score==64){
		ScoreStore(score);
		level++;
		if (level==5)
			youWon=true;
		parametersReset=0;
		points=0;
		levelPassed=1;
	}
}

void ScoreStore(int score){
		ifstream file;
		file.open("Scores.txt");

		for(int i=0;i<10;i++){
			file>>scores[i];
		}
		file.close();
		ofstream file1;
		file1.open("Scores.txt");


		for(int i=0,j=0;i<10;i++){
			if(score>scores[i]){
				j=i;
				while(i<9){
					i++;
					scores[i]=scores[i-1];
				}
				scores[j]=score;
			}
		}
		for(int i=0;i<10;i++)
			file1<<scores[i]<<endl;
		file1.close();
}
