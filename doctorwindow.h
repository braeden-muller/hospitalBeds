#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "client.h"
#include "hospital.h"

#define NUM_HOSPITALS 5

QT_BEGIN_NAMESPACE
namespace Ui { class DoctorWindow; }
QT_END_NAMESPACE

//enum Ailment { INJURY, BURN, VIRUS, RADIATION, PSYCH, RESP, CARDIAC};

enum Location { CHRISTIANSBURG, ROANOKE, PRINCETON, LYNCHBURG, BRISTOL };

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

    void on_locationSelector_currentIndexChanged(int index);

private:
    Ui::DoctorWindow *ui;
    std::string ailment;
    Client * doctorClient;
    std::vector<Hospital> * hospitals;
    web::json::value hospitalAsJSON;
    Location location;
    int bedCall;
};
#endif // DOCTORWINDOW_H

