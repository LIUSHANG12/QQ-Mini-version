#pragma once

#include <QTcpSocket>

class TcpSocket  : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();
	void run();

signals:
	void signalGetDataFromClient(QByteArray&, int);  // 从客户端收到数据后就发射信号
	void signalClientDisconnect(int);                // 告诉server有客户端断开连接

private slots:
	void onReceiveData();                            // 处理readyRead信号读取的数据
	void onClientDisconnect();                       // 处理客户端断开连接

private:
	int m_descriptor;                                // 描述符，用于唯一标识

};
