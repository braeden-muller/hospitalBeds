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
    Ui::DoctorWindow *ui; //the pointer to main display
    std::string ailment; //string for the ailment
    Client * doctorClient; //used to send request to client
    std::vector<std::tuple<std::size_t, Hospital,bool>>* hospitals; //vector of tuples for <bedID, hospital, full>
    Location location; //enum for the location chosen from drop down menu
    std::chrono::time_point<std::chrono::steady_clock> start; //time that all time stamps are based off of

    void addHospital(Hospital & h, web::json::value & bedSpec); //helper function to add hospital
};
#endif // DOCTORWINDOW_H
