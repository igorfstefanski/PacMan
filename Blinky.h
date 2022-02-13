#pragma once

#include "Enemy.h"

class Blinky : public Enemy {
	Q_OBJECT

  public:
	Blinky(int boardWidth, int boardHeight, QGraphicsItem* parent = NULL);

  private:
	int kXScatterTarget;
	int kYScatterTarget;

	void resetColor();
	void determineChaseTarget(int playerXIndex, int playerYIndex,
							  int playerDirection, int blinkyXIndex,
							  int blinkyYIndex);
};
