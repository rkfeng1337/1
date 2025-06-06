#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include<QTcpSocket>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);

signals:
    void connected();
    void messageReceived(const QString &text);
    void jsonReceived(const QJsonObject &docObj);
private:
    QTcpSocket *m_clientSocket;

public slots:
    void sendMessage(const QString &text,const QString &type = "message", const QString &userid = "id" , const QString &selfid = "id");
    void onReadyRead()  ;
    void disconnectFromHost();
    void connectToserver(const QHostAddress &address, quint16 port);
};

#endif // CHATCLIENT_H
