#pragma once

#include <QObject>

class NotifyManager  : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	void signalSkinChanged(const QColor& color);           // ��ɫ�ı��ź�

public:
	static NotifyManager* getInstance();                   // ��ȡ����ʵ��

	void notifyOtherWindowChangeSkin(const QColor& color); // ֪ͨ�������ڸı���ɫ

private:
	static NotifyManager* instance;                        // ����ʵ��
};
