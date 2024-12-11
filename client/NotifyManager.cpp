#include "NotifyManager.h"
#include "CommonUtils.h"

NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager()
	:QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}
NotifyManager* NotifyManager::getInstance()
{
	if (instance == nullptr)
	{   // 如果操作实例为空则构造一个操作实例
		instance = new NotifyManager();
	}
	return instance;
}

void NotifyManager::notifyOtherWindowChangeSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);
}