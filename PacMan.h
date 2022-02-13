#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>

class PacMan : public QObject, public QGraphicsEllipseItem {
	Q_OBJECT

  public:
	QTimer* movementTimer;

	PacMan(int boardWidth, QGraphicsItem* parent = NULL);

	int getSize() const;
	int getXIndex() const;
	int getYIndex() const;
	int getPlayerOffset() const;
	int getCurrentDirection() const;

	void setXIndex(int const& newIndex);
	void setYIndex(int const& newIndex);

	void keyPressEvent(QKeyEvent* event);

  private slots:
	void move();

  private:
	enum Direction { UP, LEFT, DOWN, RIGHT };
	PacMan::Direction currentDirection;

	const int kMoveStep = 5;
	const int kSize = 16;
	const int kPlayerOffset = 2;
	int kBoardWidth;
	int xIndex;
	int yIndex;

	bool isCollidingWithEdge() const;
};
