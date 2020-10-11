#include "doctorwindow.h"
#include "./ui_doctorwindow.h"
#include <typeinfo>

//Method: Constructor
//Purpose: This is the default for the Doctor Window it is responsible for setting all added widgets
// to their default values along with initializing all private variables.
DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    //7 Special Symptoms for the Doctor to Choose From
    //Right now there are 5 hospital locations for our clients in SWVA
    QStringList listLocations = (QStringList() << "Christiansburg, VA" << "Roanoke, VA" << "Princeton, WV" << "Lyncburg, VA" << "Bristol, TN");
    ui->locationSelector->addItems(listLocations);
    ui->locationSelector->setCurrentIndex(0); //current location is Christiansburg, VA
    location = CHRISTIANSBURG;

    doctorClient = new Client(); //Create a client so the doctor can send POST requests via (REST)

    //TODO: may move the database access into the client.
    DBConnection *newDatabase = DBConnection::getInstance();
    std::string errMsg;
    newDatabase->importDatabase(errMsg);
    hospitals = newDatabase->getHospitals();
    
    Hospital h; //temp hospital that does nothing
    for (int i = 0; i < NUM_HOSPITALS; ++i) //preset to 5 hospitals
    {
        hospitals->push_back(std::make_tuple(0,h,false)); //initial all hospitals to not registered
    }
    start = std::chrono::steady_clock::now(); //this is the base time that all timestamps will stem from
}

//Method: destructor
//Purpose: Cleaning up all allocated memory on the heap. Including the client pointer and hospital
// pointer.
DoctorWindow::~DoctorWindow() {
    delete ui;
    delete doctorClient;
    if (doctorClient != nullptr)
        doctorClient = nullptr;
    delete hospitals;
    if (hospitals != nullptr)
        hospitals = nullptr;
}

//Method: on_requestBed_pressed
//Purpose: To serve as the callback function for when the doctor requests a bed. It creates a new bed and
// then sees if the hospital it desires is registered or not. If the hospital is registered it assigns the bed
// to the hospital, if not it registers a new hospital within the database.
void DoctorWindow::on_requestBed_pressed()
{
    Hospital h; //Create a temp hospital before seeing if it is already in vector
    auto bedSpec = web::json::value::object(); //Initialize all pertinent json objects before making request.
    utility::string_t id = utility::conversions::to_string_t("id"); //PROBABLY CHANGE TO MACRO FOR THESE LATER
    utility::string_t isFull = utility::conversions::to_string_t("isFull");
    utility::string_t timestamp = utility::conversions::to_string_t("timestamp");
    utility::string_t handles = utility::conversions::to_string_t("handles");
    utility::string_t special = utility::conversions::to_string_t("special");
    bedSpec[isFull] = web::json::value(true);
    auto now = std::chrono::steady_clock::now();
    bedSpec[timestamp] = web::json::value::number(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
    std::set<condition> addedHandles;
    utility::string_t handleVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                        utility::conversions::to_string_t("virus"), utility::conversions::to_string_t("radiation"),
                                        utility::conversions::to_string_t("psychiatric"), utility::conversions::to_string_t("respiratory"),
                                        utility::conversions::to_string_t("cardiac"), utility::conversions::to_string_t("scan")};

    for (auto s = 0; s < 7; ++s)
    {
      addedHandles.emplace(conditions_by_name[handleVector[s]].c);
    }
    int count = 0;
    for (auto a : addedHandles)
    {
      bedSpec[handles][count++] = JSTR(name_by_conditions[a]);
    }
    std::set<condition> addedSpecial;
    if (specialVector.empty())
    {
      ui->requestResponse->clear();
      ui->requestResponse->setText("Please select an ailment");
      return;
    }
    for (auto it : specialVector)
    {
      addedSpecial.emplace(conditions_by_name[it].c);
    }
    count = 0;
    for (auto s : addedSpecial)
    {
      bedSpec[special][count++] = JSTR(name_by_conditions[s]);
    }
    if (!(std::get<2>((*hospitals)[location])))
    {
      addHospital(h, bedSpec); //this function will add a new hospital
    }
    bedSpec[id] = web::json::value::number(std::get<0>((*hospitals)[location]));
    Bed b(bedSpec); //create a new bed with all parameters from bedSpec json object
    if (std::get<1>((*hospitals)[location]).add_bed(b)) //add the bed to the hospital
    {
      std::get<0>((*hospitals)[location])++; //increase the bed ID
      doctorClient->sendRequest("POST", std::get<1>((*hospitals)[location]).jsonify()); //send the post request
      ui->requestResponse->setText("Your bed has been requested!");
    }
    else
    {
      ui->requestResponse->setText("The hospital is full!");
    }
    specialVector.clear();
    ui->injury_checkbox->setChecked(Qt::Unchecked);
    ui->scan_checkbox->setChecked(Qt::Unchecked);
    ui->radiation_checkbox->setChecked(Qt::Unchecked);
    ui->virus_checkbox->setChecked(Qt::Unchecked);
    ui->respiratory_checkbox->setChecked(Qt::Unchecked);
    ui->burn_checkbox->setChecked(Qt::Unchecked);
    ui->cardiac_checkbox->setChecked(Qt::Unchecked);
}

