#include "doctorwindow.h"
#include "./ui_doctorwindow.h"

DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    QStringList listAilments = (QStringList() << "injury" << "burn" << "virus" << "radiation" << "psychiatric care" << "respiratory issues" << "cardiac issues");
    ui->ailmentSelector->addItems(listAilments);
    ui->ailmentSelector->setCurrentIndex(0);
    ailment = "injury"; //default ailement is currently injury
    QStringList listLocations = (QStringList() << "Christiansburg, VA" << "Roanoke, VA" << "Princeton, WV" << "Lyncburg, VA" << "Bristol, TN");
    ui->locationSelector->addItems(listLocations);
    ui->locationSelector->setCurrentIndex(0);
    location = CHRISTIANSBURG;
    doctorClient = new Client();
    hospitals = new std::vector<Hospital>();
    Hospital h;
    bedCall = 0;
    h.set_name("Not Registered"); //means that this hospital is not registered in database
    for (int i = 0; i < NUM_HOSPITALS; ++i)
    {
        hospitals->push_back(h); //initial all hospitals to not registered
    }
}

DoctorWindow::~DoctorWindow() {
    delete ui;
    delete doctorClient;
    if (doctorClient != nullptr)
        doctorClient = nullptr;
    delete hospitals;
    if (hospitals != nullptr)
        hospitals = nullptr;
}

void DoctorWindow::on_requestBed_pressed()
{
    Hospital h;
    auto bedSpec = web::json::value::object();
    utility::string_t id = utility::conversions::to_string_t("id");
    utility::string_t isFull = utility::conversions::to_string_t("isFull");
    utility::string_t timestamp = utility::conversions::to_string_t("timestamp");
    utility::string_t handles = utility::conversions::to_string_t("handles");
    utility::string_t special = utility::conversions::to_string_t("special");
    bedSpec[id] = web::json::value(utility::conversions::to_string_t(std::to_string(bedCall)));
    ++bedCall;
    bedSpec[isFull] = web::json::value(true);
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    utility::string_t time = utility::conversions::to_string_t(std::ctime(&now));
    bedSpec[timestamp] = web::json::value(time);
    utility::string_t currentHandles = utility::conversions::to_string_t("injury, burn, virus, radiation");
    bedSpec[handles] = web::json::value(currentHandles);
    auto specificNeed = utility::conversions::to_string_t(ailment);
    bedSpec[special] = web::json::value(specificNeed);
    Bed b(bedSpec);
    h.add_bed(b); //add bed
    switch (location)
    {
    case CHRISTIANSBURG:
        h.set_name("New River Valley Medical Center");
        h.set_location(37.089081, -80.505592);
        break;
    case ROANOKE:
        h.set_name("Carilion Roanoke Memorial Hospital");
        h.set_location(37.252090, -79.942436);
        break;
    case PRINCETON:
        h.set_name("Princeton Community Hospital");
        h.set_location(37.363190, -81.113136);
        break;
    case LYNCHBURG:
        h.set_name("Centra Lynchburg General Hospital");
        h.set_location(37.416648, -79.169753);
        break;
    case BRISTOL:
        h.set_name("Bristol Regional Medical Center");
        h.set_location(36.584577,-82.251241);
        break;
    }
     doctorClient->sendRequest("POST", h.jsonify());
     ui->requestResponse->setText("Your bed has been requested!");
}

void DoctorWindow::on_ailmentSelector_currentIndexChanged(int index)
{
    ui->requestResponse->clear(); //clear the text box before new request
    ailment = (index == 0) ? "injury" : (index == 1) ? "burn" : (index == 2) ? "virus" : (index == 3) ? "radiation" : "injury";
}

void DoctorWindow::on_locationSelector_currentIndexChanged(int index)
{
    ui->requestResponse->clear(); //clear the textbox before the new request
    location = (index == 0) ? CHRISTIANSBURG : (index == 1) ? ROANOKE : (index == 2) ? PRINCETON : (index == 3) ? LYNCHBURG : (index == 4) ? BRISTOL : ROANOKE;

}

