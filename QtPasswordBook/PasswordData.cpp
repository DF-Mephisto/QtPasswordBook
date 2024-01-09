#include "PasswordData.h"

PasswordData::PasswordData(QWidget* pWgt)
	: QWidget(pWgt)
{
	//CREATING LABEL AND EDITS
	nameEdit = new QLineEdit();
	nameEdit->setPlaceholderText(tr("name"));
	QLabel* nameLabel = new QLabel(tr("Name") + ": ");
	nameLabel->setBuddy(nameEdit);
	nameLabel->setFixedWidth(100);

	emailEdit = new QLineEdit();
	emailEdit->setPlaceholderText(tr("email"));
	QRegExp emailPattern("[^@ ]+@[^@ ]+");
	emailEdit->setValidator(new QRegExpValidator(emailPattern, this));
	QLabel* emailLabel = new QLabel(tr("Email") + ": ");
	emailLabel->setBuddy(emailEdit);
	emailLabel->setFixedWidth(100);

	numberEdit = new QLineEdit();
	numberEdit->setPlaceholderText(tr("number"));
	numberEdit->setInputMask("+9(999)999-99-99");
	QLabel* numberLabel = new QLabel(tr("Number") + ": ");
	numberLabel->setBuddy(numberEdit);
	numberLabel->setFixedWidth(100);

	QRegExp loginPasswordPattern("[^ ]+");
	loginEdit = new QLineEdit();
	loginEdit->setPlaceholderText(tr("login"));
	loginEdit->setValidator(new QRegExpValidator(loginPasswordPattern, this));
	QLabel* loginLabel = new QLabel(tr("Login") + ": ");
	loginLabel->setBuddy(loginEdit);
	loginLabel->setFixedWidth(100);

	passwordEdit = new QLineEdit();
	passwordEdit->setPlaceholderText(tr("password"));
	passwordEdit->setValidator(new QRegExpValidator(loginPasswordPattern, this));
	QLabel* passwordLabel = new QLabel(tr("Password") + ": ");
	passwordLabel->setBuddy(passwordEdit);
	passwordLabel->setFixedWidth(100);

	//CREATIN LAYOUTS
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(5);

	QHBoxLayout* rowLayout = new QHBoxLayout();
	rowLayout->addWidget(nameLabel);
	rowLayout->addWidget(nameEdit);
	mainLayout->addLayout(rowLayout);

	rowLayout = new QHBoxLayout();
	rowLayout->addWidget(emailLabel);
	rowLayout->addWidget(emailEdit);
	mainLayout->addLayout(rowLayout);

	rowLayout = new QHBoxLayout();
	rowLayout->addWidget(numberLabel);
	rowLayout->addWidget(numberEdit);
	mainLayout->addLayout(rowLayout);

	rowLayout = new QHBoxLayout();
	rowLayout->addWidget(loginLabel);
	rowLayout->addWidget(loginEdit);
	mainLayout->addLayout(rowLayout);

	rowLayout = new QHBoxLayout();
	rowLayout->addWidget(passwordLabel);
	rowLayout->addWidget(passwordEdit);
	mainLayout->addLayout(rowLayout);

	//CREATING BUTTON
	rowLayout = new QHBoxLayout();
	addBtn = new QPushButton(tr("Add"));
	rowLayout->addWidget(addBtn);
	rowLayout->insertSpacing(0, 100);
	rowLayout->insertSpacing(2, 100);
	mainLayout->addLayout(rowLayout);

	setLayout(mainLayout);

	connect(addBtn, &QPushButton::clicked, [this] {emit needAdd(); });
}

QString PasswordData::getValue(const Credentials::CredType type)
{
	QString result;

	switch (type)
	{
	case Credentials::CredType::NAME:
		result = nameEdit->text();
		break;

	case Credentials::CredType::EMAIL:
		result = emailEdit->text();
		break;

	case Credentials::CredType::NUMBER:
		result = numberEdit->text();
		break;

	case Credentials::CredType::LOGIN:
		result = loginEdit->text();
		break;

	case Credentials::CredType::PASSWORD:
		result = passwordEdit->text();
		break;
	}

	return result;
}

void PasswordData::setValue(const Credentials::CredType type, const QString& val)
{
	switch (type)
	{
	case Credentials::CredType::NAME:
		nameEdit->setText(val);
		break;

	case Credentials::CredType::EMAIL:
		emailEdit->setText(val);
		break;

	case Credentials::CredType::NUMBER:
		numberEdit->setText(val);
		break;

	case Credentials::CredType::LOGIN:
		loginEdit->setText(val);
		break;

	case Credentials::CredType::PASSWORD:
		passwordEdit->setText(val);
		break;
	}
}