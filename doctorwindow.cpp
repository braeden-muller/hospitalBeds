#include "doctorwindow.h"
#include "./ui_doctorwindow.h"

DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    ailment = COVID; //default ailement is currently COVID
    doctorClient = new Client();
}

DoctorWindow::~DoctorWindow() {
    delete ui;
    delete doctorClient;
    if (doctorClient != nullptr)
        doctorClient = nullptr;
}

void DoctorWindow::on_requestBed_pressed()
{
    //TODO: send request to REST client
    bool requestSent = true;
    switch(ailment)
    {
        case COVID:
            doctorClient->sendRequest("register","Carilion-Radford", "POST");
            break;
        case ER:
            doctorClient->sendRequest("register","Carilion-Radford", "POST");
            break;
        case INJURY:
            doctorClient->sendRequest("register","Carilion-Radford", "POST");
            break;
        case ILLNESS:
            doctorClient->sendRequest("register","Carilion-Radford", "POST");
            break;
        default:
            ui->requestResponse->setText("Unable to get bed, sorry.");
            requestSent = false;
            break;
    }
    if (requestSent)
        ui->requestResponse->setText("Your bed has been requested!");
}

void DoctorWindow::on_covidButton_pressed()
{
    ailment = COVID;
}

void DoctorWindow::on_erButton_pressed()
{
    ailment = ER;
}

void DoctorWindow::on_injuryButton_pressed()
{
    ailment = INJURY;
}

void DoctorWindow::on_illnessButton_pressed()
{
    ailment = ILLNESS;
}
