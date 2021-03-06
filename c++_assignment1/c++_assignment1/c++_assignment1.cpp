// c++_assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <conio.h>
#include <functional> 

class MapItem {
public:
	int x, y; 
	MapItem() {};
	MapItem(int x, int y) : x(x), y(y) {}
};

class Player: public MapItem {
public:
	std::string name;
	bool isDead = false;
};

class Monster: public MapItem {
public:
	Monster() {}
	std::string name;
	std::function<MapItem(Player, Monster)> moveWay;
	void move(Player gameplayer) {
		MapItem result = moveWay(gameplayer, *this);
		x = result.x;
		y = result.y;
		return;
	}
};

class EscapeGame {
public:
	Player gameplayer;
	Monster* monsters;
	int monsterMoveWayNumber = 2;
	std::function<MapItem(Player, Monster)> monsterMoveWays[2] = { 
		[](Player player, Monster monster)-> MapItem { return MapItem(monster.x+1, monster.y+1); },
		[](Player player, Monster monster)-> MapItem {
			int x_value = 0, y_value = 0;
			if (player.x > monster.x) x_value = 1;
			if (player.x < monster.x) x_value = -1;
			if (player.y > monster.y) y_value = 1;
			if (player.y < monster.y) y_value = -1;
			return MapItem(monster.x + x_value, monster.y + y_value);
		}};
	
	int monsterNum;
	int lowerboundX = -100, upperboundX = 100;
	int lowerboundY = -100, upperboundY = 100;
	void GeneratePosition(MapItem &source) {
		source.x = rand() % (upperboundX - lowerboundX + 1) + lowerboundX;
		source.y = rand() % (upperboundY - lowerboundY + 1) + lowerboundY;
	}

	void GenerateMoveWayForMonster(Monster &source) {
		source.moveWay = monsterMoveWays[rand() % monsterMoveWayNumber];
	}

	void MonsterTurnMove() {
		for (int i = 0; i < monsterNum; i++) {
			monsters[i].move(gameplayer);
			if (monsters[i].x == gameplayer.x && monsters[i].y == gameplayer.y)
				gameplayer.isDead = true;
		}
		if (rand() % 50 == 1) {
			RefreshMonster(rand() % monsterNum);
		}
	}

	void PrintStatus() {
		std::cout << "Player " << gameplayer.name << " is now at (";
		std::cout << gameplayer.x << "," << gameplayer.y << ")." << std::endl;
		for (int i = 0; i < monsterNum; i++) {
			std::cout << "Monster " << monsters[i].name << " is now at (";
			std::cout << monsters[i].x << "," << monsters[i].y << ")." << std::endl;
		}
	}

	void RefreshMonster(int id) {
		std::cout << "Refreshing monster #" << id << " whose name is " << monsters[id].name << std::endl;
		std::cout << "Input new monster name: ";
		std::cin >> monsters[id].name;
		GeneratePosition(monsters[id]);
		GenerateMoveWayForMonster(monsters[id]);
	}
};

int main()
{
	srand(time(NULL));
	EscapeGame mainGame;
	std::cout << "Input player name:";
	std::cin >> mainGame.gameplayer.name;
	mainGame.GeneratePosition(mainGame.gameplayer);

	std::cout << "number of monsters:";
	std::cin >> mainGame.monsterNum;
	mainGame.monsters = new Monster[mainGame.monsterNum];
	for (int i = 0; i < mainGame.monsterNum; i++) {
		std::cout << "Input the name of monster #" << i << ":" << std::endl;
		std::cin >> mainGame.monsters[i].name;
		mainGame.GeneratePosition(mainGame.monsters[i]);
		mainGame.GenerateMoveWayForMonster(mainGame.monsters[i]);
	}
	mainGame.PrintStatus();
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			switch (toupper(ch)) {
			case 'A': mainGame.gameplayer.y--; mainGame.MonsterTurnMove(); mainGame.PrintStatus(); break;
			case 'W': mainGame.gameplayer.x++; mainGame.MonsterTurnMove(); mainGame.PrintStatus(); break;
			case 'S': mainGame.gameplayer.x--; mainGame.MonsterTurnMove(); mainGame.PrintStatus(); break;
			case 'D': mainGame.gameplayer.y++; mainGame.MonsterTurnMove(); mainGame.PrintStatus(); break;
			}
			if (mainGame.gameplayer.isDead) { std::cout << "You Died!" << std::endl; break; }
			if (toupper(ch) == 'Q') { break; }
			
		}
	}
 }

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
