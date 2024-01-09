#pragma once
#include <QtWidgets>

struct Credentials
{
	QString name;
	QString email;
	QString number;
	QString login;
	QString password;

	enum class CredType
	{
		NAME, EMAIL, NUMBER, LOGIN, PASSWORD
	};
};