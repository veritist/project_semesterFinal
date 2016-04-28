#include "stdafx.h"

//add default to switch case

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>

#include <Windows.h>
#include <conio.h>

#define key_UP 72
#define key_DOWN 80
#define key_LEFT 75
#define key_RIGHT 77

#define key_1 49
#define key_2 50
#define key_3 51

//need to work on these
#define UP_FREE ((mainGrid[i][j] == ' ') && (mainGrid[i - 1][j] == '|') && (mainGrid[i - 2][j] == '|') && (mainGrid[i - 3][j] == '|') && (mainGrid[i - 1][j - 1] == '|') && (mainGrid[i - 1][j + 1] == '|'))
#define RIGHT_FREE ((mainGrid[i][j] == ' ') && (mainGrid[i][j + 1] == '|') && (mainGrid[i][j + 2] == '|') && (mainGrid[i][j + 3] == '|') && (mainGrid[i + 1][j + 1] != ' ') && (mainGrid[i - 1][j + 1] != ' ') && (mainGrid[i - 1][j] != '*'))
#define DOWN_FREE ((mainGrid[i][j] == ' ') && (mainGrid[i + 1][j] == '|') && (mainGrid[i + 2][j] == '|') && (mainGrid[i + 3][j] == '|') && (mainGrid[i + 1][j + 1] == '|') && (mainGrid[i + 1][j - 1] == '|'))
#define LEFT_FREE ((mainGrid[i][j] == ' ') && (mainGrid[i][j - 1] == '|') && (mainGrid[i][j - 2] == '|') && (mainGrid[i][j - 3] == '|') && (mainGrid[i - 1][j - 1] != ' ') && (mainGrid[i + 1][j - 1] != ' '))

#define break_line cout << endl
#define pause cout << "[SPACE]" << endl; for(;;) if(_getch() == 32) break;
#define refresh system("cls"); drawGrid(mainGrid); //clear screen and draw changed grid

#define up 0
#define right 1
#define down 2
#define left 3 

#define sl << char(92) <<

using namespace std;
typedef vector<vector<char>>grid; //2-dimentional vector

typedef struct enemy {	//struct for enemy
	int base_damage;
	int health;
	string name;
};

typedef struct ability { //struct for player abilities
	int damage;
	int cooldown;
	int cur_cooldown;
};

void hideCursor();
void main_menu();
void nextLevel(grid&);
int  initGrid(grid&, int&, int&);
int  generateGrid(grid&, int&, int&);
bool checkReachedBorder(grid&, int&, int&);
int  fillWithObjects(grid&);
void globalMap(grid&, int&, int&);
void consoleMove(int, int, int);
void heal_player();
void add_points(int);
void draw_ui();
bool engageFight();
enemy selectEnemy();
int  drawGrid(grid);

//setting global variables
const int number_of_directions = 8; //essential for grid randomizer
const int player_damage = 21;

int dungeon_width = 16; int dungeon_height = 6; int points_number = 0;
int level_number = 0; int player_health = 50; //set player health here

int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	system("Color 1A");
	hideCursor();
	main_menu();

	system("PAUSE");
	return 0;
}

void hideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void main_menu() {
	grid mainGrid(26, vector<char>(116));
	system("cls");
	cout <<"         ___    _" << endl
		<< " _      |   " sl "  | |   ____" << endl
		<< "| |     | |" sl " " sl " | |  |  . |" << endl
		<< "| |___  | | " sl " " sl "| |  |    " sl endl
		<< "|_____| |_|  " sl "___|  |__|__|" << endl;
	cout << "       Loot 'N  Run" << endl
		<< "       (by Veritist)" << endl;
	cout << "          "; pause;
	nextLevel(mainGrid);
}

