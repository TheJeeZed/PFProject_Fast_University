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

using namespace std;

//directional constant for the arrow keys
const char UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77;
//the size of map and number of highscores to store
const int ROW = 25, COL = 25, SCORES = 10;
//current location of the player
int Crow, Ccol;
int score = 0;
int botcount;
int lives;
string name = "";
int highscores[SCORES] = { 0 };
char map[ROW][COL],mapback[ROW][COL];
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
bool updateScores(int score,string name) {
	if (score > highscores[SCORES - 1]) {
		int insertindex = SCORES - 1;
		for (int i = insertindex - 1; i >= 0; i--) {
			if (score > highscores[i]) {
				insertindex = i;
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
	// Seed the random number generator
	srand(time(0));
	bool isvalid;
	int row, col;
	for (int i = 1; i <= count; i++) {
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
	srand(time(0));
	bool isvalid;
	int row, col;
	for (int i = 1; i <= count; i++) {
		do{
			row = 1 + (rand() % (ROW / 2));
			col = 2 + (rand() % (COL - 1));
			isvalid = true;
			for (int i = row; i <= row + 1; i++) {
				for (int j = col; j <= col + 2; j++) {
					if (map[i][j] != ' ') {
						isvalid = false;
						break;
					}
				}
			}
		}while (!isvalid);
		placeBot(row + 1, col + 1);
	}
}
//add initial items to the level according to level number
void addItems(int level) {
	lives = 3;//setup lives at the start of each level
	//fills the background with spaces to handle any unwanted character 
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			mapback[i][j] = ' ';
		}
	}
	bool isvalid = false;
	srand(time(0));
	//for placing the outside walls 
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (i == 0 || i == COL - 1) {
				map[i][j] = '-';
			}
			else if (j == 0 || j == ROW - 1) {
				map[i][j] = '|';
			}
			else {
				map[i][j] = ' ';
			} 
		}
	}
	//for placing obstacles and bots according to level.
	switch (level) {
	case 1:
		placeObstacle(3,2);
		summonBot(2);
		botcount = 2;
		break;
	case 2:
		placeObstacle(3,3);
		summonBot(3);
		botcount = 3;
		break;
	case 3:
		placeObstacle(4,3);
		summonBot(4);
		botcount = 4;
		break;
	case 4:
		placeObstacle(4,4);
		summonBot(5);
		botcount = 5;
		break;
	case 5:
		placeObstacle(5,4);
		summonBot(6);
		botcount = 6;
		break;
	}
	//validating player spawn location
	do {
		Crow = (rand() % (ROW / 2 - 2)) + ROW / 2;
		Ccol = (rand() % (COL - 3) + 1) + 1;
		isvalid = true;
		for (int i = Crow - 1; i <= Crow + 1; i++) {
			for (int j = Ccol - 1; j <= Ccol + 1; j++) {
				if (map[i][j] != ' ') {
					isvalid = false;
				}
			}
		}
	}while(!isvalid);
	placeCharacter();//placing the player
}
//handles all the inputs entered by the user while playing the game
void eventHandler(char action) {
	//check if the map has player bullet
	bool hasPlayerBullet = false;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (mapback[i][j] == 'F') {
				hasPlayerBullet = true;
			}
		}
	}
	switch (action) {
	case 'W':
	case 'w':
		if (map[Crow - 2][Ccol] == ' ' && !hasPlayerBullet) {
			map[Crow - 2][Ccol] = '^';
			mapback[Crow - 2][Ccol] = 'F';
		}
		break;
	case 'S':
	case 's':
		if (map[Crow + 2][Ccol] == ' ' && !hasPlayerBullet) {
			map[Crow + 2][Ccol] = 'v';
			mapback[Crow + 2][Ccol] = 'F';
		}
		break;
	case 'A':
	case 'a':
		if (map[Crow][Ccol - 2] == ' ' && !hasPlayerBullet) {
			map[Crow][Ccol - 2] = '<';
			mapback[Crow][Ccol - 2] = 'F';
		}
		break;
	case 'D':
	case 'd':
		if (map[Crow][Ccol + 2] == ' ' && !hasPlayerBullet) {
			map[Crow][Ccol + 2] = '>';
			mapback[Crow][Ccol + 2] = 'F';
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
}
//function to kill the bot. returns true if a bot has been killed.
bool killBot(char bulletdirection,int row,int col) {
	int center;
	switch (bulletdirection) {
	case UP:
		for(center = col - 1; center <= col + 1;center++){
			if (map[row][center] == 'V') {
				for (int i = row - 1; i <= row; i++) {
					for (int j = center - 1; j <= center + 1; j++) {
						map[i][j] = ' ';
					}
				}
				score += 2;
				botcount--;
				return true;
			}
		}
		break;
	case DOWN:
		for (center = col - 1; center <= col + 1; center++) {
			if (map[row + 1][center] == 'V') {
				for (int i = row; i <= row + 1; i++) {
					for (int j = center - 1; j <= center + 1; j++) {
						map[i][j] = ' ';
					}
				}
				score += 2;
				botcount--;
				return true;
			}
		}
		break;
	case LEFT:
		for (center = row; center <= row + 1; center++) {
			if (map[center][col - 1] == 'V') {
				for (int i = center - 1; i <= center; i++) {
					for (int j = col - 2; j <= col; j++) {
						map[i][j] = ' ';
					}
				}
				score += 2;
				botcount--;
				return true;
			}
		}
		break;
	case RIGHT:
		for (center = row; center <= row + 1; center++) {
			if (map[center][col + 1] == 'V') {
				for (int i = center - 1; i <= center; i++) {
					for (int j = col; j <= col + 2; j++) {
						map[i][j] = ' ';
					}
				}
				score += 2;
				botcount--;
				return true;
			}
		}
		break;
	}
	return false;
}
//summons botslaser
void summonLaser() {
	int direction;
	int rowdiff, coldiff;
	for (int row = 1; row < ROW - 1; row++) {
		for (int col = 1; col < COL - 1; col++) {
			if (map[row][col] == 'V') {
				rowdiff = Crow - row;
				coldiff = Ccol - col;
				while (rowdiff && coldiff) {
					rowdiff--;
					coldiff--;
				}
				if (coldiff) {
					if (coldiff < 0 && map[row][col - 2] == ' ') {
						map[row][col - 2] = '<';
						mapback[row][col - 2] = 'B';
					}
					else if (coldiff > 0 && map[row][col + 2] == ' ') {
						map[row][col + 2] = '>';
						mapback[row][col + 2] = 'B';
					}
				}
				else if (rowdiff) {
					if (rowdiff < 0 && map[row - 2][col] == ' ') {
						map[row - 2][col] = '^';
						mapback[row - 2][col] = 'B';
					}
					else if (rowdiff > 0 && map[row + 1][col] == ' ') {
						map[row + 1][col] = 'v';
						mapback[row + 1][col] = 'B';
					}
				}
			}
		}
	}
}
bool damagePlayer(int row,int col) {
	if (map[row][col] == '/' || map[row][col] == '\\' || map[row][col] == 'O' || map[row + 1][col] == 'O') {
		return 1;
		lives--;
	}
	return 0;
}
//moves the bullet per update
void moveBullets() {
	bool iskilled = false;
	bool isdamaged = false;
	for (int row = 1; row < ROW - 1; row++) {
		for (int col = 1; col < COL - 1; col++) {
			if (map[row][col] == '^') {
				map[row][col] = ' ';
				if (mapback[row][col] == 'F') {
					mapback[row][col] = ' ';
					iskilled = killBot(UP, row - 1, col);
					if ((map[row - 1][col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[row - 1][col] = '^';
						mapback[row - 1][col] = 'F';
					}
				}
				else {
					mapback[row][col] = ' ';
					iskilled = damagePlayer(row - 1, col);
					if ((map[row - 1][col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[row - 1][col] = '^';
						mapback[row - 1][col] = 'B';
					}
				}
			}
			else if (map[row][col] == '<') {
				map[row][col] = ' ';
				if (map[row][col] == 'F') {
					mapback[row][col] = ' ';
					iskilled = killBot(LEFT, row, col - 1);
					if ((map[row][col - 1] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[row][col - 1] = '<';
						mapback[row][col - 1] = 'F';
					}
				}
				else {
					mapback[row][col] = ' ';
					iskilled = damagePlayer(row, col - 1);
					if ((map[row][col - 1] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[row][col - 1] = '<';
						mapback[row][col - 1] = 'B';
					}
				}
			}
			else if (map[ROW - 1 - row][COL - 1 - col] == 'v') {
				map[ROW - 1 - row][COL - 1 - col] = ' ';
				if (map[ROW - 1 - row][COL - 1 - col] == 'F') {
					mapback[ROW - 1 - row][COL - 1 - col] = ' ';
					iskilled = killBot(DOWN, ROW - row, COL - 1 - col);
					if ((map[ROW - row][COL - 1 - col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[ROW - row][COL - 1 - col] = 'v';
						mapback[ROW - row][COL - 1 - col] = 'F';
					}
				}
				else {
					mapback[ROW - 1 - row][COL - 1 - col] = ' ';
					iskilled = damagePlayer(ROW - row, COL - 1 - col);
					if ((map[ROW - row][COL - 1 - col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[ROW - row][COL - 1 - col] = 'v';
						mapback[ROW - row][COL - 1 - col] = 'B';
					}
				}
			}
			else if (map[ROW - 1 - row][COL - 1 - col] == '>') {
				map[ROW - 1 - row][COL - 1 - col] = ' ';
				if (map[ROW - 1 - row][COL - 1 - col] == 'F') {
					mapback[ROW - 1 - row][COL - 1 - col] = ' ';
					iskilled = killBot(RIGHT, ROW - 1 - row, COL - col);
					if ((map[ROW - 1 - row][COL - col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[ROW - 1 - row][COL - col] = '>';
						mapback[ROW - 1 - row][COL - col] = 'F';
					}
				}
				else {
					mapback[ROW - 1 - row][COL - 1 - col] = ' ';
					iskilled = damagePlayer(ROW - 1 - row, COL - col);
					if ((map[ROW - 1 - row][COL - col] == ' ' && row != 0 && row != ROW - 1 && col != 0 && col != COL - 1) && !iskilled) {
						map[ROW - 1 - row][COL - col] = '>';
						mapback[ROW - 1 - row][COL - col] = 'B';
					}
				}
			}
		}
	}
}
//updates the game! handling shooting of bullets and moving of robots
void tickUpdate() {
	moveBullets();
	summonLaser();
}
void game() {
	char Input;
	addItems(5);
	while (lives) {
		system("cls");
		cout << lives << endl;
		printMap();
		Input = _getch();
		system("cls");
		eventHandler(Input);
		tickUpdate();
	}
}
int main() {
	name = nameInput();
	game();
	return 0;
}