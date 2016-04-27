#include "stdafx.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#include <Windows.h>
#include <conio.h>

#define key_UP 72
#define key_DOWN 80
#define key_LEFT 75
#define key_RIGHT 77

//need to work on these
#define UP_FREE ((mainGrid[i][j] == '.') && (mainGrid[i - 1][j] == '|') && (mainGrid[i - 2][j] == '|') && (mainGrid[i - 3][j] == '|') && (mainGrid[i - 1][j - 1] == '|') && (mainGrid[i - 1][j + 1] == '|'))
#define RIGHT_FREE ((mainGrid[i][j] == '.') && (mainGrid[i][j + 1] == '|') && (mainGrid[i][j + 2] == '|') && (mainGrid[i][j + 3] == '|') && (mainGrid[i + 1][j + 1] != '.') && (mainGrid[i - 1][j + 1] != '.') && (mainGrid[i - 1][j] != '*'))
#define DOWN_FREE ((mainGrid[i][j] == '.') && (mainGrid[i + 1][j] == '|') && (mainGrid[i + 2][j] == '|') && (mainGrid[i + 3][j] == '|') && (mainGrid[i + 1][j + 1] == '|') && (mainGrid[i + 1][j - 1] == '|'))
#define LEFT_FREE ((mainGrid[i][j] == '.') && (mainGrid[i][j - 1] == '|') && (mainGrid[i][j - 2] == '|') && (mainGrid[i][j - 3] == '|') && (mainGrid[i - 1][j - 1] != '.') && (mainGrid[i + 1][j - 1] != '.'))

#define break_line cout << endl

using namespace std;
typedef vector<vector<char>>grid;

void nextLevel(grid&);
int  initGrid(grid&, int&, int&);
int  generateGrid(grid&, int&, int&);
bool checkReachedBorder(grid&, int&, int&);
int  fillWithObjects(grid&);
void globalMap(grid&, int&, int&);
bool engageFight();
int  drawGrid(grid);

//setting global variables
const int number_of_directions = 8; //essential for grid randomizer

int dungeon_width = 0; int dungeon_height = 0; int points_number = 0;
int player_health = 50; //set player health here

int main() {
	srand(static_cast<unsigned int>(time(NULL)));
	grid mainGrid(50, vector<char>(200));

	nextLevel(mainGrid); //game starts

	system("PAUSE");
	return 0;
}

void nextLevel(grid& mainGrid) {
	if ((dungeon_width <= 200) && (dungeon_height <= 50)) { //musn't pass vector borders
		dungeon_width += 16; dungeon_height += 4;
	}
	int x = 0; int y = 0;
	initGrid(mainGrid, x, y);
	fillWithObjects(mainGrid);

	x = 1; y = dungeon_height / 2; //setting 'x' and 'y' for global map
	system("cls"); drawGrid(mainGrid); //clear screen and draw changed grid
	globalMap(mainGrid, x, y);
}

int initGrid(grid& mainGrid, int &x, int &y) {
	for (int i = 0; i < dungeon_height; i++) {
		for (int j = 0; j < dungeon_width; j++) {
			if ((i == 0) || ((j == 0) || (j == dungeon_width - 1)) || (i == dungeon_height - 1)) {
				mainGrid[i][j] = '*'; //setting up borders
			}
			else {
				mainGrid[i][j] = '|'; //filling in black space
			}
		}
	}
	y = dungeon_height / 2; //entrance will always be in the middle

	mainGrid[y][x] = '[';
	x++;					//marking the entance and placing the player
	mainGrid[y][x] = 'P';

	generateGrid(mainGrid, x, y);
	return 0;
}

