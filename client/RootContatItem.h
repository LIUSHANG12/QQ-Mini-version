#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContatItem  : public QLabel
{
	Q_OBJECT

	// ��ͷ�Ƕ�
		Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContatItem(bool hasArrow = true, QWidget *parent = nullptr);
	~RootContatItem();

public:
	void setText(const QString& title);         
	void setExpanded(bool expand);               

private:
	int rotation();                           
	void setRotation(int rotation);           

protected:
	void paintEvent(QPaintEvent* event);      

private:
	QPropertyAnimation *m_animation;          // ��ͷ��ת����
	QString m_titleText;                     // �����ı�
	int m_rotation;                          // ��ͷ�ĽǶ�
	bool m_hasArrow;                         // �Ƿ��м�ͷ
};
