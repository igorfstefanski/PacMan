#pragma once

#include "Blinky.h"
#include "Clyde.h"
#include "Inky.h"
#include "PacMan.h"
#include "Pinky.h"
#include "Score.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QWidget>

#define SCREEN_WIDTH 560
#define SCREEN_HEIGHT 700
#define GRID_SIZE 20

class GameController : public QGraphicsView {
	Q_OBJECT

  public:
	QGraphicsScene* scene;
	PacMan* player;
	Score* score;

	Blinky* ghostBlinky;
	Pinky* ghostPinky;
	Inky* ghostInky;
	Clyde* ghostClyde;

	GameController(QWidget* parent = NULL);

  public slots:
	void dotEaten(int points);
	void capsuleEaten(int points);
	void ghostEaten(int points);
	void moveEnemies();
	void changeState();
	void win();
	void gameOver();

  private:
	const int kBoardWidth = 28;
	const int kBoardHeight = 31;
	const int kGhostMoveSpeed = 50;
	const int kChaseTime = 20000;
	const int kScatterTime = 7000;
	const int kFrightenTime = 5000;

	QTimer* movementTimer;
	QTimer* gameTimer;
	int stateChanges;
	int* board;
	int dotsLeft;

	void readBoard();
	void drawBoard();
	void stopTimers();
};
