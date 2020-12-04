#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QTimer>
#include <QMessageBox>
#include <thread>
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
 * Authors: Charles Ranson
 */
class DoctorWindow : public QMainWindow {
    Q_OBJECT

public:
    /*!
     * This is the default for the Doctor Window it is responsible for setting all added widgets
     * to their default values along with initializing all private variables.
     */
    explicit DoctorWindow(QWidget *parent = nullptr);
    /*!
     * Cleans up all allocated memory on the heap. Including the client pointer and hospital pointer.
     */
    ~DoctorWindow() override;

private slots:

    /*!
    *   Serves as the callback function for when the doctor requests a bed. It creates a new bed and
    *   then sees if the hospital it desires is registered or not. If the hospital is registered it assigns the bed
    *   to the hospital, if not it registers a new hospital within the database.
    */
    void on_requestBed_pressed();
    // Callback function to update ailment on change of the checkbox
    void on_injury_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_burn_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_virus_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_radiation_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_scan_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_respiratory_checkbox_stateChanged(int arg1);
    /// Callback function to update ailment on change of the checkbox
    void on_cardiac_checkbox_stateChanged(int arg1);
    /// This latitude will be the players latitude that is sent to the server
    void on_latitudeLineEdit_textChanged(const QString &arg1);
    /// This longitude will be the players longitude that is sent to the server
    void on_longitudeLineEdit_textChanged(const QString &arg1);
    /// This method will display in a QDialog Box the number of patients that have been treated
    void getStatus();

private:
    Ui::DoctorWindow *ui; //the pointer to main display
    std::string ailment; //string for the ailment
    Client * doctorClient; //used to send request to client
    QTimer * timer; // Used for doing regular updates
    double latitude; // Doctor position y
    double longitude; // Doctor position x
    std::vector<utility::string_t> ailmentVector; // Possible conditions
    std::vector<std::string> * patient_ids; // Ids of patients waiting on a hospital
    std::vector<Patient> * untreated_patients; // Patients waiting on a hospital
    /// This method will clear the checkboxes from the doctor ui.
    void clear_checkboxes(void);
    /// This values sends a request to the server to get patient Statuses
    void makeRequest(web::json::value &reply, web::json::value &status);
    
};
#endif // DOCTORWINDOW_H
