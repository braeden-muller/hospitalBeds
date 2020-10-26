#ifndef HOSPITALWINDOW_H
#define HOSPITALWINDOW_H

#include <QMainWindow>
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
    void on_hospital_data_pressed();

    void on_addHospital_pressed();

private:
    Ui::HospitalWindow *ui;
    int bedCount;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::vector<Hospital> *hospitals;
    Client * hospitalClient;
    void generateBedData(); //TODO: This function will be generating the bed data
};
#endif // HOSPITALWINDOW_H
