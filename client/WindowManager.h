#pragma once

#include <QObject>
#include "TalkWindowShell.h"

class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	QWidget* findWindowName(const QString& qsWindowName);         
	void deleteWindowName(const QString& qsWindowName);           
	void addWindowName(const QString& qsWindowName, QWidget* qWidget); 

	static WindowManager * getInstance();
	TalkWindowShell* getTalkWindowShell();
	QString getCreatingTalkId();
	// 添加新的聊天窗口
	void addNewTalkWindow(const QString& uid/*, GroupType groupType = COMPANY, const QString& strPeople = ""*/);

private:
	TalkWindowShell* m_talkWindowshell;                          
	QMap<QString, QWidget*> m_windowMap;   
	QString m_strCreatingTalkId = "";          // 正在创建的聊天窗口
};
