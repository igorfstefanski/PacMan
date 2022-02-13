#pragma once

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>

class Enemy : public QObject, public QGraphicsRectItem {
	Q_OBJECT

  public:
	int* pathsBoard;
	QTimer* collideTimer;

	int getMoveStep() const;
	int getSize() const;
	int getEnemyOffset() const;
	int getXIndex() const;
	int getYIndex() const;
	int getCurrentState() const;

	void setXIndex(int const& index);
	void setYIndex(int const& index);
	void setCurrentState(int const& newState);

	void determineBehaviour(int playerXIndex, int playerYIndex,
							int playerDirection = 0, int blinkyXIndex = 0,
							int blinkyYIndex = 0);

  public slots:
	void collidesWithPacMan();

  signals:
	void sGameOver();
	void sGhostEaten(int points);

  protected:
	enum Direction { UP, LEFT, DOWN, RIGHT };
	Enemy::Direction currentDirection;
	Enemy::Direction getRandomDirection();

	enum State { CHASE, SCATTER, EATEN, FRIGHTENED };
	Enemy::State currentState;

	const int kMoveStep = 5;
	const int kSize = 18;
	const int kEnemyOffset = 1;
	const int kPoints = 200;
	int kBoardWidth;
	int kBoardHeight;

	int xIndex;
	int yIndex;
	int* currentTarget;
	int* chaseTarget;
	int* scatterTarget;
	int eatenTarget[2] = {13, 11};
	float* distances;

	virtual void resetColor() = 0;
	virtual void determineChaseTarget(int playerXIndex, int playerYIndex,
									  int playerDirection, int blinkyXIndex,
									  int blinkyYIndex) = 0;
	void eatenBehaviour();

	void choosePath();
	float calculateDistance(int x1, int y1, int x2, int y2);
	void arrayReset();
	void move();
	void setFrightenColor();
	void setEatenColor();
	void readBoard();
};
