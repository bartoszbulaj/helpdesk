#include "widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    qDebug() <<endl<< "- startuje konstruktor userWindow";
    sciezkaUstawien = "/HelpDeskAppSettings.ini";

    loadSettings();

    qDebug() << myHostName << endl << myDefaultPort << endl << myDatabaseName << endl << myUserName << endl << myPassword ;

    this->setMinimumSize(300,250);
    this->setMaximumSize(350,300);
    this->setWindowTitle("HelpDesk - CSWIiCh");

    defaultHostName = "192.168.3.5";
    defaultDatabaseName = "helpdesk";
    defaultUserName = "root";
    defaultPassword = "";

    okButton = new QPushButton("Wyślij zgłoszenie");

    resetButton = new QPushButton("Reset\nustawień\nserwera"); //reset button

    imieLabel = new QLabel("Imię:");
    nazwiskoLabel = new QLabel("Nazwisko:");
    opisLabel = new QLabel("Opis usterki:");
    budynekLabel = new QLabel("Budynek:");
    pokojLabel = new QLabel("Pokój:");

    imieLineEdit = new QLineEdit();
    nazwiskoLineEdit = new QLineEdit();
    opisLineEdit = new QLineEdit();
    budynekLineEdit = new QLineEdit();
    budynekLineEdit->setMaxLength(15);
    pokojLineEdit = new QLineEdit();
    pokojLineEdit->setMaxLength(10);

    mylayout = new QGridLayout;

    mylayout->addWidget(imieLabel,0,0);
    mylayout->addWidget(imieLineEdit,0,1);
    mylayout->addWidget(nazwiskoLabel,1,0);
    mylayout->addWidget(nazwiskoLineEdit,1,1);
    mylayout->addWidget(budynekLabel,2,0);
    mylayout->addWidget(budynekLineEdit,2,1);
    mylayout->addWidget(pokojLabel,3,0);
    mylayout->addWidget(pokojLineEdit,3,1);
    mylayout->addWidget(opisLabel,4,0);
    mylayout->addWidget(opisLineEdit,4,1);
    mylayout->addWidget(okButton,5,0,1,2);
    mylayout->addWidget(resetButton,6,0);

//    resetButton->hide();

    this->setLayout(mylayout);

    connect(okButton,SIGNAL(clicked()),this,SLOT(wyslij()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(resetSettings()));

    qDebug() << "... zakonczenie pracy konstruktora";
}

Widget::~Widget()
{
    this->saveSettings();
    qDebug() << "ustawienia serwera zostaly zapisane...";
}

void Widget::wyslij() //zapisz w bazie
{

    if(imieLineEdit->text()=="" || nazwiskoLineEdit->text()=="" || opisLineEdit->text()=="" || budynekLineEdit->text()=="" || pokojLineEdit->text()=="")
    {
        QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
    }
    else
    {
        if(openConnection())
        {
            qDebug() << "- - - polaczenie z baza nawiazane" << endl;
            QSqlQuery query;
            query.prepare(QString("INSERT INTO oczekujace(imie,nazwisko,budynek,pokoj,opis,data)"
                                  "VALUES('%1', '%2', '%3', '%4', '%5',NOW());").arg(imieLineEdit->text()).arg(nazwiskoLineEdit->text()).arg(budynekLineEdit->text()).arg(pokojLineEdit->text()).arg(opisLineEdit->text()));
            if(!query.exec())
            {
                qDebug() << "Last error:" << query.lastError();
                QMessageBox::information(this,"HelDesk","Zgłoszenie nie zostało wysłane.");
                closeConnection();
            }
            else
            {
                QMessageBox::information(this,"HelpDesk","Zgłoszenie zostało wysłane.");
                wyczyscPola();
                closeConnection();
            }
        }
        else
        {
            QMessageBox::information(this,"HelDesk","Zgłoszenie nie zostało wysłane.");
        }
    }

}

void Widget::wyczyscPola()
{
    imieLineEdit->setText("");
    nazwiskoLineEdit->setText("");
    budynekLineEdit->setText("");
    pokojLineEdit->setText("");
    opisLineEdit->setText("");
}

bool Widget::openConnection()
{
    mydb = QSqlDatabase::addDatabase("QMYSQL");
    mydb.setHostName(myHostName);
    mydb.setPort(myDefaultPort);
    mydb.setDatabaseName(myDatabaseName);
    mydb.setUserName(myUserName);
    mydb.setPassword(myPassword);

    if(!mydb.open())
    {
        qDebug()<<"baza nie zostala otwarta...";
        QMessageBox::warning(this,"Błąd","Nie można otworzyć bazy danych.");
        return false;
    }
    else
    {
        qDebug()<<"baza otwarta...";
        return true;
    }
}

