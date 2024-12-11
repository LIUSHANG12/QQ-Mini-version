#include "basicwindow.h"
#include "CommonUtils.h"
#include "NotifyManager.h"

#include <QDesktopWidget>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QSqlQuery>
//#include <QScreen>

extern QString gLoginEmployeeID;

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
	
}

void BasicWindow::onSignalSkinChanged(const QColor& color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::setTitleBarTitle(const QString &title, const QString &icon)
{
	_titleBar->setTitleIcon(icon);               
	_titleBar->setTitleContent(title);           
}

void BasicWindow::initTitleBar(ButtonType buttontype)
{
	_titleBar = new TitleBar(this);              // 创建标题栏
	_titleBar->setButtonType(buttontype);        // 设置按钮类型
	_titleBar->move(0, 0);                       // 将其移动到左上角位置

	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::loadStyleSheet(const QString &sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen())
	{
		setStyleSheet("");                     
		QString qsstyleSheet = QLatin1String(file.readAll());    // 读取文件样式并用字符串保存

		// 获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
								.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);    
	}
	file.close();
}

void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// 子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

// 头像转圆头像
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0))
	{
		maskSize = mask.size();
	}
	else
	{
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	// 保存转换后的头像
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source); 
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool)
{   // 关闭
	close();
}

void BasicWindow::onShowMin(bool)
{   // 最小化
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{   // 隐藏窗口
	hide();
}

void BasicWindow::onShowNormal(bool)
{   // 正常显示窗口
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool)
{   // 退出
	QString strSqlStatus = QString("UPDATE tab_employees SET online_status = 1 WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	QApplication::quit();
}

// 鼠标移动
void BasicWindow::mouseMoveEvent(QMouseEvent* e)
{   
	// 鼠标按下，且按下的是左键
	if (m_mousePressed && (e->buttons() && Qt::LeftButton))
	{
		// e->globalPos()事件发生时全局坐标，相对于屏幕左上角(0,0)
		move(e->globalPos() - m_mousePoint); // 事件发生时全局的坐标值减去之前的坐标值
		e->accept();
	}
}

// 鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		// pos()事件发生时相对于窗口左上角(0,0)的偏移
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - pos();
		e->accept();
	}
}

// 鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent* e)
{
	m_mousePressed = false;
}

void BasicWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide(); // 隐藏，要去显示隐藏的图标才能找到
	}
	else
	{
		showMinimized(); // 最小化
	}
}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize; // 定义相关变量用来获取位置及大小
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize)); // 绘制矩形
}

void BasicWindow::onButtonMaxClicked()
{
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height())); // 保存最大化之前的信息
	QRect desktopRect = QApplication::desktop()->availableGeometry(); // 获取桌面矩形
	// 对获取的桌面矩形进行修改，设计出自己的实际矩形
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect); // 设计几何图形为自己设置的实际矩形
}

void BasicWindow::onButtonCloseClicked()
{
	close();
}