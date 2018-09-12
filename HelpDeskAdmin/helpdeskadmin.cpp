#include "helpdeskadmin.h"


HelpDeskAdmin::HelpDeskAdmin(QWidget *parent) :
    QWidget(parent)
{

    qDebug() << QSqlDatabase::drivers();
    this->setWindowTitle("HelpDeskAdminPanel");

    this->setWindowIcon(QIcon(":/MyFiles/custom_icon.ico"));

    qDebug() << "- ladowanie drivera bazy danych QMYSQL";
    myDataTable = QSqlDatabase::addDatabase("QMYSQL");
    myDataTable.setHostName("");
//    myDataTable.setPort();
    myDataTable.setDatabaseName("");
    myDataTable.setUserName("");
    myDataTable.setPassword("");

    myQueryModel = new QSqlQueryModel();
    myTableView = new QTableView();
    myTableView->resizeColumnsToContents(); // Adjust the column width.
    myTableView->setAlternatingRowColors(true);
    myTableView->setCornerButtonEnabled(false);

    myTablesCombo = new QComboBox();
    myTablesCombo->addItem("Zlecenia oczekujące");
    myTablesCombo->addItem("W trakcie realizacji");
    myTablesCombo->addItem("Zrealizowane");

    adminCombo = new QComboBox();
    adminCombo->addItem("Zbigniew");
    adminCombo->addItem("Karol");
    adminCombo->addItem("Lukasz");
    adminCombo->addItem("Bartek");

    idLabel = new QLabel("ID zlecenia:");
    idLineEdit = new QLineEdit();
    idLineEdit->setValidator( new QIntValidator(0,999,this));

    adminLabel = new QLabel("Admin:");
    adminLineEdit = new QLineEdit(); /// zmienic na combo box

    pobierzButton = new QPushButton("Pobierz zlecenie");
    zamknijZlecenieButton = new QPushButton("Zamknij zlecenie");

    userWindow = new Widget();

// show table in view
    this->comboChanged(0);

    myHorizontalLayout = new QHBoxLayout();
    myVerticalLayout = new QVBoxLayout();
    myGridLayout = new QGridLayout();

    myGridLayout->addWidget(idLabel,0,0);
    myGridLayout->addWidget(idLineEdit,0,1);
    myGridLayout->addWidget(adminLabel,1,0);
//    myGridLayout->addWidget(adminLineEdit,1,1);
    myGridLayout->addWidget(adminCombo,1,1);
    myGridLayout->addWidget(pobierzButton,0,2);
    myGridLayout->addWidget(zamknijZlecenieButton,1,2);

    myVerticalLayout->addWidget(myTablesCombo);
    myVerticalLayout->addWidget(myTableView);
    myVerticalLayout->addLayout(myGridLayout);

    myHorizontalLayout->addLayout(myVerticalLayout);
    myHorizontalLayout->addWidget(userWindow);

    connect(myTablesCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboChanged(int)));
    connect(pobierzButton,SIGNAL(clicked()),this,SLOT(pobierzClicked()));
    connect(zamknijZlecenieButton,SIGNAL(clicked()),this,SLOT(zamknijZlecenieClicked()));
    connect(userWindow->okButton,SIGNAL(clicked()),this,SLOT(testCombo()));

    this->setLayout(myHorizontalLayout);
readSettings();
}

HelpDeskAdmin::~HelpDeskAdmin()
{
writeSettings();
}

void HelpDeskAdmin::testCombo()
{
    this->comboChanged(0);
}

void HelpDeskAdmin::takeTicket()
{
    userWindow->openConnection();

    QString idNumber = idLineEdit->text();
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM usterki WHERE id = %1;").arg(idNumber));
    // if (sprawdzenie poprawnosci ID)

    userWindow->closeConnection();
}

void HelpDeskAdmin::comboChanged(int)
{
    userWindow->openConnection();

    QString nazwaTabeli = myTablesCombo->currentText();
    if(nazwaTabeli == "W trakcie realizacji")
    {
        nazwaTabeli="wtrakcie";
        pobierzButton->setDisabled(1);
        zamknijZlecenieButton->setEnabled(1);
        myQueryModel->setQuery(QString("SELECT id AS ID,data_zgloszenia AS 'Data zgłoszenia',data_pobrania AS 'Data pobrania',admin AS Admin,zglaszajacy AS Zgłaszający,budynek AS Budynek,pokoj AS Pokój,opis AS Opis FROM %1;").arg(nazwaTabeli));
        myTableView->setModel(myQueryModel);
        adminLineEdit->setDisabled(1);
        adminCombo->setDisabled(1);
        idLineEdit->setEnabled(1);
    }
    if(nazwaTabeli == "Zlecenia oczekujące")
    {
        nazwaTabeli="oczekujace";
        pobierzButton->setEnabled(1);
        zamknijZlecenieButton->setDisabled(1);
        myQueryModel->setQuery(QString("SELECT id AS ID, imie AS Imię, nazwisko AS Nazwisko, budynek AS Budynek, pokoj AS Pokój, opis AS Opis, data AS 'Data Zgłoszenia'  FROM %1;").arg(nazwaTabeli));
        myTableView->setModel(myQueryModel);
        adminLineEdit->setEnabled(1);
        adminCombo->setEnabled(1);
        idLineEdit->setEnabled(1);
    }
    if(nazwaTabeli == "Zrealizowane")
    {
        nazwaTabeli="zrealizowane";
        pobierzButton->setDisabled(1);
        zamknijZlecenieButton->setDisabled(1);
        myQueryModel->setQuery(QString("SELECT id AS ID, data_realizacji AS 'Data realizacji', admin AS Admin, opis AS Opis, zglaszajacy AS Zgłaszający FROM %1;").arg(nazwaTabeli));
        myTableView->setModel(myQueryModel);
        adminLineEdit->setDisabled(1);
        adminCombo->setDisabled(1);
        idLineEdit->setDisabled(1);
    }



    userWindow->closeConnection();
}