void Widget::closeConnection()
{
    mydb.close();
//    mydb.removeDatabase(QSqlDatabase::defaultConnection);
    qDebug()<< "polaczenie z baza zostalo zakonczone...";
}

void Widget::resetSettings()
{
    QSettings serverSettings(sciezkaUstawien,QSettings::IniFormat);
    serverSettings.clear();

    QSettings firstRun(sciezkaUstawien,QSettings::IniFormat);
    firstRun.beginGroup("firstRun");
    firstRun.setValue("first",true);
    firstRun.endGroup();
//    this->close();
    qDebug() << "Ustawienia serwera zostaly zresetowane...";

}

void Widget::saveSettings()
{
    QSettings serverSettings(sciezkaUstawien,QSettings::IniFormat);
    serverSettings.beginGroup("serverSettings");

    serverSettings.setValue("hostName",this->myHostName);
    serverSettings.setValue("port",this->myDefaultPort);
    serverSettings.setValue("databaseName",this->myDatabaseName);
    serverSettings.setValue("userName",this->myUserName);
    serverSettings.setValue("password",this->myPassword);

    serverSettings.endGroup();
}

void Widget::loadSettings()
{
    QSettings firstRun(sciezkaUstawien,QSettings::IniFormat);
    firstRun.beginGroup("firstRun");
    bool pierwszeUruchomienie = firstRun.value("first",true).toBool();


    if(pierwszeUruchomienie)
    {
        bool okHost = false;
        bool okPort = false;
        bool okDatabaseName = false;
        bool okUserName = false;
        bool okPassword1 = false;
        bool okPassword2 = false;
        bool samePassword = false;
        QString pusty = "";

        while(!okHost)
        {
        myHostName = QInputDialog::getText(this,"Konfiguracja","Wpisz adres serwera:",QLineEdit::Normal,"",&okHost);
        if(myHostName == pusty) okHost= false;
        }

        while(!okPort)
        {

        myDefaultPort = (QInputDialog::getText(this,"Konfiguracja","Wpisz port serwera:",QLineEdit::Normal,"",&okPort)).toInt();
        if(myDefaultPort == 0)
        {
            //okPort = false; ustawienie domyslnego portu
            myDefaultPort = 3306;
        }
        }

        while(!okDatabaseName)
        {
        myDatabaseName = QInputDialog::getText(this,"Konfiguracja","Wpisz nazwe bazy danych:",QLineEdit::Normal,"",&okDatabaseName);
        if(myDatabaseName == pusty) okDatabaseName = false;
        }


        while(!okUserName)
        {
        myUserName = QInputDialog::getText(this,"Konfiguracja","Wpisz nazwe użytkownika:",QLineEdit::Normal,"",&okUserName);
        if(myUserName == pusty) okUserName = false;
        }


        while(!samePassword)
        {
            while(!okPassword1)
            {
                myPassword = QInputDialog::getText(this,"Konfiguracja","Wpisz hasło:",QLineEdit::Password,"",&okPassword1);
                okPassword2 = false;
                while(!okPassword2)
                {
                    QString myPassword2 = QInputDialog::getText(this,"Konfiguracja","Potwierdź hasło:",QLineEdit::Password,"",&okPassword2);
                    if(myPassword == myPassword2)
                    {
                        samePassword=true;
                    }
                    else
                    {
                        QMessageBox hasloNiezgodne("Błąd","Błędnie wprowadzono hasło",QMessageBox::Warning,QMessageBox::Ok | QMessageBox::Default,0,0);
                        int y = hasloNiezgodne.exec();
                        if(y == QMessageBox::Ok) hasloNiezgodne.close();
                        okPassword1 = false;
                    }
                }
            }
        }

    }
    else
    {
        QSettings serverSettings(sciezkaUstawien,QSettings::IniFormat);
        serverSettings.beginGroup("serverSettings");

        myHostName = serverSettings.value("hostName").toString();
        myDefaultPort = serverSettings.value("port").toInt();
        myDatabaseName = serverSettings.value("databaseName").toString();
        myUserName = serverSettings.value("userName").toString();
        myPassword = serverSettings.value("password").toString();

        serverSettings.endGroup();
    }


    firstRun.setValue("first",false);
    firstRun.endGroup();
}
