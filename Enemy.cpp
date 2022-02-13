#include "EdgeNode.h"
#include "Enemy.h"
#include "GameController.h"
#include "PacMan.h"
#include <QBrush>
#include <QList>
#include <QTimer>
#include <QtMath>
#include <fstream>
#include <iostream>
#include <stdlib.h>

int Enemy::getMoveStep() const { return kMoveStep; }

int Enemy::getSize() const { return kSize; }

int Enemy::getEnemyOffset() const { return kEnemyOffset; }

int Enemy::getXIndex() const { return xIndex; }

int Enemy::getYIndex() const { return yIndex; }

int Enemy::getCurrentState() const { return currentState; }

void Enemy::setXIndex(int const& index) { xIndex = index; }

void Enemy::setYIndex(int const& index) { yIndex = index; }

void Enemy::setCurrentState(int const& newState) {
	switch (newState) {
	case 0:
		currentState = CHASE;
		resetColor();
		break;
	case 1:
		currentState = SCATTER;
		resetColor();
		break;
	case 2:
		currentState = EATEN;
		break;
	case 3:
		currentState = FRIGHTENED;
		setFrightenColor();
		break;
	default:
		break;
	}
}

void Enemy::collidesWithPacMan() {
	QList<QGraphicsItem*> collidingWith = collidingItems();

	for (int i = 0; i < collidingWith.size(); i++) {
		if (typeid(*(collidingWith[i])) == typeid(PacMan)) {
			if (currentState != FRIGHTENED && currentState != EATEN) {
				emit sGameOver();
			} else if (currentState == FRIGHTENED) {
				currentState = EATEN;
				setEatenColor();
				emit sGhostEaten(kPoints);
			}
		}
	}
}

Enemy::Direction Enemy::getRandomDirection() {
	srand(time(NULL));
	return static_cast<Direction>(rand() % (RIGHT + 1));
}

void Enemy::determineBehaviour(int playerXIndex, int playerYIndex,
							   int playerDirection, int blinkyXIndex,
							   int blinkyYIndex) {
	switch (currentState) {
	case CHASE:
		determineChaseTarget(playerXIndex, playerYIndex, playerDirection,
							 blinkyXIndex, blinkyXIndex);
		currentTarget = chaseTarget;
		break;
	case SCATTER:
		currentTarget = scatterTarget;
		break;
	case EATEN:
		eatenBehaviour();
		break;
	case FRIGHTENED:
		currentTarget = scatterTarget;
		break;
	default:
		break;
	}

	if ((((int)y() - kEnemyOffset) % GRID_SIZE == 0) &&
		(((int)x() - kEnemyOffset) % GRID_SIZE == 0)) {
		choosePath();
	}
	move();
}

void Enemy::eatenBehaviour() {
	if (xIndex == eatenTarget[0] && yIndex == eatenTarget[1]) {
		pathsBoard[(yIndex + 1) * kBoardWidth + xIndex] = 1;
		pathsBoard[(yIndex + 1) * kBoardWidth + (xIndex + 1)] = 1;
		currentState = SCATTER;
		resetColor();
	} else {
		currentTarget = eatenTarget;
	}
}

