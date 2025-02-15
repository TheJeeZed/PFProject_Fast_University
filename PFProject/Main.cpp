// this is the start of programming fundamental semester project.
// Muhammad Shahmeer(24F-0684)
// Sobaan Ahmed(24F-0564)
// Hafiza Arfa (24F-0721)

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <iomanip>

using namespace std;

//directional constant for the arrow keys
const char UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77;
//the size of map and stuff like bots and highscores
const int ROW = 25, COL = 25, SCORES = 10, BOTS = 6;
//current location of the player
struct Bots {
	int row, col, lrow, lcol;
}bots[BOTS];
int Crow, Ccol;
int Brow, Bcol;
int Bulletrow[2], Bulletcol[2];
int score,botcount,lives,shots;
bool isspecialbullet = false;
string name = "";
int highscores[SCORES] = { 0 };
char map[ROW][COL];
string names[SCORES] = {"NA","NA","NA","NA","NA","NA","NA","NA","NA","NA"};
//name input for highscores
string nameInput() {
	string name = "";
	cout << "Enter your name(8 character long with no spaces):";
	cin >> name;
	if (name.size() > 8) {
		return name.substr(0, 8);
	}
	return name;
}
//prints the score in a systematic pattern
void printScores()
{	
	cout << "Names" << setw(16) << "Scores" << endl;
	for (int i = 0; i < SCORES; i++)
		cout << names[i] << setw(16 - names[i].size()) << highscores[i] << endl;
}
//Put Scores back to the text file.
bool putScores() {
	ofstream fout;
	fout.open("highscores.txt", ios::out);
	if (fout.fail()) {
		return false;
	}
	for (int i = 0; i < SCORES; i++) {
		fout << names[i];
		fout << " ";
		fout << highscores[i];
		fout << " ";
	}
	fout.close();
	return true;
}
//Read Scores from the text file.
bool getScores() {
	ifstream fin;
	fin.open("highscores.txt", ios::in);
	if (fin.fail()) {
		return false;
	}
	for (int i = 0; i < SCORES ;i++) {
		fin >> names[i];
		fin >> highscores[i];
	}
	fin.close();
	return true;
}
//Update the score keeping the relative position same
bool updateScores() {
	if (score > highscores[SCORES - 1]) {
		int insertindex = SCORES - 1;
		for (int i = 0; i < SCORES; i++) {
			if (score > highscores[i]) {
				insertindex = i;
				break;
			}
		}
		for (int i = SCORES - 2; i >= insertindex ; i--) {
			highscores[i + 1] = highscores[i];
			names[i + 1] = names[i];
		}
		highscores[insertindex] = score;
		names[insertindex] = name;
		return true;
	}
	else {
		return false;
	}
}
//Prints the entire map
void printMap() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			cout << map[i][j];
		}
		cout << endl;
	}
}
//places the character with respect to its body
void placeCharacter() {
	map[Crow][Ccol] = '|';
	map[Crow - 1][Ccol] = 'O';
	map[Crow][Ccol - 1] = '/';
	map[Crow + 1][Ccol - 1] = '/';
	map[Crow][Ccol + 1] = '\\';
	map[Crow + 1][Ccol + 1] = '\\';
}
//places soloman Lane! the boss of the game
void placeBoss(int row,int col) {
	map[row - 1][col] = ' ';
	map[row][col] = 'X';
	map[row + 1][col] = ' ';
	map[row - 1][col + 1] = ']';
	map[row][col + 1] = ']';
	map[row + 1][col + 1] = ']';
	map[row - 1][col - 1] = '[';
	map[row][col - 1] = '[';
	map[row + 1][col - 1] = '[';
	map[row][col + 2] = '[';
	map[row][col + 3] = ']';
	map[row][col - 2] = ']';
	map[row][col - 3] = '[';
}
//places the Robots
void placeBot(int row, int col) {
	map[row][col] = 'V';
	map[row - 1][col] = '|';
	map[row][col - 1] = '[';
	map[row - 1][col - 1] = '[';
	map[row][col + 1] = ']';
	map[row - 1][col + 1] = ']';
}
//validates and places an obstacle on the map
void placeObstacle(int width,int count)
{
	bool isvalid;
	int row, col;
	for (int k = 1; k <= count; k++) {
		do { // validating check. rerolling until a valid location is found
			row = 1 + rand() % (ROW - 2);
			col = 1 + rand() % (COL - 10);
			isvalid = true;
			for (int i = col; i <= col + width + 1; i++) {
				if (map[row][i] != ' ') {
					isvalid = false;
					break;
				}
			}
		} while (!isvalid);
		//placing the obstacle after validating logic! 
		map[row][col] = '[';
		map[row][col + width + 1] = ']';
		for (int i = col + 1; i <= col + width; i++) {
			map[row][i] = '-';
		}
	}
}
//summon bots takes in the number of bots to be summoned as parameter
void summonBot(int count) {
	// Seed the random number generator
	bool isvalid;
	for (int k = 0; k < count; k++) {
		do{
			bots[k].row = 1 + (rand() % (ROW / 2));
			bots[k].col = 2 + (rand() % (COL - 1));
			isvalid = true;
			for (int i = bots[k].row - 2; i <= bots[k].row + 1; i++) {
				for (int j = bots[k].col - 2; j <= bots[k].col + 2; j++) {
					if (map[i][j] != ' ') {
						isvalid = false;
						break;
					}
				}
			}
		}while (!isvalid);
		placeBot(bots[k].row, bots[k].col);
	}
}
//add initial items to the level according to level number
void addItems(int level) {
	lives = 3;//setup lives at the start of each level
	Bulletrow[1] = -1;
	Bulletrow[0] = -1;
	bool isvalid = false;
	//for placing the outside walls 
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (i == 0 || i == COL - 1) map[i][j] = '-';
			else if (j == 0 || j == ROW - 1) map[i][j] = '|';
			else map[i][j] = ' ';
		}
	}
	Brow = 3;
	Bcol = 5;
	placeBoss(Brow, Bcol);
	//for placing obstacles and bots according to level.
	switch (level) {
	case 1:
		placeObstacle(3, 2);
		summonBot(2);
		botcount = 2;
		break;
	case 2:
		placeObstacle(3, 3);
		summonBot(3);
		botcount = 3;
		break;
	case 3:
		placeObstacle(4, 3);
		summonBot(4);
		botcount = 4;
		break;
	case 4:
		placeObstacle(4, 4);
		summonBot(5);
		botcount = 5;
		break;
	case 5:
		placeObstacle(5, 4);
		summonBot(6);
		botcount = 6;
		break;
	}
	for (int i = 0; i < botcount; i++) {
		bots[i].lrow = -1;
	}
	//validating player spawn location
	do {
		Crow = (rand() % (ROW / 2 - 2)) + ROW / 2;
		Ccol = (rand() % (COL - 3) + 1) + 1;
		isvalid = true;
		for (int i = Crow - 1; i <= Crow + 1; i++) {
			for (int j = Ccol - 1; j <= Ccol + 1; j++) {
				if (map[i][j] != ' ')
					isvalid = false;
			}
		}
	}while(!isvalid);
	placeCharacter();//placing the player
}
//handles all the inputs entered by the user while playing the game
void eventHandler(char &action) {
	switch (action) {
	case 'E':
	case 'e':
		if (shots + 10 <= score) {
			shots += 10;
			isspecialbullet = true;
		}
		break;
	case 'W':
	case 'w':
		if (map[Crow - 2][Ccol] == ' ' && Bulletrow[0] == -1) {
			if (isspecialbullet) {
				map[Crow - 2][Ccol] = 'o';
				Bulletrow[1] = Crow - 2;
				Bulletcol[1] = Ccol;
				isspecialbullet = false;
			}
			else {
				map[Crow - 2][Ccol] = '^';
				Bulletrow[0] = Crow - 2;
				Bulletcol[0] = Ccol;
			}
		}
		break;
	case 'S':
	case 's':
		if (map[Crow + 2][Ccol] == ' ' && Bulletrow[0] == -1) {
			if (isspecialbullet) {
				map[Crow + 2][Ccol] = 'o';
				Bulletrow[1] = Crow + 2;
				Bulletcol[1] = Ccol;
				isspecialbullet = false;
			}
			else {
				map[Crow + 2][Ccol] = 'v';
				Bulletrow[0] = Crow + 2;
				Bulletcol[0] = Ccol;
			}
		}
		break;
	case 'A':
	case 'a':
		if (map[Crow][Ccol - 2] == ' ' && Bulletrow[0] == -1) {
			if (isspecialbullet) {
				map[Crow][Ccol - 2] = 'o';
				Bulletrow[1] = Crow;
				Bulletcol[1] = Ccol - 2;
				isspecialbullet = false;
			}
			else {
				map[Crow][Ccol - 2] = '<';
				Bulletrow[0] = Crow;
				Bulletcol[0] = Ccol - 2;
			}
		}
		break;
	case 'D':
	case 'd':
		if (map[Crow][Ccol + 2] == ' ' && Bulletrow[0] == -1) {
			if (isspecialbullet) {
				map[Crow][Ccol + 2] = 'o';
				Bulletrow[1] = Crow;
				Bulletcol[1] = Ccol + 2;
				isspecialbullet = false;
			}
			else {
				map[Crow][Ccol + 2] = '>';
				Bulletrow[0] = Crow;
				Bulletcol[0] = Ccol + 2;
			}
		}
		break;
	case UP: 
		if (map[Crow - 2][Ccol - 1] == ' ' && map[Crow - 2][Ccol] == ' ' && map[Crow - 2][Ccol + 1] == ' ') {
			map[Crow + 1][Ccol - 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow][Ccol] = ' ';
			Crow--;
			placeCharacter();
		}
		break;
	case DOWN:
		if (map[Crow + 2][Ccol - 1] == ' ' && map[Crow + 2][Ccol] == ' ' && map[Crow + 2][Ccol + 1] == ' ') {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol - 1] = ' ';
			map[Crow][Ccol + 1] = ' ';
			Crow++;
			placeCharacter();
		}
		break;
	case LEFT:
		if (map[Crow - 1][Ccol - 2] == ' ' && map[Crow][Ccol - 2] == ' ' && map[Crow + 1][Ccol - 2] == ' ') {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol + 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow + 1][Ccol - 1] = ' ';
			Ccol--;
			placeCharacter();
		}
		break;
	case RIGHT:
		if (map[Crow - 1][Ccol + 2] == ' ' && map[Crow][Ccol + 2] == ' ' && map[Crow + 1][Ccol + 2] == ' ') {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol - 1] = ' ';
			map[Crow + 1][Ccol - 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			Ccol++;
			placeCharacter();
			break;
		}
	}
	action = '0';
}
//function to kill the bot. returns true if a bot has been killed.
bool killBot(char bulletdirection, int row, int col) {
	bool iskilled = false;
	int botnumber;
	for (botnumber = 0; botnumber < botcount; botnumber++) {
		if ((bulletdirection == UP && row == bots[botnumber].row && (bots[botnumber].col >= col - 1 && bots[botnumber].col <= col + 1)) ||
			(bulletdirection == DOWN && row + 1 == bots[botnumber].row && (bots[botnumber].col >= col - 1 && bots[botnumber].col <= col + 1)) ||
			(bulletdirection == LEFT && col - 1 == bots[botnumber].col && (bots[botnumber].row >= row && bots[botnumber].row <= row + 1)) ||
			(bulletdirection == RIGHT && col + 1 == bots[botnumber].col && (bots[botnumber].row >= row && bots[botnumber].row <= row + 1))) {
			iskilled = true;
			break;
		}
	}
	if (iskilled) {
		for (int i = bots[botnumber].col - 1; i <= bots[botnumber].col + 1; i++) {
			map[bots[botnumber].row][i] = ' ';
			map[bots[botnumber].row - 1][i] = ' ';
		}
		score += 2;
		botcount--;
		map[bots[botnumber].lrow][bots[botnumber].lcol] = ' ';
		for (int i = botnumber; i < botcount; i++) {
			bots[i].row = bots[i + 1].row;
			bots[i].col = bots[i + 1].col;
			bots[i].lrow = bots[i + 1].lrow;
			bots[i].lcol = bots[i + 1].lcol;
		}
	}
	return iskilled;
}
//sets target for the special bullet
void moveSpecialbullet() {
	if (Bulletrow[1] == -1)
		return;
	int rowdiff, coldiff;
	int min = INT_MAX;
	int targetbot = 0;
	for (int i = 0; i < botcount; i++) {
		rowdiff = bots[i].row - Bulletrow[1];
		coldiff = bots[i].col - Bulletcol[1];
		if (rowdiff * rowdiff + coldiff * coldiff < min) {
			min = rowdiff * rowdiff + coldiff * coldiff;
			targetbot = i;
		}
	}
	bool iskilled = false;
	rowdiff = bots[targetbot].row - Bulletrow[1];
	coldiff = bots[targetbot].col - Bulletcol[1];
	if (rowdiff > 0) {
		map[Bulletrow[1]][Bulletcol[1]] = ' ';
		Bulletrow[1]++;
		iskilled = killBot(DOWN, Bulletrow[1], Bulletcol[1]);
		if (map[Bulletrow[1]][Bulletcol[1]] == ' ' && !iskilled)
			map[Bulletrow[1]][Bulletcol[1]] = 'o';
		else
			Bulletrow[1] = -1;
	}
	else if (rowdiff < 0) {
		map[Bulletrow[1]][Bulletcol[1]] = ' ';
		Bulletrow[1]--;
		iskilled = killBot(UP, Bulletrow[1], Bulletcol[1]);
		if (map[Bulletrow[1]][Bulletcol[1]] == ' ' && !iskilled)
			map[Bulletrow[1]][Bulletcol[1]] = 'o';
		else
			Bulletrow[1] = -1;
	}
	if (coldiff > 0) {
		map[Bulletrow[1]][Bulletcol[1]] = ' ';
		Bulletcol[1]++;
		iskilled = killBot(RIGHT, Bulletrow[1], Bulletcol[1]);
		if(map[Bulletrow[1]][Bulletcol[1]] == ' ' && !iskilled)
			map[Bulletrow[1]][Bulletcol[1]] = 'o';
		else
			Bulletrow[1] = -1;
	}
	else if (coldiff < 0) {
		map[Bulletrow[1]][Bulletcol[1]] = ' ';
		Bulletcol[1]--;
		iskilled = killBot(LEFT, Bulletrow[1], Bulletcol[1]);
		if (map[Bulletrow[1]][Bulletcol[1]] == ' ' && !iskilled)
			map[Bulletrow[1]][Bulletcol[1]] = 'o';
		else
			Bulletrow[1] = -1;
	}
}
//summons botslaser 
void summonLaser() {
	int rowdiff, coldiff;
	for (int i = 0; i < botcount; i++) {
		if (bots[i].lrow == -1) {
			rowdiff = Crow - bots[i].row;
			coldiff = Ccol - bots[i].col;
			while (rowdiff && coldiff) {
				(rowdiff > 0) ? rowdiff-- : rowdiff++;
				(coldiff > 0) ? coldiff-- : coldiff++;
			}
			if (coldiff < 0 && map[bots[i].row][bots[i].col - 2] == ' ') {
				map[bots[i].row][bots[i].col - 2] = '<';
				bots[i].lrow = bots[i].row;
				bots[i].lcol = bots[i].col - 2;
			}
			else if (coldiff > 0 && map[bots[i].row][bots[i].col + 2] == ' ') {
				map[bots[i].row][bots[i].col + 2] = '>';
				bots[i].lrow = bots[i].row;
				bots[i].lcol = bots[i].col + 2;
			}
			else if (rowdiff < 0 && map[bots[i].row - 2][bots[i].col] == ' ') {
				map[bots[i].row - 2][bots[i].col] = '^';
				bots[i].lrow = bots[i].row - 2;
				bots[i].lcol = bots[i].col;
			}
			else if (rowdiff > 0 && map[bots[i].row + 1][bots[i].col] == ' ') {
				map[bots[i].row + 1][bots[i].col] = 'v';
				bots[i].lrow = bots[i].row + 1;
				bots[i].lcol = bots[i].col;
			}
		}
	}
}
//damages player
bool damagePlayer(int row,int col) {
	if (map[row][col] == '/' || map[row][col] == '\\' || map[row][col] == 'O' || map[row + 1][col] == 'O') {
		lives--;
		return true;
	}
	return false;
}
//moves the boss
void moveBoss() {
	bool isvalidmove = false;
	int direction;
	while (!isvalidmove) {
		direction = (rand() % 4);
		switch (direction) {
		case 0:
			isvalidmove = true;
			for (int i = Bcol - 3; i <= Bcol + 3; i++) {
				if (killBot(UP, Brow - 2, i))
					score -= 2;
			}
			damagePlayer(Brow - 2, Bcol);
			for (int i = Bcol - 3; i <= Bcol + 3; i++) {
				if (map[Brow - 2][i] != ' ') {
					isvalidmove = false;
					break;
				}
			}
			if (isvalidmove) {
				for (int i = Bcol - 3; i <= Bcol + 3; i++) {
					map[Brow + 1][i] = ' ';
				}
				map[Brow][Bcol - 3] = ' ';
				map[Brow][Bcol - 2] = ' ';
				map[Brow][Bcol + 2] = ' ';
				map[Brow][Bcol + 3] = ' ';
				Brow--;
				placeBoss(Brow, Bcol);
			}
			break;
		case 1:
			isvalidmove = true;
			for (int i = Bcol - 3; i <= Bcol + 3; i++) {
				if (killBot(DOWN,Brow+2,i))
					score -= 2;
			}
			damagePlayer(Brow + 2, Bcol);
			for (int i = Bcol - 3; i <= Bcol + 3; i++) {
				if (map[Brow + 2][i] != ' ') {
					isvalidmove = false;
					break;
				}
			}
			if (isvalidmove) {
				for (int i = Bcol - 3; i <= Bcol + 3; i++) {
					map[Brow - 1][i] = ' ';
				}
				map[Brow][Bcol - 3] = ' ';
				map[Brow][Bcol - 2] = ' ';
				map[Brow][Bcol + 2] = ' ';
				map[Brow][Bcol + 3] = ' ';
				Brow++;
				placeBoss(Brow, Bcol);
			}
			break;
		case 2:
			isvalidmove = true;
			for (int i = Brow - 1; i <= Brow + 1; i++) {
				if (killBot(LEFT, i, Bcol - 4))
					score -= 2;
			}
			damagePlayer(Brow, Bcol - 4);
			for (int i = Brow - 1; i <= Brow + 1; i++) {
				if (map[i][Bcol - 4] != ' ') {
					isvalidmove = false;
					break;
				}
			}
			if (isvalidmove) {
				for (int i = Brow - 1; i <= Brow + 1; i++) {
					map[i][Bcol + 3] = ' ';
				}
				map[Brow - 1][Bcol + 1] = ' ';
				map[Brow + 1][Bcol + 1] = ' ';
				Bcol--;
				placeBoss(Brow, Bcol);
			}
			break;
		case 3:
			for (int i = Brow - 1; i <= Brow + 1; i++) {
				if (killBot(RIGHT, i, Bcol + 4))
					score -= 2;	
			}
			damagePlayer(Brow, Bcol + 4);
			isvalidmove = true;
			for (int i = Brow - 1; i <= Brow + 1; i++) {
				if (map[i][Bcol + 4] != ' ') {
					isvalidmove = false;
					break;
				}
			}
			if (isvalidmove) {
				for (int i = Brow - 1; i <= Brow + 1; i++) {
					map[i][Bcol - 3] = ' ';
				}
				map[Brow - 1][Bcol - 1] = ' ';
				map[Brow + 1][Bcol - 1] = ' ';
				Bcol++;
				placeBoss(Brow, Bcol);
				break;
			}
		}
	}
}
//moves the bots
void moveBots() {
	bool isvalidmove = false;
	int rowdiff, coldiff;
	for (int i = 0; i < botcount; i++) {
		if (map[bots[i].row][bots[i].col] == 'V') {
			rowdiff = Crow - bots[i].row;
			coldiff = Ccol - bots[i].col;
			while (rowdiff && coldiff) {
				(rowdiff > 0) ? rowdiff-- : rowdiff++;
				(coldiff > 0) ? coldiff-- : coldiff++;
			}
			if (coldiff) {
				if (coldiff < 0) {
					damagePlayer(bots[i].row, bots[i].col - 2);
					if (map[bots[i].row - 1][bots[i].col - 2] == ' ' && map[bots[i].row][bots[i].col - 2] == ' ') {
						map[bots[i].row - 1][bots[i].col + 1] = ' ';
						map[bots[i].row][bots[i].col + 1] = ' ';
						bots[i].col--;
						placeBot(bots[i].row, bots[i].col);
					}
				}
				else if (coldiff > 0){
					damagePlayer(bots[i].row, bots[i].col + 2);
					if (map[bots[i].row - 1][bots[i].col + 2] == ' ' && map[bots[i].row][bots[i].col + 2] == ' ') {
						map[bots[i].row - 1][bots[i].col - 1] = ' ';
						map[bots[i].row][bots[i].col - 1] = ' ';
						bots[i].col++;
						placeBot(bots[i].row, bots[i].col);
					}
				}
			}
			else if (rowdiff) {
				if (rowdiff < 0){
					damagePlayer(bots[i].row - 2, bots[i].col);
					if (map[bots[i].row - 2][bots[i].col - 1] == ' ' && map[bots[i].row - 2][bots[i].col] == ' ' && map[bots[i].row - 2][bots[i].col + 1] == ' ') {
						map[bots[i].row][bots[i].col - 1] = ' ';
						map[bots[i].row][bots[i].col] = ' ';
						map[bots[i].row][bots[i].col + 1] = ' ';
						bots[i].row--;
						placeBot(bots[i].row, bots[i].col);
					}
				}
				else if (rowdiff > 0){
					damagePlayer(bots[i].row + 1, bots[i].col);
					if (map[bots[i].row + 1][bots[i].col - 1] == ' ' && map[bots[i].row + 1][bots[i].col] == ' ' && map[bots[i].row + 1][bots[i].col + 1] == ' ') {
						map[bots[i].row - 1][bots[i].col - 1] = ' ';
						map[bots[i].row - 1][bots[i].col] = ' ';
						map[bots[i].row - 1][bots[i].col + 1] = ' ';
						bots[i].row++;
						placeBot(bots[i].row, bots[i].col);
					}
				}
			}
		}
	}
}
//opens the door
bool openDoor() {
	int direction;
	if (!botcount) {
		direction = rand() % 2;
		switch (direction) {
		case 0:
			for (int i = COL / 2 - 2; i <= COL / 2 + 2; i++) {
				map[0][i] = ' ';
			}
			return true;
		case 1:
			for (int i = ROW / 2 - 2; i <= ROW / 2 + 2; i++) {
				map[i][COL - 1] = ' ';
			}
			return true;
		}
	}
	return false;
}
//moves the bullet per update
void moveBullets()	 {
	bool iskilled = false;
	for (int i = 0; i < botcount; i++) {
		if (bots[i].lrow != -1) {
			if (map[bots[i].lrow][bots[i].col] == '^') {
				map[bots[i].lrow][bots[i].lcol] = ' ';
				bots[i].lrow--;
				if (!damagePlayer(bots[i].lrow, bots[i].lcol) && map[bots[i].lrow][bots[i].lcol] == ' ')
					map[bots[i].lrow][bots[i].lcol] = '^';
				else
					bots[i].lrow = -1;
			}
			else if (map[bots[i].lrow][bots[i].lcol] == 'v') {
				map[bots[i].lrow][bots[i].lcol] = ' ';
				bots[i].lrow++;
				if (!damagePlayer(bots[i].lrow, bots[i].lcol) && map[bots[i].lrow][bots[i].lcol] == ' ')
					map[bots[i].lrow][bots[i].lcol] = 'v';
				else
					bots[i].lrow = -1;
			}
			else if (map[bots[i].lrow][bots[i].lcol] == '<') {
				map[bots[i].lrow][bots[i].lcol] = ' ';
				bots[i].lcol--;
				if (!damagePlayer(bots[i].lrow, bots[i].lcol) && map[bots[i].lrow][bots[i].lcol] == ' ')
					map[bots[i].lrow][bots[i].lcol] = '<';
				else
					bots[i].lrow = -1;
			}
			else if (map[bots[i].lrow][bots[i].lcol] == '>') {
				map[bots[i].lrow][bots[i].lcol] = ' ';
				bots[i].lcol++;
				if (!damagePlayer(bots[i].lrow, bots[i].lcol) && map[bots[i].lrow][bots[i].lcol] == ' ')
					map[bots[i].lrow][bots[i].lcol] = '>';
				else
					bots[i].lrow = -1;
			}
		}
	}
	if (Bulletrow[0] != -1) {
		if (map[Bulletrow[0]][Bulletcol[0]] == '^') {
			map[Bulletrow[0]][Bulletcol[0]] = ' ';
			Bulletrow[0]--;
			if (!killBot(UP, Bulletrow[0], Bulletcol[0]) && map[Bulletrow[0]][Bulletcol[0]] == ' ')
				map[Bulletrow[0]][Bulletcol[0]] = '^';
			else
				Bulletrow[0] = -1;
		}
		else if (map[Bulletrow[0]][Bulletcol[0]] == 'v') {
			map[Bulletrow[0]][Bulletcol[0]] = ' ';
			Bulletrow[0]++;
			if (!killBot(DOWN, Bulletrow[0], Bulletcol[0]) && map[Bulletrow[0]][Bulletcol[0]] == ' ')
				map[Bulletrow[0]][Bulletcol[0]] = 'v';
			else
				Bulletrow[0] = -1;
		}
		else if (map[Bulletrow[0]][Bulletcol[0]] == '<') {
			map[Bulletrow[0]][Bulletcol[0]] = ' ';
			Bulletcol[0]--;
			if (!killBot(LEFT, Bulletrow[0], Bulletcol[0]) && map[Bulletrow[0]][Bulletcol[0]] == ' ')
				map[Bulletrow[0]][Bulletcol[0]] = '<';
			else
				Bulletrow[0] = -1;
		}
		else if (map[Bulletrow[0]][Bulletcol[0]] == '>') {
			map[Bulletrow[0]][Bulletcol[0]] = ' ';
			Bulletcol[0]++;
			if (!killBot(RIGHT,Bulletrow[0], Bulletcol[0]) && map[Bulletrow[0]][Bulletcol[0]] == ' ')
				map[Bulletrow[0]][Bulletcol[0]] = '>';
			else
				Bulletrow[0] = -1;
		}
	}
}
//level win check
bool isWin(bool isopen) {
	if (isopen) {
		for (int i = COL / 2 - 2; i <= COL / 2 + 2; i++) {
			if (map[0][i] == 'O') {
				score += 5;
				return true;
			}
		}
		for (int i = ROW / 2 - 2; i <= ROW / 2 + 2; i++) {
			if (map[i][COL - 1] == '\\') {
				score += 5;
				return true;
			}
		}
	}
	return false;
}
//runs the entire game
void game() {
	system("cls");
	score = 0;
	shots = 0;
	char Input;
	bool isopen,hasclearedlevels = true;
	int botmovetick = 0, scoretick = 0, bossmovetick = 0;
	for (int i = 1; i <= 5; i++) {
		isopen = false;
		addItems(i);
		while (lives > 0 && !isWin(isopen)) {
			cout << lives << setw(20) << score << endl;
			printMap();
			if (_kbhit())
				Input = _getch();
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });//used to put the curser at the start of map. better then cls to avoid jittering
			eventHandler(Input);
			bossmovetick++;
			bossmovetick %= static_cast<int>(2 / pow(1.25, i));
			if(!bossmovetick)
				//moveBoss();
			botmovetick++;
			botmovetick %= static_cast<int>(5 / pow(1.1,i));
			if(!botmovetick)
				moveBots();
			moveSpecialbullet();
			moveBullets();
			summonLaser();
			if (!isopen) {
				isopen = openDoor();
			}
			Sleep(100);
			scoretick++;
			scoretick %= 100;
			if (!scoretick) {
				score++;
			}
		}
		if (lives <= 0) {
			hasclearedlevels= false;
			system("cls");
			cout << "YOU LOST MAYBE TRY AGAIN" << endl;
			system("pause");
			break;
		}
	}
	if (hasclearedlevels) {
		system("cls");
		cout << "YOU WON!" << endl;
		system("pause");
	}
	updateScores();
	putScores();
}
int main() {
	system("Color 06");
	srand(time(0));
	getScores();
	name = nameInput();
	int choice;
	do {
		cout << "Enter your choice \n1. Boot Game\n2. Show Highscore\n3. Exit\n";
		cin >> choice;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "the Input must be an integer";
			cin >> choice;
		}
		switch (choice) {
		case 1:
			game();
			break;
		case 2:
			system("cls");
			printScores();
			break;
		case 3:break;
		case 4:cout << "INVALID CHOICE" << endl;
		}
	} while (choice != 3);
	return 0;
}