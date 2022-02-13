#include "GameController.h"
#include "Score.h"
#include <QFont>

Score::Score(QGraphicsItem* parent) : QGraphicsTextItem(parent) {
	currentScore = 0;

	setPlainText(QString("Score: ") + QString::number(currentScore));
	setDefaultTextColor(Qt::white);
	setFont(QFont("Uroob", 24));
	setPos(0, SCREEN_HEIGHT / 1.1);
}

int Score::getCurrentScore() const { return currentScore; }

void Score::increaseScore(int const& amount) {
	currentScore += amount;
	setPlainText(QString("Score: ") + QString::number(currentScore));
}