//Method: on_locationSelector_currentIndexChanged
//Purpose: Callback function for the doctor changing the location of the hospital they desire
// to access
void DoctorWindow::on_locationSelector_currentIndexChanged(int index)
{
    ui->requestResponse->clear(); //clear the textbox before the new request
    location = (index == 0) ? CHRISTIANSBURG : (index == 1) ? ROANOKE : (index == 2) ? PRINCETON : (index == 3) ? LYNCHBURG :
               (index == 4) ? BRISTOL : ROANOKE;
}

//Method: addHospital
//Purpose: To initialize and add a new hospital to the vector, the hospital will be at one of
// 5 locations. All latitude and longitude values were generated via an online map
void DoctorWindow::addHospital(Hospital & h, web::json::value & bedSpec)
{
  switch (location)
  {
  case CHRISTIANSBURG:
      h.set_name("New River Valley Medical Center");
      h.set_location(37.089081, -80.505592);
      h.set_id(location);
      (*hospitals)[location] = std::make_tuple(0,h,true);
      break;
  case ROANOKE:
      h.set_name("Carilion Roanoke Memorial Hospital");
      h.set_location(37.252090, -79.942436);
      h.set_id(location);
      (*hospitals)[location] = std::make_tuple(0,h,true);
      break;
  case PRINCETON:
      h.set_name("Princeton Community Hospital");
      h.set_location(37.363190, -81.113136);
      h.set_id(location);
      (*hospitals)[location] = std::make_tuple(0,h,true);
      break;
  case LYNCHBURG:
      h.set_name("Centra Lynchburg General Hospital");
      h.set_location(37.416648, -79.169753);
      h.set_id(location);
      (*hospitals)[location] = std::make_tuple(0,h,true);
      break;
  case BRISTOL:
      h.set_name("Bristol Regional Medical Center");
      h.set_location(36.584577,-82.251241);
      h.set_id(location);
      (*hospitals)[location] = std::make_tuple(0,h,true);
      break;
  }
}

void DoctorWindow::on_injury_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("injury"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("injury");});
  }
}

void DoctorWindow::on_burn_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("burn"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("burn");});
  }
}

void DoctorWindow::on_virus_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("virus"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("virus");});
  }
}

void DoctorWindow::on_radiation_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("radiation"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("radiation");});
  }
}

void DoctorWindow::on_scan_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("scan"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("scan");});
  }
}

void DoctorWindow::on_respiratory_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("respiratory"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("respiratory");});
  }
}

void DoctorWindow::on_cardiac_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    specialVector.push_back(utility::conversions::to_string_t("cardiac"));
  }
  else
  {
    auto end = std::remove_if(specialVector.begin(), specialVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("cardiac");});
  }
}
