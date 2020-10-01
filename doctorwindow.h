#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DoctorWindow; }
QT_END_NAMESPACE

enum Ailment { COVID, ER, INJURY, ILLNESS };

/*!
 * \brief Displays data relevant to doctors
 * The doctor window is where a doctor may enter patient relevant data and receive information about patient placement
 */
class DoctorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DoctorWindow(QWidget *parent = nullptr);
    ~DoctorWindow() override;

private slots:

    void on_requestBed_pressed();

    void on_covidButton_pressed();

    void on_erButton_pressed();

    void on_injuryButton_pressed();

    void on_illnessButton_pressed();

private:
    Ui::DoctorWindow *ui;
    Ailment ailment;
    Client * doctorClient;
};
#endif // DOCTORWINDOW_H
