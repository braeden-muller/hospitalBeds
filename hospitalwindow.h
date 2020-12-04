#ifndef HOSPITALWINDOW_H
#define HOSPITALWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "client.h"
#include "hospital.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HospitalWindow; }
QT_END_NAMESPACE

const int handleLength = 7;
const int minSpecialLength = 3;
const int maxSpecialLength = 100;

/*!
 * \brief Displays data relevant to hospitals
 * The hospital window is the UI shown the hospital. It is where an administrator may see the filled and available beds,
 * the status of the hospital, and interact with this data.
 * Authors: Charles Ranson
 */
class HospitalWindow : public QMainWindow {
    Q_OBJECT

public:
    /*!
     * Used to initialize the main hospital window and provide for each input
     */
    explicit HospitalWindow(QWidget *parent = nullptr);
    /// Deletes associated heap memory
    ~HospitalWindow() override;

private slots:
    /// Slot to call after the addHospital button is pressed
    void on_addHospital_pressed();
    /// Slot for if christainsburg is Checked
    void on_Christiansburg_pressed();
    /// Slot for if Roanoke is pressed
    void on_Roanoke_pressed();
    /// Slot for if Lynchburg is pressed
    void on_Lynchburg_pressed();
    /// Slot for if Princeton is pressed
    void on_Princeton_pressed();
    /// Slot for if Bristol is pressed
    void on_Bristol_pressed();
    /// Signals to delete a hospital from the database
    void on_deleteHospitalButton_pressed();
    /// Callback for if the bedSlider is changed
    void on_bedSlider_valueChanged(int value);
    /// Adds beds to a current hospital
    void on_addBedsButton_pressed();
    /*!
     * This method makes a POST request on a polling basis to get the current
     * status of hospitals for plotting purposes
     */
    void getStatus();
    /*!
     * The next set of functions are all call backs for each of the checkboxes
     * for when a special value is Checked
     */
    void on_injuryCheckbox_stateChanged(int arg1);
    /// Burn checkbox callback
    void on_burnCheckbox_stateChanged(int arg1);
    /// Virus checkbox callback
    void on_virusCheckbox_stateChanged(int arg1);
    /// Radiation checkbox callback
    void on_radiationCheckBox_stateChanged(int arg1);
    /// Psychiatric checkbox callback
    void on_psychiatricCheckbox_stateChanged(int arg1);
    /// Respiratory checkbox callback
    void on_respiratoryCheckbox_stateChanged(int arg1);
    /// Cardiac checkbox callback
    void on_cardiacCheckbox_stateChanged(int arg1);
    /// Scan required checkbox callback
    void on_scanCheckbox_stateChanged(int arg1);

private:
    Ui::HospitalWindow *ui;
    std::size_t bedCount;
    int hospitalIndex;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::vector<Hospital> * hospitals_in_use;
    std::string hospital_name;
    std::pair<double,double> location;
    std::vector<bool> specialIndeces;
    int beds2Add;
    Client * hospitalClient;
    QTimer * timer;
    /// Method will be used to generate the bed data and display on the UI chart
    void generateBedData(const Hospital h);
    /// Uncheck all checkboxes so next hospital can be user configured
    void uncheck();

};
#endif // HOSPITALWINDOW_H
