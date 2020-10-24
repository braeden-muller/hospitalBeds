#include "doctorwindow.h"
#include "./ui_doctorwindow.h"
#include <typeinfo>
#include <iostream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::uuids;

//Method: Constructor
//Purpose: This is the default for the Doctor Window it is responsible for setting all added widgets
// to their default values along with initializing all private variables.
DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    //7 Special Symptoms for the Doctor to Choose From
    //Right now there are 5 hospital locations for our clients in SWVA
    patientId = 0;
    //Go ahead and set the placeholder text in the latitude and longitude text boxes
    ui->latitudeLineEdit->setPlaceholderText("37.18");
    ui->longitudeLineEdit->setPlaceholderText("-80.53");
    longitude = -80.53;
    latitude = 37.18;

    doctorClient = new Client(); //Create a client so the doctor can send POST requests via (REST)
}

//Method: destructor
//Purpose: Cleaning up all allocated memory on the heap. Including the client pointer and hospital
// pointer.
DoctorWindow::~DoctorWindow() {
    delete ui;
    delete doctorClient;
    if (doctorClient != nullptr)
        doctorClient = nullptr;
}

//Method: on_requestBed_pressed
//Purpose: To serve as the callback function for when the doctor requests a bed. It creates a new bed and
// then sees if the hospital it desires is registered or not. If the hospital is registered it assigns the bed
// to the hospital, if not it registers a new hospital within the database.
void DoctorWindow::on_requestBed_pressed()
{
    random_generator gen;
    std::string playerId = boost::lexical_cast<std::string>(gen());
    Patient p;
    p.set_location(latitude, longitude); //add the players latitude and longitude
    p.set_id(playerId);
    auto patientSpec = web::json::value::object();
    utility::string_t ailments = utility::conversions::to_string_t("ailments");
    std::set<condition> addedAilments;
    if (ailmentVector.empty())
    {
      ui->requestResponse->clear();
      ui->requestResponse->setText("Please select an ailment");
      return;
    }
    for (auto it : ailmentVector)
    {
      addedAilments.emplace(conditions_by_name[it].c);
    }
    p.set_ailments(addedAilments);
    p.set_treated(false);
    doctorClient->sendRequest("POST", p.jsonify()); //send the post request
    ui->requestResponse->setText("Your bed has been requested!");

    //TODO: Wrap this into a function later
    ailmentVector.clear();
    ui->injury_checkbox->setChecked(Qt::Unchecked);
    ui->scan_checkbox->setChecked(Qt::Unchecked);
    ui->radiation_checkbox->setChecked(Qt::Unchecked);
    ui->virus_checkbox->setChecked(Qt::Unchecked);
    ui->respiratory_checkbox->setChecked(Qt::Unchecked);
    ui->burn_checkbox->setChecked(Qt::Unchecked);
    ui->cardiac_checkbox->setChecked(Qt::Unchecked);
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_injury_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("injury"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("injury");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_burn_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("burn"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("burn");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_virus_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("virus"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("virus");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_radiation_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("radiation"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("radiation");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_scan_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("scan"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("scan");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_respiratory_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("respiratory"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("respiratory");});
  }
}

//Callback function to update ailment on change of the checkbox
void DoctorWindow::on_cardiac_checkbox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked)
  {
    ailmentVector.push_back(utility::conversions::to_string_t("cardiac"));
  }
  else
  {
    auto end = std::remove_if(ailmentVector.begin(), ailmentVector.end(),
               [](utility::string_t s){return s == utility::conversions::to_string_t("cardiac");});
  }
}

//This latitude will be the players latitude that is sent to the server
void DoctorWindow::on_latitudeLineEdit_textChanged(const QString &arg1)
{
    //Get the QString and map it to a double
    latitude = arg1.toDouble();

    //Force latitude to be in the specified area
    if (latitude > 38.0)
    {
      latitude = 38;
      ui->latitudeLineEdit->setPlaceholderText("38.0");
    }
    else if (latitude > 36.0)
    {
      latitude = 36;
      ui->latitudeLineEdit->setPlaceholderText("36.0");
    }
}

//This longitude will be the players longitude that is sent to the server
void DoctorWindow::on_longitudeLineEdit_textChanged(const QString &arg1)
{
    //Get the QString and map it to a double
    //TODO: In the hospital window work on a location determinator function
    longitude = arg1.toDouble();

    //Force the longitude to be in the desired in the area
    if (longitude < -83.0)
    {
      longitude = -83.0;
      ui->longitudeLineEdit->setPlaceholderText("-83.0");
    }
    else if (longitude > -78.5)
    {
      latitude = -78.5;
      ui->latitudeLineEdit->setPlaceholderText("-78.5");
    }
}
