#pragma once

#include <QProxyStyle>
#include <QPixmap>
#include <QSize>

// �ı�Ĭ�ϵĲ������
class CustomProxyStyle :public QProxyStyle
{
public:
	CustomProxyStyle(QObject* parent)
	{
		setParent(parent);
	}
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = 0)const
	{
		if (PE_FrameFocusRect == element)
		{
			// ȥ��windows�в���Ĭ�ϵı߿�����߿򣬲�����ȡ����ʱֱ�ӷ��أ������л���
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

class CommonUtils
{
public:
	CommonUtils();
public:
	// ͷ��תԲͷ��
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
	// ������ʽ��
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);
	// ����Ĭ��Ƥ����ɫ
	static void setDefaultSkinColor(const QColor& color);
	// ��ȡĬ��Ƥ����ɫ
	static QColor getDefaultSkinColor();
};

