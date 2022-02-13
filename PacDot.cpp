#include "PacDot.h"
#include "PacMan.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QList>
#include <QPen>
#include <QTimer>

PacDot::PacDot(QGraphicsItem* parent) {
	setRect(0, 0, kSize, kSize);
	setBrush(QBrush(Qt::white, Qt::SolidPattern));
	setPen(Qt::NoPen);

	QTimer* timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(collidesWithPacMan()));
	timer->start(50);
}

int PacDot::getSize() const { return kSize; }

int PacDot::getPoints() const { return kPoints; }

void PacDot::collidesWithPacMan() {
	QList<QGraphicsItem*> collidingWith = collidingItems();

	for (int i = 0; i < collidingWith.size(); i++) {
		if (typeid(*(collidingWith[i])) == typeid(PacMan)) {
			emit sDotEaten(kPoints);
			scene()->removeItem(this);
			delete this;
		}
	}
}
