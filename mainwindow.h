#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network_controller.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void network_write(const QJsonObject &);

public slots:
    void network_connected();
    void network_disconnected();
    void network_callback(const QJsonObject & obj);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    void start_interface();

    network_controller *network_controller_;
    void start_socket();
    void imprime_boleto();
    void abre_boleto_pdf(const QString &nome);
};

#endif // MAINWINDOW_H
