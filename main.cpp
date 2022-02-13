/*
 * TITLE:   TETRIS IN C++
 * AUTHOR:  Igor Stefański
 * DATE:    January 2021
 * DESCRIPTION:
 * Popular Pac-Man game in C++, using Qt5 library.
 * 
 * Task was introduced on Object-Oriented Programming in C++ course.
 * 2020/2021 Academic Year
 * Technical University of Lodz, Computer Science
 */

#include "GameController.h"
#include <QApplication>

GameController* game;

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	game = new GameController();
	game->show();

	return app.exec();
}
