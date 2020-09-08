#ifndef HOSPITALWINDOW_H
#define HOSPITALWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HospitalWindow; }
QT_END_NAMESPACE

class HospitalWindow : public QMainWindow
{
    Q_OBJECT

public:
    HospitalWindow(QWidget *parent = nullptr);
    ~HospitalWindow();

private:
    Ui::HospitalWindow *ui;
};
#endif // HOSPITALWINDOW_H