void nextLevel(grid& mainGrid) {

	//musn't pass vector borders
	if (dungeon_width <= 116) dungeon_width += 4;
	if ((dungeon_height <= 24) && (level_number % 2 == 0)) dungeon_height += 2;

	int x = 0; int y = 0;
	initGrid(mainGrid, x, y);
	fillWithObjects(mainGrid);

	x = 1; y = dungeon_height / 2; //setting 'x' and 'y' for global map
	level_number++;
	refresh; //clear screen and draw changed grid
	draw_ui();
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

	mainGrid[y][x] = 'X';
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
				mainGrid[y - 1][x] = ' ';
				y--;
			}
		}
		if (dungeonRandomizer == 1) {
			if (((mainGrid[y + 1][x + 1] != ' ') && (mainGrid[y - 1][x + 1] != ' ')) && (mainGrid[y][x + 1] == '|')) {
				mainGrid[y][x + 1] = ' ';
				x++;
			}
		}
		if (dungeonRandomizer == 2) {
			if ((mainGrid[y + 1][x + 1] == '|') && (mainGrid[y + 1][x - 1] == '|') && (mainGrid[y + 1][x] == '|')) {
				mainGrid[y + 1][x] = ' ';
				y++;
			}
		}
		if (dungeonRandomizer == 3) {
			if (((mainGrid[y - 1][x - 1] != ' ') && (mainGrid[y + 1][x - 1] != ' ')) && (mainGrid[y][x - 1] == '|')) {
				mainGrid[y][x - 1] = ' ';
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
						mainGrid[i - 1][j] = ' ';
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
						mainGrid[i - 1][j] = ' ';
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
						mainGrid[i][j + 1] = ' ';
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
						mainGrid[i][j + 1] = ' ';
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
						mainGrid[i + 1][j] = ' ';
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
						mainGrid[i + 1][j] = ' ';
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
							mainGrid[i][j - 1] = ' ';
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
							mainGrid[i][j - 1] = ' ';
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
	if ((mainGrid[y][x] == ' ') && (mainGrid[y][x + 3] == '*')) {
		mainGrid[y][x + 3] = 'X';
		mainGrid[y][x + 2] = ' ';
		mainGrid[y][x + 1] = ' ';
		return true;
	}
	return false;
}

int fillWithObjects(grid& mainGrid) { //randomly fills the dungeon with objects, consider adding difficulty
	for (int i = 0; i < dungeon_height; i++) {
		for (int j = 0; j < dungeon_width; j++) {
			if (mainGrid[i][j] == ' ') {
				int objectsRandomizer = rand() % 100;
				if ((objectsRandomizer >= 0) && (objectsRandomizer < 4)) {
					mainGrid[i][j] = 'E';
				}
				if ((objectsRandomizer >= 50) && (objectsRandomizer < 54)) {
					mainGrid[i][j] = 'C';
				}
				if (objectsRandomizer == 90) {
					mainGrid[i][j] = 'H';
				}
			}
		}
	}
	return 0;
}

//consider rewriting, too bulky
void globalMap(grid& mainGrid, int& x, int& y) {

	//Controlls are given to the player
	while (true) {
		if (_kbhit()) {
			int keypress = _getch();
			switch (keypress) {
			case key_LEFT:
				if (((mainGrid[y][x - 1]) == ' ')) { //if there is nothing, move there
					mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, left); x--;
					break;
				}
				if (((mainGrid[y][x - 1]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = ' '; x--;
					}
					refresh;
					draw_ui();
					break;
				}
				if (((mainGrid[y][x - 1]) == 'C')) { //if there is a chest, add points
					mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, left); x--;
					add_points(0);
					draw_ui();
					break;
				}
				if (((mainGrid[y][x - 1]) == 'H')) { //if there is a potion, heal player
					mainGrid[y][x - 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, left); x--;
					heal_player();
					draw_ui();
					break;
				}
				break;

			case key_RIGHT:
				if ((mainGrid[y][x + 1]) == ' ') { //if there is nothing, move there
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, right); x++;
					break;
				}
				if (((mainGrid[y][x + 1]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = ' '; x++;
					}
					refresh;
					draw_ui();
					break;
				}
				if (((mainGrid[y][x + 1]) == 'C')) { //if there is a chest, add points
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, right); x++;
					add_points(0);
					draw_ui();
					break;
				}
				if (((mainGrid[y][x + 1]) == 'H')) { //if there is a potion, heal player
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, right); x++;
					heal_player();
					draw_ui();
					break;
				}
				if (((mainGrid[y][x + 1]) == 'X')) { //if there is an exit, start new level
					mainGrid[y][x + 1] = 'P'; mainGrid[y][x] = ' '; x++;
					add_points(2);
					nextLevel(mainGrid);
					break;
				}
				break;

			case key_UP:
				if ((mainGrid[y - 1][x]) == ' ') { //if there is nothing, move there
					mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, up); y--;
					break;
				}
				if (((mainGrid[y - 1][x]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = ' '; y--;
					}
					refresh;
					draw_ui();
					break;
				}
				if (((mainGrid[y - 1][x]) == 'C')) { //if there is a chest, add points
					mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, up); y--;
					add_points(0);
					draw_ui();
					break;
				}
				if (((mainGrid[y - 1][x]) == 'H')) { //if there is a potion, heal player
					mainGrid[y - 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, up); y--;
					heal_player();
					draw_ui();
					break;
				}
				break;

			case key_DOWN:
				if ((mainGrid[y + 1][x]) == ' ') { //if there is nothing, move there
					mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, down); y++;
					break;
				}
				if (((mainGrid[y + 1][x]) == 'E')) { //if there is an enemy, engage combat
					if (engageFight()) {
						mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = ' '; y++;
					}
					refresh;
					draw_ui();
					break;
				}
				if (((mainGrid[y + 1][x]) == 'C')) { //if there is a chest, add points
					mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, down); y++;
					add_points(0);
					draw_ui();
					break;
				}
				if (((mainGrid[y + 1][x]) == 'H')) { //if there is a potion, heal player
					mainGrid[y + 1][x] = 'P'; mainGrid[y][x] = ' ';
					consoleMove(x, y, down); y++;
					heal_player();
					draw_ui();
					break;
				}
				break;
			}
		}
	}
}

