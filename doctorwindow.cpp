#include "doctorwindow.h"
#include "./ui_doctorwindow.h"

DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
}

DoctorWindow::~DoctorWindow() {
    delete ui;
}

void DoctorWindow::on_requestBed_pressed()
{
    //TODO: send request to REST client
    ui->requestResponse->setText("Your bed has been requested!");
}
