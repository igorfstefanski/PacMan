#include "Blinky.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

Blinky::Blinky(int boardWidth, int boardHeight, QGraphicsItem* parent) {
	kBoardWidth = boardWidth;
	kBoardHeight = boardHeight;
	distances = new float[4];
	pathsBoard = new int[kBoardHeight * kBoardWidth];
	readBoard();

	currentTarget = new int[2];
	chaseTarget = new int[2];
	scatterTarget = new int[2];
	kXScatterTarget = kBoardWidth - 1;
	kYScatterTarget = 0;
	scatterTarget[0] = kXScatterTarget;
	scatterTarget[1] = kYScatterTarget;

	currentDirection = UP;
	currentState = EATEN;

	setRect(0, 0, kSize, kSize);
	resetColor();

	collideTimer = new QTimer();
	connect(collideTimer, SIGNAL(timeout()), this, SLOT(collidesWithPacMan()));
	collideTimer->start(50);
}

void Blinky::resetColor() {
	setBrush(QBrush(Qt::red, Qt::SolidPattern));
	setPen(Qt::NoPen);
}

void Blinky::determineChaseTarget(int playerXIndex, int playerYIndex,
								  int playerDirection, int blinkyXIndex,
								  int blinkyYIndex) {
	chaseTarget[0] = playerXIndex;
	chaseTarget[1] = playerYIndex;
}
