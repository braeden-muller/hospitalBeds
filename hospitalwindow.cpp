#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"

HospitalWindow::HospitalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HospitalWindow) {
    ui->setupUi(this);
}

HospitalWindow::~HospitalWindow() {
    delete ui;
}

