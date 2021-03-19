#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QTextEdit>
#include <QtDebug>
#include <QtMath>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QString pending_operation_;
private slots:
    void digits();
    void on_btn_dot_clicked();
    void on_btn_clear_clicked();
    void on_unary_operand_clicked();
    void on_binary_operand_clicked();
    void on_btn_backspace_clicked();

    void abort_operation();

    bool calculate(double operand, const QString &operation);
    void on_btn_equal_clicked();
};


#endif // MAINWINDOW_H
