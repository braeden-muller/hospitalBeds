#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QMessageBox>
#include <cpprest/http_listener.h>
#include "client.h"
#include "hospital.h"
#include "patient.h"

#define NUM_HOSPITALS 5
#define HANDLE_LENGTH 7

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

    void on_injury_checkbox_stateChanged(int arg1);

    void on_burn_checkbox_stateChanged(int arg1);

    void on_virus_checkbox_stateChanged(int arg1);

    void on_radiation_checkbox_stateChanged(int arg1);

    void on_scan_checkbox_stateChanged(int arg1);

    void on_respiratory_checkbox_stateChanged(int arg1);

    void on_cardiac_checkbox_stateChanged(int arg1);

    void on_latitudeLineEdit_textChanged(const QString &arg1);

    void on_longitudeLineEdit_textChanged(const QString &arg1);

    void on_getPatients_pressed();

private:
    Ui::DoctorWindow *ui; //the pointer to main display
    std::string ailment; //string for the ailment
    Client * doctorClient; //used to send request to client
    double latitude;
    double longitude;
    std::size_t patientId;
    std::vector<utility::string_t> ailmentVector;
    std::vector<std::string> * patient_ids;
    std::vector<Patient> * untreated_patients;

    void clear_checkboxes(void);
};
#endif // DOCTORWINDOW_H
