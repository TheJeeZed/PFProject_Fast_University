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
	//to set up initial position of character
	srand(time(0));
	Crow = (rand() % (ROW / 2 - 2)) + ROW / 2;
	Ccol = (rand() % (COL - 3) + 1);
	placeCharacter();
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
//check if the player can move to the desired location
bool isvalidMovelocation(char direction) {
	switch (direction) {
	case UP:
		if (map[Crow - 2][Ccol - 1] != ' '||map[Crow - 2][Ccol] != ' '||map[Crow - 2][Ccol + 1] != ' ')
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
	case UP: //character for up arrow key
		if (isvalidMovelocation(UP)) {
			map[Crow + 1][Ccol - 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow][Ccol] = ' ';
			Crow--;
			placeCharacter();
		}
		break;
	case DOWN: //character for down arrow key
		if (isvalidMovelocation(DOWN)) {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol - 1] = ' ';
			map[Crow][Ccol + 1] = ' ';
			Crow++;
			placeCharacter();
		}
		break;
	case LEFT: //character for right arrow key
		if (isvalidMovelocation(LEFT)) {
			map[Crow - 1][Ccol] = ' ';
			map[Crow][Ccol + 1] = ' ';
			map[Crow + 1][Ccol + 1] = ' ';
			map[Crow + 1][Ccol - 1] = ' ';
			Ccol--;
			placeCharacter();
		}
		break;
	case RIGHT: //character for right arrow key
		if (isvalidMovelocation(RIGHT)) {
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
void game() {
	char Input;
	addItems();
	while (true) {
		system("cls");
		printMap();
		Input = _getch();
		system("cls");
		eventHandler(Input);
	}
}
int main() {
	char c;
	string name = "";
	name = nameInput();
	game();
	return 0;
}