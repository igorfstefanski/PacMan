#include "Pinky.h"
#include <QBrush>
#include <QPen>
#include <QTimer>

Pinky::Pinky(int boardWidth, int boardHeight, QGraphicsItem* parent) {
	kBoardWidth = boardWidth;
	kBoardHeight = boardHeight;
	distances = new float[4];
	pathsBoard = new int[kBoardHeight * kBoardWidth];
	readBoard();

	currentTarget = new int[2];
	chaseTarget = new int[2];
	scatterTarget = new int[2];
	kXScatterTarget = 0;
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

void Pinky::resetColor() {
	QColor pink(253, 215, 228);
	setBrush(QBrush(pink));
	setPen(Qt::NoPen);
}

void Pinky::determineChaseTarget(int playerXIndex, int playerYIndex,
								 int playerDirection, int blinkyXIndex,
								 int blinkyYIndex) {
	switch (playerDirection) {
	case 0:
		chaseTarget[0] = playerXIndex;
		chaseTarget[1] = playerYIndex - kChaseTargetOffset;
		break;
	case 1:
		chaseTarget[0] = playerXIndex - kChaseTargetOffset;
		chaseTarget[1] = playerYIndex;
		break;
	case 2:
		chaseTarget[0] = playerXIndex;
		chaseTarget[1] = playerYIndex + kChaseTargetOffset;
		break;
	case 3:
		chaseTarget[0] = playerXIndex + kChaseTargetOffset;
		chaseTarget[1] = playerYIndex;
		break;
	default:
		chaseTarget = scatterTarget;
		break;
	}
}