int generateGrid(grid& mainGrid, int &x, int &y) {
	if (!checkReachedBorder(mainGrid, x, y)) { //recursion base

											   //randomizing essential variables
		int dungeonRandomizer = rand() % number_of_directions;
		int heightRandomizer = rand() % dungeon_height;
		int widthRandomizer = rand() % dungeon_width;
		int directionRandomizer = rand() % 2;

		//0 - up, 1 - right, 2 - down, 3 - left

		if (dungeonRandomizer == 0) {
			if ((mainGrid[y - 1][x - 1] == '|') && (mainGrid[y - 1][x + 1] == '|') && (mainGrid[y - 1][x] == '|')) {
				mainGrid[y - 1][x] = '.';
				y--;
			}
		}
		if (dungeonRandomizer == 1) {
			if (((mainGrid[y + 1][x + 1] != '.') && (mainGrid[y - 1][x + 1] != '.')) && (mainGrid[y][x + 1] == '|')) {
				mainGrid[y][x + 1] = '.';
				x++;
			}
		}
		if (dungeonRandomizer == 2) {
			if ((mainGrid[y + 1][x + 1] == '|') && (mainGrid[y + 1][x - 1] == '|') && (mainGrid[y + 1][x] == '|')) {
				mainGrid[y + 1][x] = '.';
				y++;
			}
		}
		if (dungeonRandomizer == 3) {
			if (((mainGrid[y - 1][x - 1] != '.') && (mainGrid[y + 1][x - 1] != '.')) && (mainGrid[y][x - 1] == '|')) {
				mainGrid[y][x - 1] = '.';
				x--;
			}
		}

		/* Here brancing begins, we scan and look for a place to add a branch,
		branches will be placed: 4 - up, 5 - right, 6 - down, 7 - left from
		the dungeon.

		'dungeonRandomizer' allowes us to scan code randomly from the bottom,
		or the opposite */

		//Too much repeating, consider rewriting

		if ((dungeonRandomizer == 4) && (directionRandomizer == 0)) {
			for (int i = heightRandomizer; i < dungeon_height; i++) {
				for (int j = widthRandomizer; j < dungeon_width; j++) {
					if UP_FREE{
						mainGrid[i - 1][j] = '.';
					x = j; y = i - 1;
					break;
					}
				}
			}
		}
		if ((dungeonRandomizer == 4) && (directionRandomizer == 1)) {
			for (int i = heightRandomizer; i >= 0; i--) {
				for (int j = widthRandomizer; j >= 0; j--) {
					if UP_FREE{
						mainGrid[i - 1][j] = '.';
					x = j; y = i - 1;
					break;
					}
				}
			}
		}
		if ((dungeonRandomizer == 5) && (directionRandomizer == 0)) {
			for (int i = heightRandomizer; i < dungeon_height; i++) {
				for (int j = widthRandomizer; j < dungeon_width; j++) {
					if RIGHT_FREE{
						mainGrid[i][j + 1] = '.';
					x = j + 1; y = i;
					break;
					}
				}
			}
		}
		if ((dungeonRandomizer == 5) && (directionRandomizer == 1)) {
			for (int i = heightRandomizer; i >= 0; i--) {
				for (int j = widthRandomizer; j >= 0; j--) {
					if RIGHT_FREE{
						mainGrid[i][j + 1] = '.';
					x = j + 1; y = i;
					break;
					}
				}
			}
		}
		if ((dungeonRandomizer == 6) && (directionRandomizer == 0)) {
			for (int i = heightRandomizer; i < dungeon_height; i++) {
				for (int j = widthRandomizer; j < dungeon_width; j++) {
					if DOWN_FREE{
						mainGrid[i + 1][j] = '.';
					x = j; y = i + 1;
					break;
					}
				}
			}
		}
		if ((dungeonRandomizer == 6) && (directionRandomizer == 1)) {
			for (int i = heightRandomizer; i >= 0; i--) {
				for (int j = widthRandomizer; j >= 0; j--) {
					if DOWN_FREE{
						mainGrid[i + 1][j] = '.';
					x = j; y = i + 1;
					break;
					}
				}
			}
		}
		if (dungeonRandomizer == 7) {

			//Left branching needs exceptions, to make sure that it doesn't leave vector borders
			int exeptionHeight = 0; int exeptionWidth = 0;
			if (heightRandomizer == 0) exeptionHeight = 1;
			else exeptionHeight = heightRandomizer;
			if (widthRandomizer == 0) exeptionWidth = 1;
			else exeptionWidth = widthRandomizer;

			if (directionRandomizer == 0) {
				for (int i = exeptionHeight; i < dungeon_height; i++) {
					for (int j = exeptionWidth; j < dungeon_width; j++) {
						if LEFT_FREE{
							mainGrid[i][j - 1] = '.';
						x = j - 1; y = i;
						break;
						}
					}
				}
			}
			if (directionRandomizer == 1) {
				for (int i = exeptionHeight; i <= 0; i--) {
					for (int j = exeptionWidth; j <= 0; j--) {
						if LEFT_FREE{
							mainGrid[i][j - 1] = '.';
						x = j - 1; y = i;
						break;
						}
					}
				}
			}
		}
		generateGrid(mainGrid, x, y); //recursion
	}
	return 0;
}

