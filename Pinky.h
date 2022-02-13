#pragma once

#include "Enemy.h"

class Pinky : public Enemy {
	Q_OBJECT

  public:
	Pinky(int boardWidth, int boardHeight, QGraphicsItem* parent = NULL);

  private:
	int kXScatterTarget;
	int kYScatterTarget;
	int kChaseTargetOffset = 4;

	void resetColor();
	void determineChaseTarget(int playerXIndex, int playerYIndex,
							  int playerDirection, int blinkyXIndex,
							  int blinkyYIndex);
};
