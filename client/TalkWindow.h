#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include "TalkWindowShell.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString& uid/*, GroupType groupType*/);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);               // ��ӱ���ͼƬ
	void setWindowName(const QString& name);
	QString getTalkId();

private slots:
	void onFileOpenBtnClicked(bool);

	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();

	//void initCompanyTalk();                             // ��ʼ����˾Ⱥ����
	//void initPersonelTalk();                            // ��ʼ�����²�����
	//void initMarketTalk();                              // ��ʼ���г�������
	//void initDevelopmentTalk();                         // ��ʼ���з�������

	void initTalkWindow();                                // ��ʼ��Ⱥ��
	void initPtoPTalk();                                  // ��ʼ������
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID);    // ���Ա����Ϣ

private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;                                   // �Ƿ�ΪȺ��
	//GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;     // ���з�����ϵ������    
	friend class TalkWindowShell;
};
