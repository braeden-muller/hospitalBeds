#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"

HospitalWindow::HospitalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HospitalWindow) {
    ui->setupUi(this);
    hospitalClient = new Client();
}

HospitalWindow::~HospitalWindow() {
    delete ui;
    delete hospitalClient;
    if (hospitalClient != nullptr)
      hospitalClient = nullptr;
}

//Method will be used to generate the bed data and display on the UI chart 
void HospitalWindow::generateBedData(void)
{

    hospitalClient->sendRequest("POST", JSTR("hospital")); //send request to get a hospital (Wait till db is done)
    //Code that will be used to create a QChart
    QChart *chart = new  QChart(); //create chart
    QBarSeries *series = new QBarSeries(); //create series
    QBarSet *set = new QBarSet("Patients"); //create set for number of patients being treated
    QFont *qFont = new QFont(); //used for setting the fonts of labels and titles if needed
    qFont->setPointSize(4); //arbitrary font size
    set->setLabelFont(*qFont);
    set->append(5); //put data in the set this will be # of patients. 5 is just an arbitrary number chosen by the student
    series->append(set); //append the set to a series
    chart->addSeries(series); //add series to chart

}

void HospitalWindow::on_hospital_data_pressed()
{
    generateBedData();
}
