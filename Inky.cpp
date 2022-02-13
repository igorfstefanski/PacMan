#include "Inky.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

Inky::Inky(int boardWidth, int boardHeight, QGraphicsItem* parent) {
	kBoardWidth = boardWidth;
	kBoardHeight = boardHeight;
	distances = new float[4];
	pathsBoard = new int[kBoardHeight * kBoardWidth];
	readBoard();

	currentTarget = new int[2];
	chaseTarget = new int[2];
	scatterTarget = new int[2];
	kXScatterTarget = kBoardWidth - 1;
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

void Inky::resetColor() {
	setBrush(QBrush(Qt::cyan, Qt::SolidPattern));
	setPen(Qt::NoPen);
}

void Inky::determineChaseTarget(int playerXIndex, int playerYIndex,
								int playerDirection, int blinkyXIndex,
								int blinkyYIndex) {
	chaseTarget[0] = calculateSymetricIndex(playerXIndex, blinkyXIndex, X);
	chaseTarget[1] = calculateSymetricIndex(playerYIndex, blinkyYIndex, Y);
}

int Inky::calculateSymetricIndex(int a, int b, symetricIndex mode) {
	int result = (2 * a - b);

	if (result < 0) {
		return 0;
	}

	if (mode == X) {
		if (result > kBoardWidth - 1) {
			return kBoardWidth - 1;
		}
	} else if (mode == Y) {
		if (result > kBoardHeight - 1) {
			return kBoardHeight - 1;
		}
	}

	return result;
}
