#include "SysTray.h"
#include "CustomMenu.h"

SysTray::SysTray(QWidget *parent)
	: m_parent(parent)
	,QSystemTrayIcon(parent)
{
	initSystemTray();
	show();
}

SysTray::~SysTray()
{}

void SysTray::initSystemTray()
{
	setToolTip(QStringLiteral("QQ-天天集团"));               // 设置系统托盘提示
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

void SysTray::addSystrayMenu()
{
	CustomMenu* customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", 
								":/Resources/MainWindow/app/logo.ico", 
								QStringLiteral("显示"));
	customMenu->addCustomMenu("onQuit", 
								":/Resources/MainWindow/app/page_close_btn_hover.png",
								QStringLiteral("关闭"));

	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)),
		m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)),
		m_parent, SLOT(onShowQuit(bool)));

	customMenu->exec(QCursor::pos());               
	delete customMenu;                              
	customMenu = nullptr;                           
}

void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	// 如果是触发，则将系统托盘改为显示
	if (reason == QSystemTrayIcon::Trigger)
	{
		m_parent->show();
		m_parent->activateWindow();
	}
	// 如果是文本，则添加菜单
	else if (reason == QSystemTrayIcon::Context)
	{
		addSystrayMenu();
	}
}