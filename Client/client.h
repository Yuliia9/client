#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

const QString SUCCESS = "0";
const QString FAIL = "1";


namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(const QUrl &_url, QWidget *parent = 0);
    ~Client();

Q_SIGNALS:
    void closed();

private slots:
    void onConectError(QAbstractSocket::SocketError errorCode);

private Q_SLOTS:
    void onConnected();
    void sendTextMessage(QString _message);
    void onTextMessageReceived(QString message);
    void on_SIgnIn_clicked();
    void on_LoadVideo_clicked();
    void on_SendVideo_clicked();


private:
    /*fields*/
    Ui::Client *ui;
    QWebSocket webSocket;
    QUrl url;
    QString message;
    QByteArray data;
    QString path;

    enum Action{
        authorization,
        sendVideo,
        seeResults,
        seeHistory,
        addPicture,
        addUser
    };

    enum Users{
        Admin,
        SimpleUser,
        AdvancedUser
    };
};

#endif // CLIENT_H
