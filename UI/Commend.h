#pragma once

#include<QObject>
#include<QString>
class fCommend :public QObject
{
	Q_OBJECT //±ØÐë°üº¬µÄºê

public:
	fCommend() { m_value = "zero"; }

	QString value() const { return  m_value; }

public slots:
	void CommendReture();
private:
	QString m_value;
};

