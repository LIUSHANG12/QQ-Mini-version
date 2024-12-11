#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qdebug.h>

QString gLoginEmployeeID;               // ��¼�ߵ�QQ�ţ�Ա���ţ�

UserLogin::UserLogin(QWidget* parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);

	// �������ݿ�
	if (!connectMySql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ��!"));
		close();
	}
}

bool UserLogin::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("myqtqq");                // ���ݿ�����
	db.setHostName("127.0.0.1");				 // ������
	db.setUserName("root");						 // �û���
	db.setPassword("root");						 // ����
	db.setPort(3306);							 // �˿�

	if (db.open())
	{
		qDebug() << "mysql connect ok";
		return true;
	}
	else
	{
		return false;
	}
}

bool UserLogin::veryfyAccountCode(bool &isAccountLogin, QString &strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();

	// ����Ա����(QQ�ŵ�¼)
	QString  strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);           // ��ѯ
	queryEmployeeID.exec();                          // ִ�н��

	if (queryEmployeeID.first())                     // ָ��������һ��
	{
		// ���ݿ���qq�Ŷ�Ӧ������
		QString strCode = queryEmployeeID.value(0).toString();
		
		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strAccountInput;
			isAccountLogin = false;

			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	// �˺ŵ�¼
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account = '%1'")
						.arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = queryAccount.value(1).toString();

			strAccount = strAccountInput;
			isAccountLogin = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount;                            // �˺Ż�QQ��
	if (!veryfyAccountCode(isAccountLogin, strAccount))
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��������˺Ż���������,����������!"));
		//ui.editUserAccount->setText("");         // �����һ�ε��˺�����
		//ui.editPassword->setText("");
		return;
	}

	// ���µ�½״̬Ϊ��¼
	QString strSqlStatus = QString("UPDATE tab_employees SET online_status = 2 WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	close();
	CCMainWindow* mainwindow = new CCMainWindow(strAccount, isAccountLogin);
	mainwindow->show();
}