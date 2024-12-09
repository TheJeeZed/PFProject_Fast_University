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
const int ROW = 30, COL = 30, SCORES = 10;
//current location of the player
int Crow, Ccol;
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
//add initial items to the level
void addItems() {
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
	//to set up initial position of characters
	srand(time(0));
	Crow = (rand() % (ROW / 2 - 2)) + ROW / 2;
	Ccol = (rand() % (COL - 3) + 1);
	placeCharacter();
	placeBot(20, 20);
}
//for summoning lasers in the game.both for player and bots
void summonLaser(bool isplayer, char direction) {
	if (isplayer) {
		switch (direction) {
		case UP:
			map[Crow - 2][Ccol] = '^';
			break;
		case DOWN:
			map[Crow + 2][Ccol] = 'v';
			break;
		case LEFT:
			map[Crow][Ccol - 2] = '<';
			break;
		case RIGHT:
			map[Crow][Ccol + 2] = '>';
			break;
		}
	}
}
//check if the entity can move to the desired location
//1 for player.2 for bullet
bool isvalidMovelocation(int entity,char direction, int row = 0, int col = 0) {
	switch (entity) {
	case 1://for the player movement
		switch (direction) {
		case UP:
			if (map[Crow - 2][Ccol - 1] != ' ' || map[Crow - 2][Ccol] != ' ' || map[Crow - 2][Ccol + 1] != ' ')
				return 0;
			break;
		case DOWN:
			if (map[Crow + 2][Ccol - 1] != ' ' || map[Crow + 2][Ccol] != ' ' || map[Crow + 2][Ccol + 1] != ' ')
				return 0;
			break;
		case LEFT:
			if (map[Crow - 1][Ccol - 2] != ' ' || map[Crow][Ccol - 2] != ' ' || map[Crow + 1][Ccol - 2] != ' ')
				return 0;
			break;
		case RIGHT:
			if (map[Crow - 1][Ccol + 2] != ' ' || map[Crow][Ccol + 2] != ' ' || map[Crow + 1][Ccol + 2] != ' ')
				return 0;
			break;
		}
		break;
	case 2:
		if (map[row][col] != ' ')
			return 0;
		break;
	}
	return 1;
}
//handles all the inputs entered by the user while playing the game
void eventHandler(char action) {
	switch (action) {
	case 'W':
	case 'w':
		summonLaser(true, UP);
		break;
	case 'S':
	case 's':
		summonLaser(true, DOWN);
		break;
	case 'A':
	case 'a':
		summonLaser(true, LEFT);
		break;
	case 'D':
	case 'd':
		summonLaser(true, RIGHT);
		break;
	case UP: 
		if (isvalidMovelocation(1,UP)) {
			map[Crow + 1][Ccol - 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow][Ccol] = ' ';
			Crow--;
			placeCharacter();
		}
		break;
	case DOWN:
		if (isvalidMovelocation(1,DOWN)) {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol - 1] = ' ';
			map[Crow][Ccol + 1] = ' ';
			Crow++;
			placeCharacter();
		}
		break;
	case LEFT:
		if (isvalidMovelocation(1,LEFT)) {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol + 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow + 1][Ccol - 1] = ' ';
			Ccol--;
			placeCharacter();
		}
		break;
	case RIGHT:
		if (isvalidMovelocation(1,RIGHT)) {
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
	bool isBot = false;
	int center;
	switch (bulletdirection) {
	case UP:
		for(center = col - 1; center <= col + 1;center++){
			if (map[row][center] == 'V') {
				isBot = true;
				break;
			}
		}
		if (isBot) {
			for (int i = row - 1; i <= row; i++) {
				for (int j = center - 1; j <= center + 1; j++) {
					map[i][j] = ' ';
				}
			}
			return true;
		}
		break;
	case DOWN:
		for (center = col - 1; center <= col + 1; center++) {
			if (map[row + 1][center] == 'V') {
				isBot = true;
				break;
			}
		}
		if (isBot) {
			for (int i = row; i <= row + 1; i++) {
				for (int j = center - 1; j <= center + 1; j++) {
					map[i][j] = ' ';
				}
			}
			return true;
		}
		break;
	case LEFT:
		for (center = row; center <= row + 1; center++) {
			if (map[center][col - 1] == 'V') {
				isBot = true;
				break;
			}
		}
		if (isBot) {
			for (int i = center - 1; i <= center; i++) {
				for (int j = col - 2; j <= col; j++) {
					map[i][j] = ' ';
				}
			}
			return true;
		}
		break;
	case RIGHT:
		for (center = row; center <= row + 1; center++) {
			if (map[center][col + 1] == 'V') {
				isBot = true;
				break;
			}
		}
		if (isBot) {
			for (int i = center - 1; i <= center; i++) {
				for (int j = col; j <= col + 2; j++) {
					map[i][j] = ' ';
				}
			}
			return true;
		}
		break;
	}
	return false;
}
//moves the bullet per update and perlocation
void moveBullets(int row, int col) {
	bool iskilled = false;
	if (map[row][col] == '^') {
		map[row][col] = ' ';
		iskilled = killBot(UP, row - 1, col);
		if (isvalidMovelocation(2, UP, row - 1, col) && !iskilled)
			map[row - 1][col] = '^';
	}
	else if (map[row][col] == '<') {
		map[row][col] = ' ';
		iskilled = killBot(LEFT, row, col - 1);
		if (isvalidMovelocation(2, LEFT, row, col - 1) && !iskilled)
			map[row][col - 1] = '<';
	}
	else if (map[ROW - 1 - row][COL - 1 - col] == 'v') {
		map[ROW - 1 - row][COL - 1 - col] = ' ';
		iskilled = killBot(DOWN, ROW - row, COL - 1 - col);
		if (isvalidMovelocation(2, DOWN, ROW - row, COL - 1 - col) && !iskilled)
			map[ROW - row][COL - 1 - col] = 'v';
	}
	else if (map[ROW - 1 - row][COL - 1 - col] == '>') {
		map[ROW - 1 - row][COL - 1 - col] = ' ';
		iskilled = killBot(RIGHT, ROW - 1 - row, COL - col);
		if (isvalidMovelocation(2, RIGHT, ROW - 1 - row, COL - col) && !iskilled)
			map[ROW - 1 - row][COL - col] = '>';
	}
}
//updates the game! handling shooting of bullets and moving of robots
void tickUpdate() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			moveBullets(i, j);
		}
	}
}
void game() {
	char Input;
	addItems();
	while (true) {
		system("cls");
		printMap();
		Input = _getch();
		system("cls");
		eventHandler(Input);
		tickUpdate();
	}
}
int main() {
	char c;
	string name = "";
	name = nameInput();
	game();
	return 0;
}