#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Calculator");
    this->setFixedSize(331, 365);

    ui->btn_multi->    setText(QChar(0x00D7));
    ui->btn_pow2->     setText('x' + QChar(0x00B2));
    ui->btn_sqrt->     setText(QChar(0x221A));
    ui->btn_backspace->setText(QChar(0x2190));

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_1, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_2, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_3, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_4, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_5, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_6, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_7, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_8, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->btn_9, SIGNAL(clicked()), this, SLOT(digits()));

    connect(ui->btn_sqrt,        SIGNAL(clicked()), this, SLOT(on_unary_operand_clicked()));
    connect(ui->btn_pow2,        SIGNAL(clicked()), this, SLOT(on_unary_operand_clicked()));
    connect(ui->btn_change_sign, SIGNAL(clicked()), this, SLOT(on_unary_operand_clicked()));

    connect(ui->btn_division,    SIGNAL(clicked()), this, SLOT(on_binary_operand_clicked()));
    connect(ui->btn_multi,       SIGNAL(clicked()), this, SLOT(on_binary_operand_clicked()));
    connect(ui->btn_plus,        SIGNAL(clicked()), this, SLOT(on_binary_operand_clicked()));
    connect(ui->btn_minus,       SIGNAL(clicked()), this, SLOT(on_binary_operand_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digits()
{
    QPushButton* btn = static_cast<QPushButton*>(sender());
    int digit = btn->text().toInt();

    if (ui->lbl_operand->text() == "0") {
        ui->lbl_operand->clear();
        ui->lbl_total->clear();
    }
    ui->lbl_operand->setText(ui->lbl_operand->text() + QString::number(digit));
}



void MainWindow::on_btn_dot_clicked()
{
    if (!ui->lbl_operand->text().contains('.')) {
        ui->lbl_operand->setText(ui->lbl_operand->text() + '.');
    }
}

void MainWindow::on_btn_clear_clicked()
{
    ui->lbl_operand->setText("0");
    ui->lbl_total->clear();
    ui->lbl_sign->clear();
}

void MainWindow::on_unary_operand_clicked()
{
    QPushButton* btn = static_cast<QPushButton*>(sender());
    QString operation = btn->text();

    QString txt = ui->lbl_operand->text();
    double val = txt.toDouble();

    if (operation == ui->btn_change_sign->text()) {
        val *= -1;
    } else if (operation == ui->btn_sqrt->text()) {
        if (val > 0.0) {
            val = std::sqrt(val);
        } else {
            abort_operation();
            return;
        }
    } else if (operation == ui->btn_pow2->text()) {
        val *= val;
    }

    ui->lbl_operand->setText(QString::number(val));
}

void MainWindow::on_btn_backspace_clicked()
{
    QString txt = ui->lbl_operand->text();
    txt.chop(1);

    if (txt.isEmpty()) {
        txt = "0";
    }

    ui->lbl_operand->setText(txt);
}

void MainWindow::abort_operation()
{
    ui->lbl_sign->setText("");
    ui->lbl_total->setText("Value error!");
    ui->lbl_operand->setText("0");
}

bool MainWindow::calculate(double operand, const QString &operation)
{
    double temp_total = ui->lbl_total->text().toDouble();

    if (operation == ui->btn_plus->text()) {
        temp_total += operand;
    } else if (operation == ui->btn_minus->text()) {
        temp_total -= operand;
    } else if (operation == ui->btn_multi->text()) {
        temp_total *= operand;
    } else if (operation == ui->btn_division->text()) {
        if (!operand) {
            return false;
        }
        temp_total /= operand;
    }

    ui->lbl_total->setText(QString::number(temp_total));
    return true;
}

void MainWindow::on_binary_operand_clicked()
{
    QPushButton* btn = static_cast<QPushButton*>(sender());
    QString operation = btn->text();

    double operand = ui->lbl_operand->text().toDouble();

    if (ui->lbl_operand->text() == "0")
        return;

    ui->lbl_sign->setText(operation);

    if (ui->lbl_operand->text() == "")
        return;

    ui->lbl_operand->clear();

    if (!pending_operation_.isEmpty()) {
        if (!calculate(operand, pending_operation_)) {
            abort_operation();
            return;
        }
        pending_operation_.clear();
    } else {
        ui->lbl_total->setText(QString::number(operand));
    }

    pending_operation_ = operation;

}

void MainWindow::on_btn_equal_clicked()
{
    double operand = ui->lbl_operand->text().toDouble();

    if (!pending_operation_.isEmpty()) {
        if (!calculate(operand, pending_operation_)) {
            abort_operation();
            return;
        }
        pending_operation_.clear();
    }

    ui->lbl_operand->setText(ui->lbl_total->text());
    ui->lbl_total->clear();
    ui->lbl_sign->clear();
}
