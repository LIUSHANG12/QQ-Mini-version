#pragma once

#include "basicwindow.h"
#include "ui_TalkWindowShell.h"
#include <qmap.h>
#include <QTcpSocket>
#include <QUdpSocket>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

const int gtcpPort = 8888;

enum GroupType
{
	COMPANY = 0,                               // ��˾Ⱥ
	PERSONELGROUP,                             // ���²�
	MARKETGROUP,                               // �г���
	DEVELOPMENTGROUP,                          // �з���
	PTOP                                       // ͬ��֮�䵥������
};

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

public:
	// ����µ����촰��
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString uid/*, GroupType groupType*/);
	// ���õ�ǰ���촰��
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

private:
	void initControl();                                // ��ʼ���ؼ�
	void initTcpSocket();                              // ��ʼ��TCP�׽���
	void initUdpSocket();                              // ��ʼ��UDP

	void getEmployeesID(QStringList& employeesList);   // ��ȡ����Ա����QQ��
	bool createJSFile(QStringList& employeesList);
	
	// ������յ���Ϣ
	void handleReceiveMsg(int senderEmployeeId, int msgType, QString strMsg);

public slots:
	void onEmotionBtnClicked(bool);                    // ���鰴ť�����ִ�еĲۺ���
	
	// �ͻ��˷���Tcp����(����, ��������, �ļ�)
	void updateSendTcpMsg(QString& strData, int& msgType, QString fileName = "");

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);     // ����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);               // ���鱻ѡ��
	void processPendingData();                               // ����㲥�յ�������

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;    // �򿪵����촰��
	EmotionWindow* m_emotionWindow;                          // ���鴰��

private:
	QTcpSocket* m_tcpClientSocket;

	QUdpSocket* m_udpReceiver;                               // udp���ն�
};
