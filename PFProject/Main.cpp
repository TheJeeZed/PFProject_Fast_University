// this is the start of programming fundamental semester project.
// Muhammad Shahmeer(24F-0684)
// Sobaan Ahmed(24F-0564)
// Hafiza Arfa (24F-0721)

#include <iostream>

using namespace std;

const int ROW = 50, COL = 50;
void placeWalls(char arr[][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (i == 0 || i == COL - 1) {
				arr[i][j] = '-';
			}
			else if (j == 0 || j == ROW - 1) {
				arr[i][j] = '|';
			}
			else {
				arr[i][j] = ' ';
			}
		}
	}
}
void printMap(char arr[][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			cout << arr[i][j];
		}
		cout << endl;
	}
}

int main() {
	char map[ROW][COL];
	placeWalls(map);
	printMap(map);
	cout << "HelloWorld!";
	system("pause");
	return 0;
}