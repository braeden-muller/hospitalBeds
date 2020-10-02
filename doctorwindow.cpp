#include "doctorwindow.h"
#include "./ui_doctorwindow.h"

DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    QStringList list = (QStringList() << "COVID" << "ER" << "Injury" << "Illness");
    ui->ailmentSelector->addItems(list);
    ui->ailmentSelector->setCurrentIndex(0); //default to COVID
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

void DoctorWindow::on_ailmentSelector_currentIndexChanged(int index)
{
    ailment = (index == 0) ? COVID : (index == 1) ? ER : (index == 2) ? INJURY : (index == 3) ? ILLNESS : COVID;
}