void consoleMove(int x, int y, int direction) { //display player new position
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (direction) {
	case 0:
		cPos.Y = y;
		cPos.X = x;
		SetConsoleCursorPosition(hCon, cPos);
		cout << " ";
		cPos.Y = y - 1;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "P";
		break;
	case 1:
		cPos.Y = y;
		cPos.X = x;
		SetConsoleCursorPosition(hCon, cPos);
		cout << " ";
		cPos.X = x + 1;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "P";
		break;
	case 2:
		cPos.Y = y;
		cPos.X = x;
		SetConsoleCursorPosition(hCon, cPos);
		cout << " ";
		cPos.Y = y + 1;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "P";
		break;
	case 3:
		cPos.Y = y;
		cPos.X = x;
		SetConsoleCursorPosition(hCon, cPos);
		cout << " ";
		cPos.X = x - 1;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "P";
		break;
	}
}

void heal_player() {
	if (player_health < 50) {
		player_health += 10;
		if (player_health > 50) {
			player_health = 50;
		}
	}
}

void add_points(int pointsGranted) {
	if (pointsGranted == 0) { //collecting a chest
		points_number += 50;
	}
	if (pointsGranted == 1) { //defeating an enemy
		points_number += 100;
	}
	if (pointsGranted == 2) { //new level
		points_number += 200;
	}
}

void draw_ui() { //draw points meter and player health
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	cPos.Y = dungeon_height;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "LEVEL " << level_number << "   ";

	cPos.Y = dungeon_height + 2;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "[HP]" << player_health << "   ";

	cPos.Y = dungeon_height + 2;
	cPos.X = 8;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "[PT]" << points_number << "   ";


}