void HelpDeskAdmin::getTicketId()
{
    ticketId = new int(idLineEdit->text().toInt());
}

void HelpDeskAdmin::pobierzClicked()
{
    userWindow->openConnection();

    if(myTablesCombo->currentText()== "Zlecenia oczekujące")
    {
        if(adminCombo->currentText() != "" && idLineEdit->text() != "")
       // if(adminLineEdit->text() != "" && idLineEdit->text() != "")
        {
            getTicketId();
            QString idNumber = QString::number(*ticketId);


            QSqlQuery query1; // SELECT * FROM oczekujace
            QSqlQuery query2; // INSERT INTO wtrakcie
            QSqlQuery query3; // DELETE FROM oczekujace WHERE id = *
            query1.prepare(QString("SELECT * FROM oczekujace;").arg(idNumber));
            query1.exec();
            bool istnieje = 0;
            while(query1.next())
            {
                // IF ID == poprawne ID
                if(idNumber == query1.value(0).toString())
                {
                    istnieje = 1;
                    // INSERT
                    query2.prepare(QString("INSERT INTO wtrakcie(id,data_zgloszenia,data_pobrania,admin,zglaszajacy,budynek,pokoj,opis)"
                                           "VALUES ('"+ query1.value(0).toString() +"','"+ query1.value(6).toString() +"', NOW() ,'" + adminCombo->currentText() +"','"+query1.value(1).toString() +" "+ query1.value(2).toString() +"','"+query1.value(3).toString()+"','"+query1.value(4).toString() +"','"+query1.value(5).toString()+"');"));
                    query2.exec();

                    // DROP POBRANE PYTANIE
                    query3.prepare(QString("DELETE FROM oczekujace WHERE id= %1").arg(query1.value(0).toString()));
                    query3.exec();
                }

            }
            if(istnieje == 0) QMessageBox::warning(this,"Błąd","Nie ma zlecenia o podanym numerze ID.");
        }
        else
        {
            QMessageBox::warning(this,"Błąd","Pola \"Admin\" oraz \"ID zlecenia\" nie mogą być puste.");
        }
    }
    else
    {
        QMessageBox::warning(this,"Błąd","Pobranie zlecenia niemożliwe.\n\nWybierz tabelę ze zleceniami oczekującymi.");
    }

    this->comboChanged(0);
    this->clear_Id_Admin_LineEdit();
    userWindow->closeConnection();
}

void HelpDeskAdmin::zamknijZlecenieClicked()
{
    userWindow->openConnection();

    getTicketId();
    QString idNumber = QString::number(*ticketId);

    QSqlQuery query1; // SELECT * FROM wtrakcie WHERE id = *
    QSqlQuery query2; // INSERT INTO zrealizowane
    QSqlQuery query3; // DELETE FROM wtrakcie

    query1.prepare(QString("SELECT * FROM wtrakcie").arg(idNumber));
    query1.exec();
    bool istnieje = 0;
    while(query1.next())
    {
        if(idNumber == query1.value(0).toString())
        {
            istnieje = 1;
            // INSERT
            query2.prepare(QString("INSERT INTO zrealizowane(id,data_realizacji,admin,opis,zglaszajacy)"
                                   "VALUES ('"+query1.value(0).toString()+"',NOW(),'"+query1.value(3).toString()+"','"+query1.value(7).toString()+"','"+query1.value(4).toString()+"');"));
            query2.exec();
            // DELETE
            query3.prepare(QString("DELETE FROM wtrakcie WHERE id = %1").arg(query1.value(0).toString()));
            query3.exec();
        }
    }
    if(istnieje == 0) QMessageBox::warning(this,"Błąd","Nie ma zlecenia o podanym numerze ID.");

    this->comboChanged(0);
    this->clear_Id_Admin_LineEdit();

    userWindow->closeConnection();
}

void HelpDeskAdmin::clear_Id_Admin_LineEdit()
{
    adminLineEdit->clear();
    idLineEdit->clear();
}

void HelpDeskAdmin::writeSettings()
{
    QSettings settings("Bartosz Joker Inc.","HelpDeskAdmin");

    settings.beginGroup("mainWindow");
    settings.setValue("size",size());
    settings.endGroup();
}

void HelpDeskAdmin::readSettings()
{
    QSettings settings("Bartosz Joker Inc.","HelpDeskAdmin");

    settings.beginGroup("mainWindow");
    resize(settings.value("size",QSize(600,400)).toSize());
    settings.endGroup();
}
