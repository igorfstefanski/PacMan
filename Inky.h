#pragma once

#include "Enemy.h"

class Inky : public Enemy {
	Q_OBJECT

  public:
	Inky(int boardWidth, int boardHeight, QGraphicsItem* parent = NULL);

  private:
	enum symetricIndex { X, Y };
	int kXScatterTarget;
	int kYScatterTarget;

	void resetColor();
	void determineChaseTarget(int playerXIndex, int playerYIndex,
							  int playerDirection, int blinkyXIndex,
							  int blinkyYIndex);
	int calculateSymetricIndex(int a, int b, symetricIndex mode);
};
