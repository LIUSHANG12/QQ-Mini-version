#include "QClickLabel.h"
#include <QMouseEvent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{}

QClickLabel::~QClickLabel()
{}

void QClickLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked(); // 按下鼠标左键发出点击信号
	}
}
