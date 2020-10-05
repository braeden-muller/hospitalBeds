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
    Hospital h; //not actually setting the bed, yet this should be fixed
    std::string hos = "";
    switch(ailment)
    {
     //Potentially wait for response to ensure request was sent properly, this could be
     //printing something to the screen.
        case COVID:
            h.findBed("COVID");
            hos = h.stringify();
            doctorClient->sendRequest("register",hos, "POST");
            break;
        case ER:
            h.findBed("ER");
            hos = h.stringify();
            doctorClient->sendRequest("register",hos, "POST");
            break;
        case INJURY:
            h.findBed("INJURY");
            hos = h.stringify();
            doctorClient->sendRequest("register",hos, "POST");
            break;
        case ILLNESS:
            h.findBed("ILLNESS");
            hos = h.stringify();
            doctorClient->sendRequest("register",hos, "POST");
            break;
      }
      ui->requestResponse->setText("Your bed has been requested!");
}

void DoctorWindow::on_ailmentSelector_currentIndexChanged(int index)
{
    ui->requestResponse->clear(); //clear the text box before new request
    ailment = (index == 0) ? COVID : (index == 1) ? ER : (index == 2) ? INJURY : (index == 3) ? ILLNESS : COVID;
}