void Enemy::choosePath() {
	float minDistance = 1000;
	int index = 0;

	arrayReset();
	if (currentDirection == UP) {
		if (pathsBoard[(yIndex - 1) * kBoardWidth + xIndex] == 0) {
			distances[UP] = calculateDistance(
				xIndex, yIndex - 1, currentTarget[0], currentTarget[1]);
		}
		if (pathsBoard[yIndex * kBoardWidth + (xIndex - 1)] == 0) {
			distances[LEFT] = calculateDistance(
				xIndex - 1, yIndex, currentTarget[0], currentTarget[1]);
		}
		if (pathsBoard[yIndex * kBoardWidth + (xIndex + 1)] == 0) {
			distances[RIGHT] = calculateDistance(
				xIndex + 1, yIndex, currentTarget[0], currentTarget[1]);
		}
	} else if (currentDirection == LEFT) {
		if (xIndex > 0) {
			if (pathsBoard[(yIndex - 1) * kBoardWidth + xIndex] == 0) {
				distances[UP] = calculateDistance(
					xIndex, yIndex - 1, currentTarget[0], currentTarget[1]);
			}
			if (pathsBoard[yIndex * kBoardWidth + (xIndex - 1)] == 0) {
				distances[LEFT] = calculateDistance(
					xIndex - 1, yIndex, currentTarget[0], currentTarget[1]);
			}
			if (pathsBoard[(yIndex + 1) * kBoardWidth + xIndex] == 0) {
				distances[DOWN] = calculateDistance(
					xIndex, yIndex + 1, currentTarget[0], currentTarget[1]);
			}
		} else {
			distances[LEFT] = 0;
		}
	} else if (currentDirection == DOWN) {
		if (pathsBoard[yIndex * kBoardWidth + (xIndex - 1)] == 0) {
			distances[LEFT] = calculateDistance(
				xIndex - 1, yIndex, currentTarget[0], currentTarget[1]);
		}
		if (pathsBoard[(yIndex + 1) * kBoardWidth + xIndex] == 0) {
			distances[DOWN] = calculateDistance(
				xIndex, yIndex + 1, currentTarget[0], currentTarget[1]);
		}
		if (pathsBoard[yIndex * kBoardWidth + (xIndex + 1)] == 0) {
			distances[RIGHT] = calculateDistance(
				xIndex + 1, yIndex, currentTarget[0], currentTarget[1]);
		}
	} else if (currentDirection == RIGHT) {
		if (xIndex < kBoardWidth - 1) {
			if (pathsBoard[(yIndex - 1) * kBoardWidth + xIndex] == 0) {
				distances[UP] = calculateDistance(
					xIndex, yIndex - 1, currentTarget[0], currentTarget[1]);
			}
			if (pathsBoard[(yIndex + 1) * kBoardWidth + xIndex] == 0) {
				distances[DOWN] = calculateDistance(
					xIndex, yIndex + 1, currentTarget[0], currentTarget[1]);
			}
			if (pathsBoard[yIndex * kBoardWidth + (xIndex + 1)] == 0) {
				distances[RIGHT] = calculateDistance(
					xIndex + 1, yIndex, currentTarget[0], currentTarget[1]);
			}
		} else {
			distances[RIGHT] = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (distances[i] < minDistance) {
			minDistance = distances[i];
			index = i;
		}
	}

	switch (index) {
	case 0:
		currentDirection = UP;
		break;
	case 1:
		currentDirection = LEFT;
		break;
	case 2:
		currentDirection = DOWN;
		break;
	case 3:
		currentDirection = RIGHT;
		break;
	default:
		break;
	}
}

float Enemy::calculateDistance(int x1, int y1, int x2, int y2) {
	return qSqrt(qPow(x1 - x2, 2) + qPow(y1 - y2, 2));
}

void Enemy::arrayReset() {
	distances[UP] = 1000;
	distances[LEFT] = 1000;
	distances[DOWN] = 1000;
	distances[RIGHT] = 1000;
}

void Enemy::move() {
	switch (currentDirection) {
	case UP:
		setPos(x(), y() - kMoveStep);
		if (((int)y() - kEnemyOffset) % GRID_SIZE == 0) {
			yIndex--;
		}
		break;
	case LEFT:
		setPos(x() - kMoveStep, y());
		if (x() <= -kSize) {
			setPos(SCREEN_WIDTH - kSize - kEnemyOffset, y());
			xIndex = kBoardWidth - 1;
		} else {
			if (((int)x() - kEnemyOffset) % GRID_SIZE == 0) {
				xIndex--;
			}
		}
		break;
	case DOWN:
		setPos(x(), y() + kMoveStep);
		if (((int)y() - kEnemyOffset) % GRID_SIZE == 0) {
			yIndex++;
		}
		break;
	case RIGHT:
		setPos(x() + kMoveStep, y());
		if (x() >= SCREEN_WIDTH) {
			setPos(kEnemyOffset, y());
			xIndex = 0;
		} else {
			if (((int)x() - kEnemyOffset) % GRID_SIZE == 0) {
				xIndex++;
			}
		}
		break;
	default:
		break;
	}
}

void Enemy::setFrightenColor() {
	setBrush(QBrush(Qt::magenta, Qt::SolidPattern));
	setPen(Qt::NoPen);
}

void Enemy::setEatenColor() {
	setBrush(QBrush(Qt::gray, Qt::SolidPattern));
	setPen(Qt::NoPen);
}

void Enemy::readBoard() {
	std::ifstream file("PathsBoard.txt");
	int i = 0, value;

	while (i < kBoardHeight * kBoardWidth && file >> value) {
		pathsBoard[i++] = value;
	}
}
