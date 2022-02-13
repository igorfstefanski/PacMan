#pragma once

#include <QGraphicsTextItem>

class Score : public QGraphicsTextItem {
  public:
	Score(QGraphicsItem* parent = NULL);

	int getCurrentScore() const;

	void increaseScore(int const& amount);

  private:
	int currentScore;
};
