#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>

class PacDot : public QObject, public QGraphicsEllipseItem {
	Q_OBJECT

  public:
	PacDot(QGraphicsItem* parent = NULL);

	int getSize() const;
	int getPoints() const;

  public slots:
	void collidesWithPacMan();

  signals:
	void sDotEaten(int points);

  private:
	const int kSize = 6;
	const int kPoints = 10;
};
