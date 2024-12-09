//
//#include<iostream>
//using namespace std;
//int i, j;
//const int ROW = 50;
//const int COL = 30;
//char arr[ROW][COL];
//void level1();
//void level2();
//void level3();
//void level4();
//void level5();
//void placeObstacle(int width);
//int main()
//{
//	level1();
//	cout << endl;
//	level2();
//	cout << endl;
//	level3();
//	cout << endl;
//	level4();
//	cout << endl;
//	level5();
//
//	system("pause");
//	return 0;
//}
//void bounderies(){
//	{
//		for (i = 0; i < ROW; i++)                //for space
//		{
//			for (j = 0; j < COL; j++)
//			{
//				arr[i][j] = ' ';
//			}
//		}
//		for (j = 0; j < COL; j++)                 //Horizontal bound
//		{
//			arr[0][j] = '-';
//			arr[ROW - 1][j] = '-';
//
//		}
//		for (i = 0; i < ROW; i++)                 //Vertical bound
//		{
//			arr[i][0] = '|';
//			arr[i][COL - 1] = '|';
//		}
//	}
//}
//
//void print() {
//	for (i = 0; i < ROW; i++)                  //for print
//	{
//		for (j = 0; j < COL; j++)
//			cout << arr[i][j];
//		cout << endl;
//	}
//}
//void level1()
//{
//	bounderies();
//	placeObstacle(3);
//	placeObstacle(3);
//	print();
//}
//void level2()
//{
//	bounderies();
//	placeObstacle(3);
//	placeObstacle(3);
//	placeObstacle(3);
//	print();
//}
//void level3()
//{
//	bounderies();
//	placeObstacle(4);
//	placeObstacle(4);
//	placeObstacle(4);
//	print();
//}
//void level4()
//{
//	bounderies();
//	placeObstacle(4);
//	placeObstacle(4);
//	placeObstacle(4);
//	placeObstacle(4);
//	print();
//}
//void level5()
//{
//	bounderies();
//	placeObstacle(5);
//	placeObstacle(5);
//	placeObstacle(5);
//	placeObstacle(5);
//	print();
//}
//void placeObstacle(int width)                          
//{
//	bool isvalid = false;
//	int row, col;
//	
//	while (!isvalid)
//	{
//		// Seed the random number generator
//		srand(time(0));
//		row = 1 + rand() % (ROW - 2);
//		col = 1 + rand() % (COL - 10);
//		isvalid = true;
//		for (int i = col; i <= col + width + 1; i++) {
//			if (arr[row][i] != ' ') {
//				isvalid = false;
//				break;
//			}
//		}
//	}
//	arr[row][col] = '[';
//	arr[row][col + width + 1] = ']';
//	for (int i = col + 1; i <= col + width; i++) {
//		arr[row][i] = '-';
//	}
//}