bool engageFight() {
	enemy current_enemy = selectEnemy();

	//setting player abilities
	ability powAtt; powAtt.damage = 10; powAtt.cooldown = 2; powAtt.cur_cooldown = 0;

	system("cls");
	cout << "You have encountered a " << current_enemy.name << "!" << endl;

	pause;

	int cooldown = 0;
	while (true) { //battle loop

		bool choose_attack_loop = true;
		int dmgPlayer = rand() % player_damage;
		int dmgEnemy = rand() % current_enemy.base_damage;

		system("cls");
		cout << "You have " << player_health << " HP!" << endl;
		cout << "Enemy has " << current_enemy.health << " HP!" << endl;
		break_line;

		cout << "Your turn, choose an attack: " << endl
			<< "Attack" << "  " << "Power Attack" << "   " << "Run Away" << endl
			<< "  [1]  " << "  " << "    [2]";

		if (powAtt.cur_cooldown > 0) cout << "(" << powAtt.cur_cooldown << ")" << "      [3]" << endl;
		else cout << "         [3]" << endl;

		while (choose_attack_loop) {
			if (_kbhit()) {
				int keypress = _getch();
				switch (keypress) {
				case key_1:
					cout << "You use 'Attack', " << dmgPlayer << " damage delivered to " << current_enemy.name << "!" << endl;
					current_enemy.health -= dmgPlayer;
					cooldown = 1; choose_attack_loop = false; break; //Ability is executed, cooldown for next ability will be lowered

				case key_2:
					if (powAtt.cur_cooldown == 0) {
						cout << "You use 'Power Attack', " << dmgPlayer + powAtt.damage << " damage delivered to " << current_enemy.name << "!" << endl;
						current_enemy.health -= dmgPlayer + powAtt.damage;
						powAtt.cur_cooldown = powAtt.cooldown;
						cooldown = 1; choose_attack_loop = false; break;
					}
					break;

				case key_3:
					cout << "Player tried to run away ";
					for (int i = 0; i < 3; i++) {
						cout << ".";
						Sleep(200);
					}
					int generateRunAway = rand() % 2;
					if (generateRunAway == 0) {
						cout << " and did!" << endl;
						pause;
						return false;
					}
					else {
						cout << " but couldn't!" << endl;
						cooldown = 1; choose_attack_loop = false; break;
					}
				}
			}
		}

		//resetting cooldowns
		if (powAtt.cur_cooldown > 0) powAtt.cur_cooldown -= cooldown;
		cooldown = 0;

		cout << "The enemy has " << current_enemy.health << "HP" << " left!" << endl;

		pause; system("cls");

		if (current_enemy.health <= 0) {
			cout << "The enemy was slain!" << endl;
			add_points(1);
			pause;
			return true;
		}

		cout << "Enemy attacks! ";
		for (int i = 0; i < 3; i++) {
			cout << ".";
			Sleep(200);
		}
		cout << " " << dmgEnemy << " damage delivered to player!" << endl;

		player_health -= dmgEnemy;

		cout << "You have " << player_health << " HP left!" << endl;

		Sleep(200);

		if (player_health <= 0) {
			cout << "Game Over, you are dead!" << endl;
			pause;
			main_menu();
		}
		pause;
	}
	return true;
}

enemy selectEnemy() {
	//init enemies here
	enemy e_skeleton; e_skeleton.base_damage = 5; e_skeleton.health = 20; e_skeleton.name = "skeleton";
	enemy e_battle_skeleton; e_battle_skeleton.base_damage = 10; e_battle_skeleton.health = 40; e_battle_skeleton.name = "battle skeleton";
	enemy e_boss_skeleton; e_boss_skeleton.base_damage = 25; e_boss_skeleton.health = 60; e_boss_skeleton.name = "boss skeleton";

	//pick a random enemy and return
	int enemyRandomizer = rand() % 100;
	if ((enemyRandomizer >= 0) && (enemyRandomizer < 65)) return e_skeleton;
	if ((enemyRandomizer >= 65) && (enemyRandomizer < 85)) return e_battle_skeleton;
	if (enemyRandomizer >= 85) return e_boss_skeleton;
}

int drawGrid(grid mainGrid) {
	for (int i = 0; i < dungeon_height; i++) {
		for (int j = 0; j < dungeon_width; j++) {

			//customise output
			if ((mainGrid[i][j] == '*') || (mainGrid[i][j] == '|')) cout << char(219);
			else if ((mainGrid[i][j] == 'X') || (mainGrid[i][j] == 'X')) cout << char(177);
			else cout << mainGrid[i][j];

		}
		break_line;
	}
	return 0;
}