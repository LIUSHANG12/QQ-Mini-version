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
	void onLoginBtnClicked();    // 登录按钮

private:
	void initControl();          // 初始化
	bool connectMySql();         // 连接数据库
	bool veryfyAccountCode(bool &isAccountLogin, QString &strAccount);    // 验证账号与密码

private:
	Ui::UserLoginClass ui;
};