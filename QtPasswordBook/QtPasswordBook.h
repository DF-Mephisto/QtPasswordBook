#pragma once

#include <QtWidgets>

#include "PasswordData.h"

class QtPasswordBook : public QWidget
{
    Q_OBJECT

public:
    QtPasswordBook(QWidget *parent = Q_NULLPTR);
    ~QtPasswordBook();

private:
    PasswordData* passwordData;
    QTableWidget* passwordTable;
    QLineEdit* searchEdit;
    QVBoxLayout* mainLayout;
    QLabel* errorMsg;
    QMenu* contextMenu;
    QPushButton* saveBtn;
    int selectedRow;

    QHash<QString, Credentials> creds;
    const QString fileName = "creds.pwd";

    void updateTable();
    void removeItem();
    bool validateFields(const Credentials& newItemCreds);

private slots:
    void processNewItem();
    void cellClicked(int row, int col);
    void handleContextMenu(const QPoint& point);
    void saveCreds();
    void loadCreds();
};
