#pragma once
#include <QtWidgets>

#include "Credentials.h"

class PasswordData : public QWidget
{
	Q_OBJECT

private:
	QLineEdit* nameEdit;
	QLineEdit* emailEdit;
	QLineEdit* numberEdit;
	QLineEdit* loginEdit;
	QLineEdit* passwordEdit;

	QPushButton* addBtn;

public:
	PasswordData(QWidget* pWgt = Q_NULLPTR);
	QString getValue(const Credentials::CredType type);
	void setValue(const Credentials::CredType type, const QString& val);

signals:
	void needAdd();
};

