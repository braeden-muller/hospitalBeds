#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "client.h"
#include "hospital.h"

#define JSTR json::value::string

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

    void on_ailmentSelector_currentIndexChanged(int index);

private:
    Ui::DoctorWindow *ui;
    Ailment ailment;
    Client * doctorClient;
};
#endif // DOCTORWINDOW_H
