#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>

class PowerCapsule : public QObject, public QGraphicsEllipseItem {
	Q_OBJECT

  public:
	PowerCapsule(QGraphicsItem* parent = NULL);

	int getSize() const;
	int getPoints() const;

  public slots:
	void collidesWithPacMan();

  signals:
	void sCapsuleEaten(int points);

  private:
	const int kSize = 12;
	const int kPoints = 50;
};
