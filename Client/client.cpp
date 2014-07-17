#include "client.h"
#include "ui_client.h"
#include <QtCore/QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>


QT_USE_NAMESPACE

Client::Client(const QUrl &_url, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client),
    url(_url)
{
     ui->setupUi(this);
     ui->mainForm->setVisible(false);
     ui->fileName->setText("");
     connect(&webSocket, &QWebSocket::connected, this, &Client::onConnected);
     webSocket.open(QUrl(_url));
}

Client::~Client()
{
    webSocket.close();
    delete ui;
}

void Client::onConnected()
{
    ui->resultTextBox->setText("WebSocket connected");
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
}

void Client::onConectError(QAbstractSocket::SocketError errorCode)
{
 QMessageBox::information(this, tr("Warning"), tr("Cann't connect to server. Error" + errorCode));
}


void Client::sendTextMessage(QString _message)
{
    webSocket.sendTextMessage(_message);
    ui->resultTextBox->setText("Message send: " + _message);
}


void Client::onTextMessageReceived(QString message)
{
    ui->resultTextBox->setText(message);

    /*Process server's response
     *  first character represent the user's action
     *  second server's result of action */
    QStringList substrings = message.split("|") ;
    if(substrings.length() == 1)
    {
     QMessageBox::information(this, tr("Error"), tr("Error occurs while trying to proccess request"));
     return;
    }
    QString result = substrings.value(substrings.length()- 2 );

    switch(message[0].digitValue())
    {
    case authorization:
        if(result == SUCCESS)
        {
            ui->resultTextBox->setText("Authorization succeed");
            //check user's type
            ui->authorization->setVisible(false);
            ui->mainForm->setVisible(true);
        }
        else
        {
            QMessageBox::information(this, tr("Error"), tr("Authorization failed. Please check login and password"));
        }
        break;

    case sendVideo:
        ui->resultTextBox->setText(message);
        break;

    case seeHistory:
        break;

    case addUser:
        break;

     /*if server doesn't support user's request*/
    default:
        QMessageBox::information(this, tr("Error"), tr("Error occurs while trying to proccess request"));
        return;
    }
 }



void Client::on_SIgnIn_clicked()
{
    if(ui->login->text() != "" && ui->password->text()!= "")
    {
        message = QString::number(authorization);
        message +=  "|"+ ui->login->text() + "|" + ui->password->text();
        sendTextMessage(message);
    }
    else
    {
        QMessageBox::information(this, tr("Warning"), tr("Please enter login and password"));
        ui->resultTextBox->setText("Input login and password");
    }

}


void Client::on_LoadVideo_clicked()
{
    path = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "D://",
                "Video Files (*.avi);; Image Files (*.jpg)"
                );
    ui->fileName->setText(path);
}



void Client::on_SendVideo_clicked()
{
    if(path == NULL)
    {
        QMessageBox::information(this, tr("Error"), tr("File wasn't selected"));
        return;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Warning"), tr("File not open. Try again"));
        return;
    }
    QStringList substrings = path.split("/");
    QString filename = substrings.value(substrings.length() - 1);
    quint64 filesize = file.size();

    data += QString::number(sendVideo) + "|";
    data += filename + "|";
    data += QString::number(filesize);

    //sendBinaryMessage(data);
    data.clear();

    quint64 sendedData = 0;
    while(sendedData < filesize)
    {
        data = file.read(1024);
        webSocket.sendBinaryMessage(data);
        //sendBinaryMessage(data);
        sendedData += data.size();
        data.clear();

   }

        ui->resultTextBox->setText(QString::number(file.pos()));

    ui->resultTextBox->setText(QString::number(sendedData));

    ui->fileName->setText("");
    data.clear();
    path = "";
    file.close();
}

