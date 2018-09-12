#ifndef HELPDESKADMIN_H
#define HELPDESKADMIN_H

#include <QWidget>
#include <QDebug>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

#include <QtSql>
#include <QSqlDatabase>

#include <QSqlQueryModel>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>

#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>

#include "widget.h"

class HelpDeskAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit HelpDeskAdmin(QWidget *parent = 0);
    ~HelpDeskAdmin();

private:


    Widget *userWindow;

    QSqlQueryModel *myQueryModel;
    QTableView *myTableView;

    QSqlDatabase myDataTable;

    QComboBox *myTablesCombo;
    QComboBox *adminCombo;

    int *ticketId;

    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QLabel *adminLabel;
    QLineEdit *adminLineEdit;
    QPushButton *pobierzButton;
    QPushButton *zamknijZlecenieButton;

    QGridLayout *myGridLayout;
    QVBoxLayout *myVerticalLayout;
    QHBoxLayout *myHorizontalLayout;

private slots:
    void takeTicket();
public slots:
    void comboChanged(int);
private slots:
    void clear_Id_Admin_LineEdit();

    void getTicketId();
    void pobierzClicked();
    void zamknijZlecenieClicked();

    void testCombo();

    void writeSettings();
    void readSettings();


};

#endif // HELPDESKADMIN_H
