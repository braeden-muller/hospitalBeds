#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class DoctorWindow; }
QT_END_NAMESPACE

/*!
 * \brief Displays data relevant to doctors
 * The doctor window is where a doctor may enter patient relevant data and receive information about patient placement
 */
class DoctorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DoctorWindow(QWidget *parent = nullptr);
    ~DoctorWindow() override;

private:
    Ui::DoctorWindow *ui;
};
#endif // DOCTORWINDOW_H