bool checkReachedBorder(grid& mainGrid, int &x, int &y) {

	//if current 'x' is three symbols left to the right border, we fill the gap and end generation
	if ((mainGrid[y][x] == '.') && (mainGrid[y][x + 3] == '*')) {
		mainGrid[y][x + 3] = ']';
		mainGrid[y][x + 2] = '.';
		mainGrid[y][x + 1] = '.';
		return true;
	}
	return false;
}

int fillWithObjects(grid& mainGrid) { //randomly fills the dungeon with objects, consider adding difficulty
	for (int i = 0; i < dungeon_height; i++) {
		for (int j = 0; j < dungeon_width; j++) {
			if (mainGrid[i][j] == '.') {
				int objectsRandomizer = rand() % 100;
				if ((objectsRandomizer >= 0) && (objectsRandomizer < 3)) {
					mainGrid[i][j] = 'E';
				}
				if ((objectsRandomizer >= 30) && (objectsRandomizer < 33)) {
					mainGrid[i][j] = 'C';
				}
			}
		}
	}
	return 0;
}

//consider rewriting, too bulky
void globalMap(grid& mainGrid, int& x, int& y) {

	//Controlls are given to the player
	for (;;) {
		if (_kbhit()) {
			int keypress = _getch();
			switch (keypress) {
			case key_LEFT: {
				if (((mainGrid[y][x - 1]) == '.')) { //if there is nothing, move there
					mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = '.'; x--;
					break;
				}
				if (((mainGrid[y][x - 1]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = '.'; x--;
						break;
					}
				}
				if (((mainGrid[y][x - 1]) == 'C')) { //if there is a chest, add points
					mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = '.'; x--; 
					break;
				}
				break;
			}
			case key_RIGHT: {
				if ((mainGrid[y][x + 1]) == '.') { //if there is nothing, move there
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = '.'; x++; 
					break;
				}
				if (((mainGrid[y][x + 1]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = '.'; x++;
						break;
					}
				}
				if (((mainGrid[y][x + 1]) == 'C')) { //if there is a chest, add points
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = '.'; x++;
					break;
				}
				if (((mainGrid[y][x + 1]) == ']')) { //if there is an exit, start new level
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = '.'; x++; nextLevel(mainGrid);
					break;
				}
				break;
			}
			case key_UP: {
				if ((mainGrid[y - 1][x]) == '.') { //if there is nothing, move there
					mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = '.'; y--; 
					break;
				}
				if (((mainGrid[y - 1][x]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = '.'; y--; 
						break;
					}
				}
				if (((mainGrid[y - 1][x]) == 'C')) { //if there is a chest, add points
					mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = '.'; y--;
					break;
				}
				break;
			}
			case key_DOWN: {
				if ((mainGrid[y + 1][x]) == '.') { //if there is nothing, move there
					mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = '.'; y++;
					break;
				}
				if (((mainGrid[y + 1][x]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = '.'; y++;
						break;
					}
				}
				if (((mainGrid[y + 1][x]) == 'C')) { //if there is a chest, add points
					mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = '.'; y++;
					break;
				}
				break;
			}
			}
			system("cls"); drawGrid(mainGrid); //clear screen and draw changed grid
		}
	}
}

bool engageFight() {
	return true;
}

int drawGrid(grid mainGrid) {
	for (int i = 0; i < dungeon_height; i++) {
		for (int j = 0; j < dungeon_width; j++) {
			cout << mainGrid[i][j];
		}
		break_line;
	}
	return 0;
}


















