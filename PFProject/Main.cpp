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

const int ROW = 30, COL = 30, SCORES = 10;
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
	//for placing the outside boundries
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

void eventHandler(char action) {
	switch (action) {
	case 'W':
	case 'w':
		break;
	case 'S':
	case 's':
		break;
	case 'A':
	case 'a':
		break;
	case 'D':
	case 'd':
		break;
	case 72: //character for up arrow key
		map[Crow + 1][Ccol - 1] = ' ';
		map[Crow + 1][Ccol + 1] = ' ';
		map[Crow][Ccol] = ' ';
		Crow--;
		placeCharacter();
		break;
	case 80: //character for down arrow key
		map[Crow - 1][Ccol] = ' ';
		map[Crow][Ccol - 1] = ' ';
		map[Crow][Ccol + 1] = ' ';
		Crow++;
		placeCharacter();
		break;
	case 75: //character for right arrow key
		map[Crow - 1][Ccol] = ' ';
		map[Crow][Ccol + 1] = ' ';
		map[Crow + 1][Ccol + 1] = ' ';
		map[Crow + 1][Ccol - 1] = ' ';
		Ccol--;
		placeCharacter();
		break;
	case 77: //character for right arrow key
		map[Crow - 1][Ccol] = ' ';
		map[Crow][Ccol - 1] = ' ';
		map[Crow + 1][Ccol - 1] = ' ';
		map[Crow + 1][Ccol + 1] = ' ';
		Ccol++;
		placeCharacter();
		break;
	}
}
int main() {
	char c;
	string name = "";
	addItems();
	printMap();
	system("pause");
	while (true) {
		c = _getch();
		system("cls");
		eventHandler(c);
		printMap();
	}
	return 0;
}