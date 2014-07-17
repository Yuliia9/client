#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client(QUrl(QStringLiteral("ws://localhost:80")));
    QObject::connect(&client, &Client::closed, &a, &QCoreApplication::quit);
    //QObject::connect(&client, SIGNAL(error(QAbstractSocket::SocketError)), &client, SLOT(Client::onConectError(QAbstractSocket::SocketError)));
    client.show();

    return a.exec();
}
