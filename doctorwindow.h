#ifndef DCOTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class DoctorWindow; }
QT_END_NAMESPACE

class DoctorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DoctorWindow(QWidget *parent = nullptr);
    ~DoctorWindow();

private:
    Ui::DoctorWindow *ui;
};
#endif // DOCTORWINDOW_H
