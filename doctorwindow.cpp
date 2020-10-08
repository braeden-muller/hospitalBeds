#include "doctorwindow.h"
#include "./ui_doctorwindow.h"
#include <typeinfo>

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
    start = std::chrono::steady_clock::now();
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

    //Initialize all pertinent json objects before making request.
    auto bedSpec = web::json::value::object();
    utility::string_t id = utility::conversions::to_string_t("id");
    utility::string_t isFull = utility::conversions::to_string_t("isFull");
    utility::string_t timestamp = utility::conversions::to_string_t("timestamp");
    utility::string_t handles = utility::conversions::to_string_t("handles");
    utility::string_t special = utility::conversions::to_string_t("special");

    //Plave values in JSON format
    bedSpec[id] = web::json::value::number(bedCall);
    ++bedCall;
    bedSpec[isFull] = web::json::value(true);
    auto now = std::chrono::steady_clock::now();
    bedSpec[timestamp] = web::json::value::number(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());

    std::set<condition> addedHandles;
    utility::string_t handleVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                        utility::conversions::to_string_t("radiation"), utility::conversions::to_string_t("virus")};

    auto json_handles_val = web::json::value(handleVector);
    auto & json_handles_arr = json_handles_val.as_array();
    for (auto & j : json_handles_arr)
    {
      addedHandles.emplace(conditions_by_name[j.as_string()].c);
    }
    int count = 0;
    for (auto h : addedHandles)
    {
      bedSpec[handles][count++] = JSTR(name_by_conditions[h]);
    }
    std::cout << "Made it before sepcial " << std::endl;
    std::set<condition> addedSpecial;
    utility::string_t specialVector[] = {utility::conversions::to_string_t(ailment)};
    auto json_special_val = web::json::value(specialVector);
    auto & json_special_arr = json_special_val.as_array();
    for (auto & j : json_special_arr)
    {
      addedSpecial.emplace(conditions_by_name[j.as_string()].c);
    }
    count = 0;
    for (auto s : addedSpecial)
    {
      bedSpec[special][count++] = JSTR(name_by_conditions[s]);
    }
    std::cout << "Before call to bed spec" << std::endl;
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
