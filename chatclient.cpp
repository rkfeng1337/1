#include "chatclient.h"
#include <QDatastream>
#include <QJsonObject>
#include <QJsonDocument>

ChatClient::ChatClient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket =new QTcpSocket(this);
    connect(m_clientSocket,&QTcpSocket::connected,this,&ChatClient::connected);
    connect(m_clientSocket,&QTcpSocket::readyRead,this,&ChatClient::onReadyRead);
}

void ChatClient::sendMessage(const QString &text, const QString &type, const QString &destid, const QString &sourceid)
{    if(m_clientSocket->state()!= QAbstractSocket::ConnectedState)
        return;
    if(!text.isEmpty()){
        //create a Qatastream operating on the socket
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_9);
        //create the JsoN we want to send
        QJsonObject message;
        message["type"]=type;
        message["text"]= text;
        message["destid"]= destid;
        message["sourceid"]= sourceid;
        //send the JsoN using QDatastream
        serverStream<<QJsonDocument(message).toJson();
    }
}


void ChatClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    //start an infinite loop
    for(;;){
        socketStream.startTransaction();
        socketStream >>jsonData;
        if(socketStream.commitTransaction()){
            //emit messageReceived(QString::fromUtf8(jsonData));

            QJsonParseError parseError;
            const QJsonDocument jsonDoc =QJsonDocument::fromJson(jsonData, &parseError);
            if(parseError.error ==QJsonParseError::NoError){
                if(jsonDoc.isObject()){
                    // and is a JsoN object
                   // emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());// parse the JsoN
                }}
        } else {
            break ;
        }}}

void ChatClient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}

void ChatClient::connectToserver(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address,port);
}
