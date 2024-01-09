#include "QtPasswordBook.h"

QtPasswordBook::QtPasswordBook(QWidget *parent)
    : QWidget(parent)
{
	resize(700, 500);

    passwordData = new PasswordData();

    passwordTable = new QTableWidget(0, 5);
    passwordTable->horizontalHeader()->setStretchLastSection(true);
	QStringList horHeaders;
	horHeaders << tr("Name") << tr("Email") << tr("Number") << tr("Login") << tr("Password");
	passwordTable->setHorizontalHeaderLabels(horHeaders);
	passwordTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	passwordTable->verticalHeader()->hide();
	passwordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	passwordTable->setContextMenuPolicy(Qt::CustomContextMenu);

	searchEdit = new QLineEdit();
	searchEdit->setPlaceholderText(tr("search"));
	QLabel* searchLabel = new QLabel(tr("Search") + ": ");
	searchLabel->setBuddy(searchEdit);

	errorMsg = new QLabel(tr("Some fields have invalid values"));
	errorMsg->setStyleSheet("QLabel {color: red;}");
	errorMsg->setAlignment(Qt::AlignCenter);

	saveBtn = new QPushButton(tr("save"));
	QHBoxLayout* saveBtnLayout = new QHBoxLayout();
	saveBtnLayout->addWidget(saveBtn);
	saveBtnLayout->insertSpacing(0, 100);
	saveBtnLayout->insertSpacing(2, 100);

	QHBoxLayout* rowLayout = new QHBoxLayout();
	rowLayout->addWidget(searchLabel, 1);
	rowLayout->addWidget(searchEdit, 4);

	mainLayout = new QVBoxLayout();
	mainLayout->addWidget(passwordData);
	mainLayout->addWidget(errorMsg);
	mainLayout->addLayout(rowLayout);
	mainLayout->addWidget(passwordTable);
	mainLayout->addLayout(saveBtnLayout);

	errorMsg->hide();

	setLayout(mainLayout);

	contextMenu = new QMenu(this);
	QAction* deleteItemAction = contextMenu->addAction(tr("Remove"));
	connect(deleteItemAction, &QAction::triggered, [this] {
		removeItem();
		});

	connect(passwordData, &PasswordData::needAdd, this, &QtPasswordBook::processNewItem);
	connect(searchEdit, &QLineEdit::textChanged, [this] {updateTable(); });
	connect(passwordTable, &QTableWidget::cellClicked, this, &QtPasswordBook::cellClicked);
	connect(passwordTable, &QTableWidget::customContextMenuRequested, this, &QtPasswordBook::handleContextMenu);
	connect(saveBtn, &QPushButton::clicked, this, &QtPasswordBook::saveCreds);

	loadCreds();
}

QtPasswordBook::~QtPasswordBook()
{

}

void QtPasswordBook::updateTable()
{
	passwordTable->clearContents();
	passwordTable->setRowCount(0);

	auto iter = creds.begin();
	int row = 0;

	QString searchString = searchEdit->text();
	QTableWidgetItem* item = nullptr;

	while (iter != creds.end())
	{
		if (searchString.isEmpty() || iter->name.contains(searchString, Qt::CaseInsensitive))
		{
			passwordTable->insertRow(passwordTable->rowCount());

			item = new QTableWidgetItem;
			item->setText(iter->name);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			passwordTable->setItem(row, 0, item);

			item = new QTableWidgetItem;
			item->setText(iter->email);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			passwordTable->setItem(row, 1, item);

			item = new QTableWidgetItem;
			item->setText(iter->number);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			passwordTable->setItem(row, 2, item);

			item = new QTableWidgetItem;
			item->setText(iter->login);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			passwordTable->setItem(row, 3, item);

			item = new QTableWidgetItem;
			item->setText(iter->password);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			passwordTable->setItem(row, 4, item);

			row++;
		}

		iter++;
	}
}

bool QtPasswordBook::validateFields(const Credentials& newItemCreds)
{
	if (newItemCreds.name.isEmpty() || 
		newItemCreds.name.startsWith(" ") || 
		newItemCreds.name.endsWith(" ") ||
		!QRegExp("[^ @]+@[^ @]+").exactMatch(newItemCreds.email) ||
		!QRegExp("\\+\\d\\(\\d\\d\\d\\)\\d\\d\\d-\\d\\d-\\d\\d").exactMatch(newItemCreds.number) ||
		newItemCreds.login.isEmpty() || 
		newItemCreds.login.startsWith(" ") || 
		newItemCreds.login.endsWith(" ") ||
		newItemCreds.password.isEmpty() || 
		newItemCreds.password.startsWith(" ") || 
		newItemCreds.password.endsWith(" "))
		return false;

	return true;
}

void QtPasswordBook::processNewItem()
{
	const Credentials newItemCreds = {
		passwordData->getValue(Credentials::CredType::NAME),
		passwordData->getValue(Credentials::CredType::EMAIL),
		passwordData->getValue(Credentials::CredType::NUMBER),
		passwordData->getValue(Credentials::CredType::LOGIN),
		passwordData->getValue(Credentials::CredType::PASSWORD)
	};

	if (!validateFields(newItemCreds))
	{
		errorMsg->show();
		return;
	}
	else
	{
		errorMsg->hide();
	}

	creds[passwordData->getValue(Credentials::CredType::NAME)] = newItemCreds;
	updateTable();
}

void QtPasswordBook::cellClicked(int row, int col)
{
	passwordData->setValue(Credentials::CredType::NAME, passwordTable->item(row, 0)->text());
	passwordData->setValue(Credentials::CredType::EMAIL, passwordTable->item(row, 1)->text());
	passwordData->setValue(Credentials::CredType::NUMBER, passwordTable->item(row, 2)->text());
	passwordData->setValue(Credentials::CredType::LOGIN, passwordTable->item(row, 3)->text());
	passwordData->setValue(Credentials::CredType::PASSWORD, passwordTable->item(row, 4)->text());
}

void QtPasswordBook::handleContextMenu(const QPoint& point)
{
	QTableWidgetItem* item = passwordTable->itemAt(point);
	if (item) {
		selectedRow = item->row();
		contextMenu->exec(QCursor::pos());
	}
}

void QtPasswordBook::removeItem()
{
	QString name = passwordTable->item(selectedRow, 0)->text();
	creds.remove(name);
	updateTable();
}

void QtPasswordBook::saveCreds()
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream stream(&file);
		auto iter = creds.begin();

		while (iter != creds.end())
		{
			stream << iter->name << iter->email << iter->number << iter->login << iter->password;
			iter++;
		}
	}
}

void QtPasswordBook::loadCreds()
{
	QFile file(fileName);
	if (file.exists() && file.open(QIODevice::ReadOnly))
	{
		QDataStream stream(&file);

		while (!stream.atEnd())
		{
			Credentials userCreds;
			stream >> userCreds.name;
			stream >> userCreds.email;
			stream >> userCreds.number;
			stream >> userCreds.login;
			stream >> userCreds.password;

			creds[userCreds.name] = userCreds;
		}
	}

	updateTable();
}