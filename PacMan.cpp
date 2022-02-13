#include "EdgeNode.h"
#include "GameController.h"
#include "PacMan.h"
#include <QBrush>
#include <QKeyEvent>
#include <QPen>
#include <QTimer>

PacMan::PacMan(int boardWidth, QGraphicsItem* parent) {
	kBoardWidth = boardWidth;
	setRect(0, 0, getSize(), getSize());
	setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
	setPen(Qt::NoPen);

	currentDirection = UP;
	movementTimer = new QTimer();
	connect(movementTimer, SIGNAL(timeout()), this, SLOT(move()));
	movementTimer->start(50);
}

int PacMan::getSize() const { return kSize; }

int PacMan::getXIndex() const { return xIndex; }

int PacMan::getYIndex() const { return yIndex; }

int PacMan::getPlayerOffset() const { return kPlayerOffset; }

int PacMan::getCurrentDirection() const { return currentDirection; }

void PacMan::setXIndex(int const& newIndex) { xIndex = newIndex; }

void PacMan::setYIndex(int const& newIndex) { yIndex = newIndex; }

bool PacMan::isCollidingWithEdge() const {
	QList<QGraphicsItem*> collidingWith = collidingItems();

	for (int i = 0; i < collidingWith.size(); i++) {
		if (typeid(*(collidingWith[i])) == typeid(EdgeNode)) {
			return true;
		}
	}

	return false;
}

void PacMan::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
		setSpanAngle(360 * 14);
		setStartAngle(360 * 5);
		currentDirection = UP;
	} else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
		setSpanAngle(360 * 14);
		setStartAngle(360 * 9);
		currentDirection = LEFT;
	} else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
		setSpanAngle(360 * 14);
		setStartAngle(360 * 13);
		currentDirection = DOWN;
	} else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
		setSpanAngle(360 * 14);
		setStartAngle(360);
		currentDirection = RIGHT;
	}
}

void PacMan::move() {
	switch (currentDirection) {
	case UP:
		setPos(x(), y() - kMoveStep);
		if (isCollidingWithEdge()) {
			setPos(x(), y() + kMoveStep);
		} else {
			yIndex = (y() - kPlayerOffset) / GRID_SIZE;
		}
		break;
	case LEFT:
		setPos(x() - kMoveStep, y());

		if (isCollidingWithEdge()) {
			setPos(x() + kMoveStep, y());
		} else if (x() <= -kSize) {
			setPos(SCREEN_WIDTH - kSize - kPlayerOffset, y());
			xIndex = kBoardWidth - 1;
		} else {
			xIndex = (x() - kPlayerOffset) / GRID_SIZE;
		}
		break;
	case DOWN:
		setPos(x(), y() + kMoveStep);

		if (isCollidingWithEdge()) {
			setPos(x(), y() - kMoveStep);
		} else {
			yIndex = (y() - kPlayerOffset) / GRID_SIZE;
		}
		break;
	case RIGHT:
		setPos(x() + kMoveStep, y());

		if (isCollidingWithEdge()) {
			setPos(x() - kMoveStep, y());
		} else if (x() >= SCREEN_WIDTH) {
			setPos(kPlayerOffset, y());
			xIndex = 0;
		} else {
			xIndex = (x() - kPlayerOffset) / GRID_SIZE;
		}
		break;
	default:
		break;
	}
}
