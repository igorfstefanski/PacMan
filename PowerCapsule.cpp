#include "PacMan.h"
#include "PowerCapsule.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QList>
#include <QPen>
#include <QTimer>

PowerCapsule::PowerCapsule(QGraphicsItem* parent) {
	setRect(0, 0, kSize, kSize);
	setBrush(QBrush(Qt::red, Qt::SolidPattern));
	setPen(Qt::NoPen);

	QTimer* timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(collidesWithPacMan()));
	timer->start(50);
}

int PowerCapsule::getSize() const { return kSize; }

int PowerCapsule::getPoints() const { return kPoints; }

void PowerCapsule::collidesWithPacMan() {
	QList<QGraphicsItem*> collidingWith = collidingItems();

	for (int i = 0; i < collidingWith.size(); i++) {
		if (typeid(*(collidingWith[i])) == typeid(PacMan)) {
			emit sCapsuleEaten(kPoints);
			scene()->removeItem(this);
			delete this;
		}
	}
}
