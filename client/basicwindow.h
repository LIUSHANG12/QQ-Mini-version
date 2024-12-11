#pragma once

#include <QDialog>
#include "titlebar.h"

class BasicWindow  : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	// 加载样式表
	void loadStyleSheet(const QString &sheetName);

	// 获取圆头像
	QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize = QSize(0, 0));

private:
	void initBackGroundColor();                   // 初始化背景

protected:
	void paintEvent(QPaintEvent*);                // 绘制事件
	void mousePressEvent(QMouseEvent* event);     // 鼠标事件
	void mouseMoveEvent(QMouseEvent* event);      // 鼠标移动事件
	void mouseReleaseEvent(QMouseEvent*);         // 鼠标松开事件

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);  // 初始化标题栏
	void setTitleBarTitle(const QString& title, const QString& icon = ""); // 设置标题栏标题

public slots:
	void onShowClose(bool);                       // 关闭窗口
	void onShowMin(bool);                         // 窗口最小化
	void onShowHide(bool);                        // 窗口隐藏
	void onShowNormal(bool);                      // 窗口常态
	void onShowQuit(bool);                        // 退出
	void onSignalSkinChanged(const QColor& color); // 改变窗口颜色

	void onButtonMinClicked();                    // 实现窗口最小化按钮的函数
	void onButtonRestoreClicked();                // 实现窗口最大化还原按钮的函数
	void onButtonMaxClicked();                    // 实现窗口最大化按钮的函数
	void onButtonCloseClicked();                  // 实现关闭窗口按钮的函数

protected:
	QPoint m_mousePoint;                          // 鼠标位置
	bool m_mousePressed;                          // 鼠标是否按下
	QColor m_colorBackGround;                     // 背景色
	QString m_styleName;                          // 标题文件名称
	TitleBar* _titleBar;                          // 标题栏
};
