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
    //Potentially wait for response to ensure request was sent properly, this could be
    //printing something to the screen.
        case COVID:
            doctorClient->sendRequest("register","Carilion-Radford", "POST");
            break;
        case ER:
            doctorClient->sendRequest("register","Carilion-Roanoke", "POST");
            break;
        case INJURY:
            doctorClient->sendRequest("register","Centra-Lynchburg", "POST");
            break;
        case ILLNESS:
            doctorClient->sendRequest("register","Centra-Charlottesville", "POST");
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
    ui->requestResponse->clear(); //clear the text box before new request
    ailment = (index == 0) ? COVID : (index == 1) ? ER : (index == 2) ? INJURY : (index == 3) ? ILLNESS : COVID;
}
