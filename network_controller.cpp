#include "network_controller.h"
#include "network_utils.h"
#include <QDataStream>

static inline qint32 ArrayToInt(QByteArray source);

network_controller::network_controller() : size(0)
{
}

network_controller::~network_controller()
{    
}

void network_controller::start_socket()
{
    timer_ping = new QTimer(this);
    timer_ping->setInterval(10000);

    configure_signal_socket();

    conecta_socket();
}

void network_controller::conecta_socket()
{
    s->abort();
    s->connectToHost("localhost", 3536);
}

void network_controller::configure_signal_socket()
{
    s = new QTcpSocket(this);
    connect(s, & QTcpSocket::disconnected, this, & network_controller::network_disconnected);
    connect(s, & QTcpSocket::connected, this, & network_controller::network_connected);
    connect(s, & QTcpSocket::readyRead, this, & network_controller::read_all_buffer);
    //connect(s, & QTcpSocket::bytesWritten, this, & network_controller::bytesWritten);
    connect(s, & QTcpSocket::stateChanged, this, & network_controller::state_changed);
    connect(this, & network_controller::network_write, this, & network_controller::write_socket);
}

void network_controller::bytesWritten(qint64 bytes)
{
    _D << "bytes " << bytes;
    emit network_bytes_written(bytes);
}

void network_controller::read_all()
{
    QByteArray data = s->readAll();
    QJsonObject obj = socket_ba_to_json(data);
    emit network_callback(obj);
}

void network_controller::state_changed(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::UnconnectedState:
        timer_ping->stop();
        break;
    case QAbstractSocket::ConnectedState:
        timer_ping->start(10000);
        break;
    default:
        _D << socketState;
        break;
    }
}

void network_controller::ping()
{
    QJsonObject o;
    o.insert("KEEPALIVE", 1);
    write_socket(o);
}

void network_controller::read_all_buffer()
{    
    while (s->bytesAvailable() > 0)
    {
        buffer.append(s->readAll());

        while ((size == 0 && buffer.size() >= 4) ||
               (size > 0 && buffer.size() >= size))
        {

            if (size == 0 && buffer.size() >= 4)
            {
                size = ArrayToInt(buffer.mid(0, 4));
                buffer.remove(0, 4);
                //_D << "Tamanho recebido: " << size <<  buffer.size();
            }

            if (size > 0 && buffer.size() >= size)
            {
               QByteArray data = buffer.mid(0, size);

               buffer = QByteArray();
               size = 0;

               QJsonObject obj = socket_ba_to_json(data);
               emit network_callback(obj);
            }
        }
    }
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QJsonObject network_controller::socket_ba_to_json(QByteArray &array)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array, & err);
    QJsonObject object = doc.object();

    if(err.error != QJsonParseError::NoError)
    {
        _D << err.errorString() << array;
        object.insert("ERRO", 1);
    }

    return object;
}

QByteArray network_controller::socket_json_to_ba(QJsonObject &obj)
{
    QJsonDocument doc(obj);
    QByteArray array(doc.toJson());
    return array;
}

void network_controller::write_socket(QJsonObject obj)
{
    QByteArray array = socket_json_to_ba(obj);
    s->write(array);
    s->waitForBytesWritten();
}

QByteArray network_controller::IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

