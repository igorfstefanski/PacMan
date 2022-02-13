#include "EdgeNode.h"
#include <QBrush>

EdgeNode::EdgeNode(QGraphicsItem* parent) {
	setRect(0, 0, kSize, kSize);
	setBrush(QBrush(Qt::blue, Qt::SolidPattern));
}

int EdgeNode::getSize() const { return kSize; }
