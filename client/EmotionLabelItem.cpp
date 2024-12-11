#include "EmotionLabelItem.h"
#include <QMovie>

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLabel(parent)
{
	initControl();
	connect(this, &QClickLabel::clicked, [this] {
		emit emotionClicked(m_emotionName);
	});
}

EmotionLabelItem::~EmotionLabelItem()
{}

void EmotionLabelItem::setEmotionName(int emotionName)
{
	// ���ñ�������
	m_emotionName = emotionName;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionName);
	// Ϊ�������ö�ͼ
	m_apngMovie = new QMovie(imageName, "apng", this);
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);
}

void EmotionLabelItem::initControl()
{
	setAlignment(Qt::AlignCenter);                // ���ж���
	setObjectName("emotionLabelItem");            // ������
	setFixedSize(32, 32);                         // ���ù̶���С
}