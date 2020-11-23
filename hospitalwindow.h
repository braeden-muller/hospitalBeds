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

/*!
 * \brief Displays data relevant to hospitals
 * The hospital window is the UI shown the hospital. It is where an administrator may see the filled and available beds,
 * the status of the hospital, and interact with this data.
 */
class HospitalWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit HospitalWindow(QWidget *parent = nullptr);
    ~HospitalWindow() override;

private slots:

    void on_addHospital_pressed();

    void on_Christiansburg_pressed();

    void on_Roanoke_pressed();

    void on_Lynchburg_pressed();

    void on_Princeton_pressed();

    void on_Bristol_pressed();

    void on_deleteHospitalButton_pressed();

    void on_bedSlider_valueChanged(int value);

    void on_addBedsButton_pressed();

    void getStatus();

    void on_injuryCheckbox_stateChanged(int arg1);

    void on_burnCheckbox_stateChanged(int arg1);

    void on_virusCheckbox_stateChanged(int arg1);

    void on_radiationCheckBox_stateChanged(int arg1);

    void on_psychiatricCheckbox_stateChanged(int arg1);

    void on_respiratoryCheckbox_stateChanged(int arg1);

    void on_cardiacCheckbox_stateChanged(int arg1);

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
    void generateBedData(const Hospital h); //TODO: This function will be generating the bed data
};
#endif // HOSPITALWINDOW_H
