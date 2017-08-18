#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    start_interface();
}

MainWindow::~MainWindow()
{
    if(network_controller_)
        network_controller_->deleteLater();

    delete ui;
}

void MainWindow::start_interface()
{
   start_socket();
   ui->vencimento->setDate(QDate::currentDate());
   ui->data_desconto->setDate(QDate::currentDate());
   ui->data_juros->setDate(QDate::currentDate());
   ui->data_abatimento->setDate(QDate::currentDate());
   ui->data_documento->setDate(QDate::currentDate());
   ui->data_processamento->setDate(QDate::currentDate());
   ui->data_protesto->setDate(QDate::currentDate());
}

void MainWindow::network_connected()
{
    qDebug() << "network_connected";
}

void MainWindow::network_disconnected()
{
    qDebug() << "network_disconnected";
}

void MainWindow::network_callback(const QJsonObject &obj)
{
    qDebug() << "network_callback" << obj;

    const QString & ERRO = obj.value("ERRO").toString();
    const QString & MSG = obj.value("MSG").toString();
    const QString & DATETIME = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");

    const QString & MSG1 = "[ " + DATETIME + " ] - " + MSG;

    ui->log->appendPlainText(MSG1);

    if(ERRO.isEmpty())
        abre_boleto_pdf(MSG);
}

void MainWindow::abre_boleto_pdf(const QString & nome)
{
    QUrl f(QString("file:///%1").arg(nome));
    QDesktopServices::openUrl(f);
}

void MainWindow::on_pushButton_clicked()
{
    imprime_boleto();
}

void MainWindow::start_socket()
{
    network_controller_ = new network_controller();

    connect(this, & MainWindow::network_write, network_controller_, & network_controller::network_write, Qt::UniqueConnection);
    connect(network_controller_, & network_controller::network_connected, this, & MainWindow::network_connected, Qt::UniqueConnection);
    connect(network_controller_, & network_controller::network_disconnected, this, &  MainWindow::network_connected, Qt::UniqueConnection);
    connect(network_controller_, & network_controller::network_callback, this, &  MainWindow::network_callback, Qt::UniqueConnection);

    network_controller_->start_socket();
}

