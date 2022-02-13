#pragma once

#include "Enemy.h"

class Clyde : public Enemy {
	Q_OBJECT

  public:
	Clyde(int boardWidth, int boardHeight, QGraphicsItem* parent = NULL);

  private:
	int kXScatterTarget;
	int kYScatterTarget;
	int kPlayerRadius = 8;

	void resetColor();
	void determineChaseTarget(int playerXIndex, int playerYIndex,
							  int playerDirection, int blinkyXIndex,
							  int blinkyYIndex);
};
