#include "doctorwindow.h"
#include "./ui_doctorwindow.h"
#include <typeinfo>
#include <iostream>

//Method: Constructor
//Purpose: This is the default for the Doctor Window it is responsible for setting all added widgets
// to their default values along with initializing all private variables.
DoctorWindow::DoctorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
    //7 Special Symptoms for the Doctor to Choose From
    //Right now there are 5 hospital locations for our clients in SWVA
    //Go ahead and set the placeholder text in the latitude and longitude text boxes
    ui->latitudeLineEdit->setPlaceholderText("37.18");
    ui->longitudeLineEdit->setPlaceholderText("-80.53");
    longitude = -80.53;
    latitude = 37.18;
    timer = new QTimer(parent);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(getStatus()));
    timer->start(45000); //time specified in ms, so poll every 45 seconds
    patient_ids = new std::vector<std::string>;
    untreated_patients = new std::vector<Patient>;
    doctorClient = new Client(); //Create a client so the doctor can send POST requests via (REST)
}

//Method: destructor
//Purpose: Cleaning up all allocated memory on the heap. Including the client pointer and hospital
// pointer.
DoctorWindow::~DoctorWindow() {
    delete ui;
    if (doctorClient != nullptr)
    {
      delete doctorClient;
      doctorClient = nullptr;
    }
    if (patient_ids != nullptr)
    {
      delete patient_ids;
      patient_ids = nullptr;
    }
    if (untreated_patients != nullptr)
    {
      delete untreated_patients;
      untreated_patients = nullptr;
    }
    if (timer != nullptr)
    {
      delete timer;
      timer = nullptr;
    }
}

//Method: on_requestBed_pressed
//Purpose: To serve as the callback function for when the doctor requests a bed. It creates a new bed and
// then sees if the hospital it desires is registered or not. If the hospital is registered it assigns the bed
// to the hospital, if not it registers a new hospital within the database.
void DoctorWindow::on_requestBed_pressed()
{
    Patient p;
    p.set_location(latitude, longitude); //add the players latitude and longitude
    //p.set_id(patientId++);
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
    p.set_assigned_hospital("NONE"); //just set the assigned hospital to None as it has not been treated
    doctorClient->sendRequest("POST", p.jsonify()); //send the post request
    untreated_patients->push_back(p);
    ui->requestResponse->setText("Your bed has been requested!");
    patient_ids->push_back(p.get_id());
    clear_checkboxes();
}

//This method will clear the checkboxes from the doctor ui.
void DoctorWindow::clear_checkboxes(void)
{
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

//This method will display in a QDialog Box the number of patients that have been treated
void DoctorWindow::getStatus()
{
   std::stringstream ss;
   web::json::value j_patient_status;
   int i = 0;
   utility::string_t pStatus = utility::conversions::to_string_t("patient_statuses");
   for (auto it : *patient_ids)
   {
     j_patient_status[pStatus][i++] = JSTR(it);
   }
   try
   {
     auto r = doctorClient->sendRequest("POST", j_patient_status);
     for (int i = 0; i < 100000; ++i); //Kill me now

    web::json::value test = r;
    auto arr = test.as_array();
    std::vector<Patient> temp;
    std::string denied_ids = "Denied IDs: \n";
    std::string accepted_ids = "Accepted IDs: \n";
    for (auto &a : arr)
    {
      Patient p(a["patient"]);
      temp.push_back(p);
    }
    //Loop through returned patients
    for (auto a : temp)
    {
      int treated = -1;
      for (auto i = 0; i < untreated_patients->size(); ++i)
      {
        //See if a decision has been made on the patient by looping through and chekcing
        //all untreated patients ids.
        std::cout << "In here " << std::endl;
        std::cout << untreated_patients->at(i).get_id();
        if (a.get_id() == untreated_patients->at(i).get_id())
        {
          std::cout << "Found one" << std::endl;
          treated = i;
          //If declined push into denied ids vector.
          if (a.get_assigned_hospital() == "DECLINED")
          {
            denied_ids += a.get_id() + '\n';
          }
          //If accpeted push into accppted ids vector
          else
          {
            accepted_ids += a.get_id() + '\n';
          }
          break;
        }
      }
      if (treated != -1)
      {
        untreated_patients->erase(untreated_patients->begin() + treated);
      }
    }
    //Dumb conversion to const char * for compilation purposes.
    std::cout << "Denied: " << denied_ids << '\n' << "Accepted: " << accepted_ids << std::endl;
    std::string output = denied_ids + accepted_ids;
    const char * msg = output.c_str();
    QMessageBox::information(this,tr("Patient Statuses"), tr(msg));
   }
   catch(std::exception& e)
   {
     std::cout << e.what() << std::endl;
     QMessageBox::information(this,tr("Error"), tr("Please start the server."));
   }
}
