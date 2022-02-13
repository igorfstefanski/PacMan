#include "Clyde.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

Clyde::Clyde(int boardWidth, int boardHeight, QGraphicsItem* parent) {
	kBoardWidth = boardWidth;
	kBoardHeight = boardHeight;
	distances = new float[4];
	pathsBoard = new int[kBoardHeight * kBoardWidth];
	readBoard();

	currentTarget = new int[2];
	chaseTarget = new int[2];
	scatterTarget = new int[2];
	kXScatterTarget = 0;
	kYScatterTarget = kBoardHeight - 1;
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

void Clyde::resetColor() {
	QColor orange(255, 131, 0);
	setBrush(QBrush(orange));
	setPen(Qt::NoPen);
}

void Clyde::determineChaseTarget(int playerXIndex, int playerYIndex,
								 int playerDirection, int blinkyXIndex,
								 int blinkyYIndex) {
	if (calculateDistance(xIndex, yIndex, playerXIndex, playerYIndex) >
		kPlayerRadius) {
		chaseTarget[0] = playerXIndex;
		chaseTarget[1] = playerYIndex;
	} else {
		chaseTarget = scatterTarget;
	}
}
