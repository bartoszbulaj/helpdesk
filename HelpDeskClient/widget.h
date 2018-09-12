#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

#include <QtSql>
#include <QSqlDatabase>

#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>

#include <QSettings>
#include <QInputDialog>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QString sciezkaUstawien;

    QString defaultHostName;
    QString defaultDatabaseName;
    QString defaultUserName;
    QString defaultPassword;

    QString myHostName;
    QString myDatabaseName;
    QString myUserName;
    QString myPassword;

    QGridLayout *mylayout;

    QPushButton *okButton;
    QPushButton *resetButton;

    QLineEdit *imieLineEdit;
    QLineEdit *nazwiskoLineEdit;
    QLineEdit *opisLineEdit;
    QLineEdit *budynekLineEdit;
    QLineEdit *pokojLineEdit;

    QLabel *imieLabel;
    QLabel *nazwiskoLabel;
    QLabel *opisLabel;
    QLabel *budynekLabel;
    QLabel *pokojLabel;

public:
    QSqlDatabase mydb;
    QSqlQuery query;

private slots:
    void wyslij();
    void wyczyscPola();

    void saveSettings();
    void loadSettings();

public slots:
    void closeConnection();
    bool openConnection();    
    void resetSettings();

};

#endif // WIDGET_H
