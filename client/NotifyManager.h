#pragma once

#include <QObject>

class NotifyManager  : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	void signalSkinChanged(const QColor& color);           // 颜色改变信号

public:
	static NotifyManager* getInstance();                   // 获取操作实例

	void notifyOtherWindowChangeSkin(const QColor& color); // 通知其它窗口改变颜色

private:
	static NotifyManager* instance;                        // 操作实例
};
