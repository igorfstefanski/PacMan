#include "EdgeNode.h"
#include "Enemy.h"
#include "GameController.h"
#include "PacDot.h"
#include "PowerCapsule.h"
#include <QFont>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QWidget>
#include <fstream>

GameController::GameController(QWidget* parent) {
	dotsLeft = 0;
	stateChanges = 0;

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	scene = new QGraphicsScene();
	scene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	setScene(scene);

	readBoard();

	player = new PacMan(kBoardWidth);
	player->setFlag(QGraphicsItem::ItemIsFocusable);
	player->setFocus();

	ghostBlinky = new Blinky(kBoardWidth, kBoardHeight);
	connect(ghostBlinky, SIGNAL(sGameOver()), this, SLOT(gameOver()));
	ghostPinky = new Pinky(kBoardWidth, kBoardHeight);
	connect(ghostPinky, SIGNAL(sGameOver()), this, SLOT(gameOver()));
	ghostInky = new Inky(kBoardWidth, kBoardHeight);
	connect(ghostInky, SIGNAL(sGameOver()), this, SLOT(gameOver()));
	ghostClyde = new Clyde(kBoardWidth, kBoardHeight);
	connect(ghostClyde, SIGNAL(sGameOver()), this, SLOT(gameOver()));

	score = new Score();

	scene->addItem(player);
	scene->addItem(ghostBlinky);
	scene->addItem(ghostPinky);
	scene->addItem(ghostInky);
	scene->addItem(ghostClyde);
	scene->addItem(score);

	drawBoard();

	movementTimer = new QTimer();
	connect(movementTimer, SIGNAL(timeout()), this, SLOT(moveEnemies()));
	movementTimer->start(kGhostMoveSpeed);

	gameTimer = new QTimer();
	connect(gameTimer, SIGNAL(timeout()), this, SLOT(changeState()));
	gameTimer->start(kScatterTime);

	show();
}

void GameController::dotEaten(int points) {
	dotsLeft--;
	score->increaseScore(points);

	if (dotsLeft <= 0) {
		win();
	}
}

void GameController::capsuleEaten(int points) {
	dotsLeft--;
	score->increaseScore(points);

	ghostBlinky->setCurrentState(3);
	ghostPinky->setCurrentState(3);
	ghostInky->setCurrentState(3);
	ghostClyde->setCurrentState(3);

	gameTimer->setInterval(kFrightenTime);

	if (dotsLeft <= 0) {
		win();
	}
}

void GameController::ghostEaten(int points) { score->increaseScore(points); }

void GameController::moveEnemies() {
	ghostBlinky->determineBehaviour(player->getXIndex(), player->getYIndex());
	ghostPinky->determineBehaviour(player->getXIndex(), player->getYIndex(),
								   player->getCurrentDirection());
	ghostInky->determineBehaviour(
		player->getXIndex(), player->getYIndex(), player->getCurrentDirection(),
		ghostBlinky->getXIndex(), ghostBlinky->getYIndex());
	ghostClyde->determineBehaviour(player->getXIndex(), player->getYIndex());
}

void GameController::changeState() {
	if (stateChanges % 2 == 0) {
		if (ghostBlinky->getCurrentState() != 2) {
			ghostBlinky->setCurrentState(0);
		}
		if (ghostPinky->getCurrentState() != 2) {
			ghostPinky->setCurrentState(0);
		}
		if (ghostInky->getCurrentState() != 2) {
			ghostInky->setCurrentState(0);
		}
		if (ghostClyde->getCurrentState() != 2) {
			ghostClyde->setCurrentState(0);
		}
		gameTimer->setInterval(kChaseTime);
	} else {
		if (ghostBlinky->getCurrentState() != 2) {
			ghostBlinky->setCurrentState(1);
		}
		if (ghostPinky->getCurrentState() != 2) {
			ghostPinky->setCurrentState(1);
		}
		if (ghostInky->getCurrentState() != 2) {
			ghostInky->setCurrentState(1);
		}
		if (ghostClyde->getCurrentState() != 2) {
			ghostClyde->setCurrentState(1);
		}
		gameTimer->setInterval(kScatterTime);
	}

	stateChanges++;
}

void GameController::win() {
	stopTimers();
	for (int i = 0; i < scene->items().size(); i++) {
		scene->items()[i]->setEnabled(false);
	}

	QGraphicsTextItem* winText = new QGraphicsTextItem();
	winText->setPlainText(QString("Win!"));
	winText->setDefaultTextColor(Qt::green);
	winText->setFont(QFont("Uroob", 50));
	winText->setPos(SCREEN_WIDTH / 3.5, score->y());

	scene->addItem(winText);
}

