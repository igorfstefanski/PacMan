#pragma once

#include <QGraphicsRectItem>

class EdgeNode : public QGraphicsRectItem {
  public:
	EdgeNode(QGraphicsItem* parent = NULL);

	int getSize() const;

  private:
	const int kSize = 20;
};
