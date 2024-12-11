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
	// ������ʽ��
	void loadStyleSheet(const QString &sheetName);

	// ��ȡԲͷ��
	QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize = QSize(0, 0));

private:
	void initBackGroundColor();                   // ��ʼ������

protected:
	void paintEvent(QPaintEvent*);                // �����¼�
	void mousePressEvent(QMouseEvent* event);     // ����¼�
	void mouseMoveEvent(QMouseEvent* event);      // ����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent*);         // ����ɿ��¼�

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);  // ��ʼ��������
	void setTitleBarTitle(const QString& title, const QString& icon = ""); // ���ñ���������

public slots:
	void onShowClose(bool);                       // �رմ���
	void onShowMin(bool);                         // ������С��
	void onShowHide(bool);                        // ��������
	void onShowNormal(bool);                      // ���ڳ�̬
	void onShowQuit(bool);                        // �˳�
	void onSignalSkinChanged(const QColor& color); // �ı䴰����ɫ

	void onButtonMinClicked();                    // ʵ�ִ�����С����ť�ĺ���
	void onButtonRestoreClicked();                // ʵ�ִ�����󻯻�ԭ��ť�ĺ���
	void onButtonMaxClicked();                    // ʵ�ִ�����󻯰�ť�ĺ���
	void onButtonCloseClicked();                  // ʵ�ֹرմ��ڰ�ť�ĺ���

protected:
	QPoint m_mousePoint;                          // ���λ��
	bool m_mousePressed;                          // ����Ƿ���
	QColor m_colorBackGround;                     // ����ɫ
	QString m_styleName;                          // �����ļ�����
	TitleBar* _titleBar;                          // ������
};
