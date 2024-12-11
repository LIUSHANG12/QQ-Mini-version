#pragma once

#include "basicwindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget* parent = nullptr);
	~UserLogin();

private slots:
	void onLoginBtnClicked();    // ��¼��ť

private:
	void initControl();          // ��ʼ��
	bool connectMySql();         // �������ݿ�
	bool veryfyAccountCode(bool &isAccountLogin, QString &strAccount);    // ��֤�˺�������

private:
	Ui::UserLoginClass ui;
};