void GameController::gameOver() {
	stopTimers();
	for (int i = 0; i < scene->items().size(); i++) {
		scene->items()[i]->setEnabled(false);
	}

	QGraphicsTextItem* gameOverText = new QGraphicsTextItem();
	gameOverText->setPlainText(QString("Game Over!"));
	gameOverText->setDefaultTextColor(Qt::red);
	gameOverText->setFont(QFont("Uroob", 50));
	gameOverText->setPos(SCREEN_WIDTH / 3.5, score->y());

	scene->addItem(gameOverText);
}

void GameController::readBoard() {
	board = new int[kBoardHeight * kBoardWidth];
	std::ifstream file("GameBoard.txt");
	int i = 0, value;

	while (i < kBoardHeight * kBoardWidth && file >> value) {
		board[i++] = value;
	}
}

void GameController::drawBoard() {
	int element = 0;
	for (int i = 0; i < kBoardHeight; i++) {
		for (int j = 0; j < kBoardWidth; j++) {
			element = board[i * kBoardWidth + j];

			switch (element) {
			case 1: {
				EdgeNode* node = new EdgeNode();
				node->setPos(j * GRID_SIZE, i * GRID_SIZE);
				scene->addItem(node);
			} break;
			case 2: {
				PacDot* dot = new PacDot();
				dotsLeft++;
				connect(dot, SIGNAL(sDotEaten(int)), this, SLOT(dotEaten(int)));
				dot->setPos(
					j * GRID_SIZE + (GRID_SIZE / 2 - dot->getSize() / 2),
					i * GRID_SIZE + (GRID_SIZE / 2 - dot->getSize() / 2));
				scene->addItem(dot);
			} break;
			case 3: {
				PowerCapsule* capsule = new PowerCapsule();
				dotsLeft++;
				connect(capsule, SIGNAL(sCapsuleEaten(int)), this,
						SLOT(capsuleEaten(int)));
				capsule->setPos(
					j * GRID_SIZE + (GRID_SIZE / 2 - capsule->getSize() / 2),
					i * GRID_SIZE + (GRID_SIZE / 2 - capsule->getSize() / 2));
				scene->addItem(capsule);
			} break;
			case 4: {
				ghostBlinky->setXIndex(j);
				ghostBlinky->setYIndex(i);
				connect(ghostBlinky, SIGNAL(sGhostEaten(int)), this,
						SLOT(ghostEaten(int)));
				ghostBlinky->setPos(ghostBlinky->getXIndex() * GRID_SIZE +
										ghostBlinky->getEnemyOffset(),
									ghostBlinky->getYIndex() * GRID_SIZE +
										ghostBlinky->getEnemyOffset());
			} break;
			case 5: {
				ghostPinky->setXIndex(j);
				ghostPinky->setYIndex(i);
				connect(ghostPinky, SIGNAL(sGhostEaten(int)), this,
						SLOT(ghostEaten(int)));
				ghostPinky->setPos(ghostPinky->getXIndex() * GRID_SIZE +
									   ghostPinky->getEnemyOffset(),
								   ghostPinky->getYIndex() * GRID_SIZE +
									   ghostPinky->getEnemyOffset());
			} break;
			case 6: {
				ghostInky->setXIndex(j);
				ghostInky->setYIndex(i);
				connect(ghostInky, SIGNAL(sGhostEaten(int)), this,
						SLOT(ghostEaten(int)));
				ghostInky->setPos(ghostInky->getXIndex() * GRID_SIZE +
									  ghostInky->getEnemyOffset(),
								  ghostInky->getYIndex() * GRID_SIZE +
									  ghostInky->getEnemyOffset());
			} break;
			case 7: {
				ghostClyde->setXIndex(j);
				ghostClyde->setYIndex(i);
				connect(ghostClyde, SIGNAL(sGhostEaten(int)), this,
						SLOT(ghostEaten(int)));
				ghostClyde->setPos(ghostClyde->getXIndex() * GRID_SIZE +
									   ghostClyde->getEnemyOffset(),
								   ghostClyde->getYIndex() * GRID_SIZE +
									   ghostClyde->getEnemyOffset());
			} break;
			case 8: {
				player->setXIndex(j);
				player->setYIndex(i);
				player->setPos(j * GRID_SIZE + player->getPlayerOffset(),
							   i * GRID_SIZE + player->getPlayerOffset());
			} break;
			default:
				break;
			}
		}
	}
}

void GameController::stopTimers() {
	player->movementTimer->stop();

	movementTimer->stop();
	gameTimer->stop();

	ghostBlinky->collideTimer->stop();
	ghostPinky->collideTimer->stop();
	ghostInky->collideTimer->stop();
	ghostClyde->collideTimer->stop();
}
