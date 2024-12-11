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
	_titleBar = new TitleBar(this);              // ����������
	_titleBar->setButtonType(buttontype);        // ���ð�ť����
	_titleBar->move(0, 0);                       // �����ƶ������Ͻ�λ��

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
		QString qsstyleSheet = QLatin1String(file.readAll());    // ��ȡ�ļ���ʽ�����ַ�������

		// ��ȡ�û���ǰ��Ƥ��RGBֵ
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

// ���໯����ʱ����Ҫ��д��ͼ�¼����ñ���ͼ
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

// ͷ��תԲͷ��
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

	// ����ת�����ͷ��
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
{   // �ر�
	close();
}

void BasicWindow::onShowMin(bool)
{   // ��С��
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{   // ���ش���
	hide();
}

void BasicWindow::onShowNormal(bool)
{   // ������ʾ����
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool)
{   // �˳�
	QString strSqlStatus = QString("UPDATE tab_employees SET online_status = 1 WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	QApplication::quit();
}

// ����ƶ�
void BasicWindow::mouseMoveEvent(QMouseEvent* e)
{   
	// ��갴�£��Ұ��µ������
	if (m_mousePressed && (e->buttons() && Qt::LeftButton))
	{
		// e->globalPos()�¼�����ʱȫ�����꣬�������Ļ���Ͻ�(0,0)
		move(e->globalPos() - m_mousePoint); // �¼�����ʱȫ�ֵ�����ֵ��ȥ֮ǰ������ֵ
		e->accept();
	}
}

// ��갴���¼�
void BasicWindow::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		// pos()�¼�����ʱ����ڴ������Ͻ�(0,0)��ƫ��
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - pos();
		e->accept();
	}
}

// ����ͷ��¼�
void BasicWindow::mouseReleaseEvent(QMouseEvent* e)
{
	m_mousePressed = false;
}

void BasicWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide(); // ���أ�Ҫȥ��ʾ���ص�ͼ������ҵ�
	}
	else
	{
		showMinimized(); // ��С��
	}
}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize; // ������ر���������ȡλ�ü���С
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize)); // ���ƾ���
}

void BasicWindow::onButtonMaxClicked()
{
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height())); // �������֮ǰ����Ϣ
	QRect desktopRect = QApplication::desktop()->availableGeometry(); // ��ȡ�������
	// �Ի�ȡ��������ν����޸ģ���Ƴ��Լ���ʵ�ʾ���
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect); // ��Ƽ���ͼ��Ϊ�Լ����õ�ʵ�ʾ���
}

void BasicWindow::onButtonCloseClicked()
{
	close();
}