#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include <QSqlQueryModel>

// 单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstance)

WindowManager::WindowManager()
	:QObject(nullptr)
	,m_talkWindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

QWidget* WindowManager::findWindowName(const QString& qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);
	}

	return nullptr;
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName))
	{
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager * WindowManager::getInstance()
{
	return theInstance;
}

TalkWindowShell* WindowManager::getTalkWindowShell()
{
	return m_talkWindowshell;
}

QString WindowManager::getCreatingTalkId()
{
	return m_strCreatingTalkId;
}

void WindowManager::addNewTalkWindow(const QString& uid/*, GroupType groupType, const QString& strPeople*/)
{
	if (m_talkWindowshell == nullptr)
	{
		m_talkWindowshell = new TalkWindowShell;
		connect(m_talkWindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {
			m_talkWindowshell = nullptr;
		});
	}

	QWidget* widget = findWindowName(uid);
	if (!widget)
	{   
		m_strCreatingTalkId = uid;
		TalkWindow* talkwindow = new TalkWindow(m_talkWindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		// 判断是群聊还是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name, sign FROM tab_department WHERE departmentID = %1").arg(uid);
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();

		QString strWindowName, strMsgLabel;

		if (rows == 0) // 单聊
		{
			QString sql = QString("SELECT employee_name, employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			sqlDepModel.setQuery(sql);
		}
		
		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);         // 0行0列
		signIndex = sqlDepModel.index(0, 1);             // 0行1列
		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);        // 窗口名称
		talkwindowItem->setMsgLabelContent(strMsgLabel);  // 左侧联系人文本显示
		m_talkWindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);

		/*
		switch (groupType)
		{
		case COMPANY: {
			talkwindow->setWindowName(QStringLiteral("天天集团-越分享越拥有"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("公司群"));
			break;
		}
		case PERSONELGROUP: {
			talkwindow->setWindowName(QStringLiteral("公心、民兴、爱心，事事在心"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("人事部"));
			break;
		}
		case MARKETGROUP: {
			talkwindow->setWindowName(QStringLiteral("今天工作不努力，明天努力找工作"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("市场部"));
			break;
		}
		case DEVELOPMENTGROUP: {
			talkwindow->setWindowName(QStringLiteral("只有两种编程语言，一种是天天挨骂的，另一种是没人用的"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("研发部"));
			break;
		}
		case PTOP: {
			talkwindow->setWindowName(QStringLiteral(""));
			talkwindowItem->setMsgLabelContent(strPeople);
			break;
		}
		default:
			break;
		}
		
		m_talkWindowshell->addTalkWindow(talkwindow, talkwindowItem, groupType);
		*/
	}
	else
	{
		// 左侧聊天列表设为被选中
		QListWidgetItem* item = m_talkWindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		// 设置右侧当前聊天窗口
		m_talkWindowshell->setCurrentWidget(widget);
	}

	m_talkWindowshell->show();
	m_talkWindowshell->activateWindow();            
}
