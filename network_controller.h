#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QTimer>
#include <QBuffer>

class network_controller : public QObject
{
    Q_OBJECT

public:
    explicit network_controller();
    ~network_controller();


    void start_socket();

signals:
    void network_callback(const QJsonObject &);
    void network_write(const QJsonObject &);

    void network_connected();
    void network_disconnected();
    void network_error(const QJsonObject &);
    void network_bytes_written(const int &);

private slots:
    void configure_signal_socket();


public slots:
    void bytesWritten(qint64 bytes);
    void conecta_socket();
    void read_all_buffer();
    void read_all();
    void state_changed(QAbstractSocket::SocketState socketState);

    void ping();

private:
    void write_socket(QJsonObject obj);

    QJsonObject socket_ba_to_json(QByteArray &array);
    QByteArray socket_json_to_ba(QJsonObject &obj);

    QTcpSocket *s;    
    QTimer *timer_ping;

    QByteArray buffer;
    qint32 size;
    QByteArray IntToArray(qint32 source);
};