void MainWindow::imprime_boleto()
{
    // RECEBEDOR
    const QString & ESTABELECIMENTO_NOME = ui->lineEdit_2->text();
    const QString & ESTABELECIMENTO_CPF_CNPJ = ui->lineEdit_5->text();
    const QString & ESTABELECIMENTO_LOGRADOURO = ui->lineEdit_11->text();
    const QString & ESTABELECIMENTO_NUMERO = ui->lineEdit_12->text();
    const QString & ESTABELECIMENTO_BAIRRO = ui->lineEdit_25->text();
    const QString & ESTABELECIMENTO_CIDADE = ui->lineEdit_26->text();
    const QString & ESTABELECIMENTO_CEP = ui->lineEdit_27->text();
    const QString & ESTABELECIMENTO_COMPLEMENTO = ui->lineEdit_29->text();
    const QString & SIGLA_ESTADO = ui->lineEdit_28->text();
    const int & TIPO_PESSOA = ui->comboBox_2->currentIndex();
    const QString & NUMERO_CONTA = ui->lineEdit_31->text();
    const QString & DIGITO_CONTA = ui->lineEdit_30->text();
    const QString & NUMERO_AGENCIA = ui->lineEdit_32->text();
    const QString & DIGITO_AGENCIA = ui->lineEdit_33->text();
    const QString & MODALIDADE = ui->lineEdit_34->text();
    const int & INDICE_ACBR_BANCO = ui->comboBox->currentIndex();
    const QString & BANCO_CONVENIO = ui->lineEdit_35->text();
    const int & LAYOUT_BOLETO = ui->comboBox_3->currentIndex();
    const QString & CODIGO_CEDENTE = ui->lineEdit_37->text();

    // SACADO
    const QString & RESPONSAVEL = ui->lineEdit->text();
    const QString & RESPONSAVEL_CPF_CNPJ = ui->lineEdit_3->text();
    const QString & RESPONSAVEL_LOGRADOURO = ui->lineEdit_4->text();
    const QString & RESPONSAVEL_NUMERO = ui->lineEdit_13->text();
    const QString & RESPONSAVEL_BAIRRO = ui->lineEdit_14->text();
    const QString & RESPONSAVEL_COMPLEMENTO = ui->lineEdit_15->text();
    const QString & RESPONSAVEL_CIDADE = ui->lineEdit_16->text();
    const QString & SIGLA_UF = ui->lineEdit_18->text();

    const QString & MENSALIDADE_NORMAL = ui->le_matricula_->text();

    const QString & instrucao1 = ui->le_instrucao1->text().simplified();
    const QString & instrucao2 = ui->le_instrucao2->text().simplified();

    const QString & data_vencimento = ui->vencimento->date().toString("dd/MM/yyyy");
    const QString & data_documento = ui->data_documento->date().toString("dd/MM/yyyy");
    const QString & data_processamento = ui->data_processamento->date().toString("dd/MM/yyyy");
    const QString & data_abatimento = ui->data_abatimento->date().toString("dd/MM/yyyy");
    const QString & data_desconto = ui->data_desconto->text();

    const QString & especie_documento = ui->especie_documento->currentText();
    const QString & local_pagamento = ui->le_local_pagamento->text().simplified();
    const QString & msg = ui->le_mensagem->text().simplified();
    const QString & data_juros = ui->data_juros->text().simplified();
    const QString & data_protesto = ui->data_protesto->text().simplified();

    const QString & mensalidade_normal = ui->le_matricula_->text();
    const QString & valor_desconto = ui->le_desconto->text();
    const QString & valor_juros = ui->le_juros->text();
    const QString & valor_abatimento = ui->le_abatimento->text();
    const QString & valor_outras_despesas = ui->le_outras_despesas->text();
    const QString & percentual_multa = ui->le_percentual_multa->text();

    const QString & VALOR_IOF = ui->le_valor_iof->text();

    QJsonObject boleto_info;

    boleto_info.insert("ESTABELECIMENTO_NOME", ESTABELECIMENTO_NOME);
    boleto_info.insert("ESTABELECIMENTO_CPF_CNPJ", ESTABELECIMENTO_CPF_CNPJ);
    boleto_info.insert("ESTABELECIMENTO_LOGRADOURO", ESTABELECIMENTO_LOGRADOURO);
    boleto_info.insert("ESTABELECIMENTO_NUMERO", ESTABELECIMENTO_NUMERO);
    boleto_info.insert("ESTABELECIMENTO_BAIRRO", ESTABELECIMENTO_BAIRRO);
    boleto_info.insert("ESTABELECIMENTO_CIDADE", ESTABELECIMENTO_CIDADE);
    boleto_info.insert("ESTABELECIMENTO_CEP", ESTABELECIMENTO_CEP);
    boleto_info.insert("ESTABELECIMENTO_COMPLEMENTO", ESTABELECIMENTO_COMPLEMENTO);
    boleto_info.insert("SIGLA_ESTADO", SIGLA_ESTADO);
    boleto_info.insert("TIPO_PESSOA", TIPO_PESSOA);
    boleto_info.insert("NUMERO_CONTA", NUMERO_CONTA);
    boleto_info.insert("DIGITO_CONTA", DIGITO_CONTA);
    boleto_info.insert("NUMERO_AGENCIA", NUMERO_AGENCIA);
    boleto_info.insert("DIGITO_AGENCIA", DIGITO_AGENCIA);
    boleto_info.insert("MODALIDADE", MODALIDADE);
    boleto_info.insert("INDICE_ACBR_BANCO", QString::number(INDICE_ACBR_BANCO));
    boleto_info.insert("BANCO_CONVENIO", BANCO_CONVENIO);
    boleto_info.insert("LAYOUT_BOLETO", LAYOUT_BOLETO);
    boleto_info.insert("CODIGO_CEDENTE", CODIGO_CEDENTE);

    boleto_info.insert("TIPO_IMPRESSAO", 1);
    boleto_info.insert("NOSSO_NUMERO", ui->lineEdit_36->text());

    boleto_info.insert("INSTRUCAO1", instrucao1);
    boleto_info.insert("INSTRUCAO2", instrucao2);

    boleto_info.insert("RESPONSAVEL", RESPONSAVEL);
    boleto_info.insert("RESPONSAVEL_CPF_CNPJ", RESPONSAVEL_CPF_CNPJ);
    boleto_info.insert("RESPONSAVEL_LOGRADOURO", RESPONSAVEL_LOGRADOURO);
    boleto_info.insert("RESPONSAVEL_NUMERO", RESPONSAVEL_NUMERO);
    boleto_info.insert("RESPONSAVEL_BAIRRO", RESPONSAVEL_BAIRRO);
    boleto_info.insert("RESPONSAVEL_COMPLEMENTO", RESPONSAVEL_COMPLEMENTO);
    boleto_info.insert("RESPONSAVEL_CIDADE", RESPONSAVEL_CIDADE);
    boleto_info.insert("SIGLA_UF", SIGLA_UF);
    boleto_info.insert("MENSALIDADE_NORMAL", MENSALIDADE_NORMAL);

    boleto_info.insert("MENSALIDADE_NORMAL", mensalidade_normal);
    boleto_info.insert("VALOR_ABATIMENTO", valor_abatimento);
    boleto_info.insert("PERCENTUAL_MULTA", percentual_multa);
    boleto_info.insert("VALOR_OUTRAS_DESPESAS", valor_outras_despesas);
    boleto_info.insert("VALOR_DESCONTO", valor_desconto);
    boleto_info.insert("VALOR_JUROS", valor_juros);

    boleto_info.insert("CONTA_CARTEIRA_NUMERO", "102");

    boleto_info.insert("ESPECIE_DOCUMENTO", especie_documento);
    boleto_info.insert("DATA_VENCIMENTO", data_vencimento);

    boleto_info.insert("VALOR_IOF", VALOR_IOF);
    boleto_info.insert("PARCELA", "10");
    boleto_info.insert("TOTAL_PARCELA", "11");

    if(ui->cb_data_documento->isChecked())
        boleto_info.insert("DATA_DOCUMENTO", data_documento);

    if(ui->cb_data_processamento->isChecked())
        boleto_info.insert("DATA_PROCESSAMENTO", data_processamento);

    if(ui->cb_data_protesto->isChecked())
        boleto_info.insert("DATA_PROTESTO", data_protesto);

    boleto_info.insert("DATA_DESCONTO", data_desconto);
    boleto_info.insert("DATA_JUROS", data_juros);
    boleto_info.insert("DATA_ABATIMENTO", data_abatimento);

    boleto_info.insert("LOCAL_PAGAMENTO", local_pagamento);
    boleto_info.insert("MSG", msg);
    boleto_info.insert("ACEITE", "1");
    boleto_info.insert("TIPO_DIA_PROTESTO", "");

    emit network_write(boleto_info);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setText("Fulano da Silva Santos");
    ui->lineEdit_3->setText("37769137000115");
    ui->lineEdit_4->setText("Logradouro Sacado teste");
    ui->lineEdit_13->setText("100");
    ui->lineEdit_14->setText("Bairro Teste");
    ui->lineEdit_15->setText("Complemento Teste");
    ui->lineEdit_17->setText("Cidade Teste");
    ui->lineEdit_16->setText("DF");
    ui->lineEdit_18->setText("70660073");
    ui->le_matricula_->setText("1900,20");
    ui->le_matricula_->setText("900,00");
    ui->le_juros->setText("1");
    ui->le_abatimento->setText("3");
    ui->le_percentual_multa->setText("2");
    ui->lineEdit_2->setText("Estabelecimento Teste");
    ui->lineEdit_5->setText("37769137000115");
    ui->lineEdit_11->setText("Logradouro Estabelecimento teste");
    ui->lineEdit_12->setText("200");
    ui->lineEdit_25->setText("Bairro Teste");
    ui->lineEdit_26->setText("Complemento Teste");
    ui->lineEdit_27->setText("Cidade Teste");
    ui->lineEdit_29->setText("DF");
    ui->lineEdit_28->setText("70660073");
    ui->lineEdit_31->setText("12345");
    ui->lineEdit_30->setText("0");
    ui->lineEdit_32->setText("5555");
    ui->lineEdit_33->setText("1");
    ui->lineEdit_34->setText("1");
    ui->lineEdit_37->setText("12345");
    ui->lineEdit_36->setText("00001");
}
