#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"

HospitalWindow::HospitalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HospitalWindow) {
    ui->setupUi(this);
    hospitalClient = new Client();
    hospitals_in_use = new std::vector<Hospital>;
    hospitalIndex = 0;
    bedCount = 0;
    hospital_name = "New River Valley Medical Center";
    location = std::make_pair(0.0,0.0); //just give default location of 0,0
    start = std::chrono::steady_clock::now();
    beds2Add = 1;
    ui->beds_count_edit->setText(QString::number(0));
    //Poll for hospitals data
    timer = new QTimer(parent);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(getStatus()));
    timer->start(30000); //time specified in ms, so poll every 1 minute

    //fill the array for special indeces
    for (auto i = 0; i < 8; ++i)
        specialIndeces.push_back(false);
}

HospitalWindow::~HospitalWindow()
{
    delete ui;
    if (hospitalClient != nullptr)
    {
      delete hospitalClient;
      hospitalClient = nullptr;
    }
    if (timer != nullptr)
    {
      delete timer;
      timer = nullptr;
    }
    if (hospitals_in_use != nullptr)
    {
      delete hospitals_in_use;
      hospitals_in_use = nullptr;
    }
}

void HospitalWindow::generateBedData(const Hospital h)
{
    //Note that this code uses HW1 as a reference (source used)
    //Code that will be used to create a QChart
    QChart *chart = new  QChart(); //create chart
    QBarSeries *series = new QBarSeries(); //create series
    QBarSet *set = new QBarSet("Patients"); //create set for number of patients being treated
    QFont *qFont = new QFont(); //used for setting the fonts of labels and titles if needed
    qFont->setPointSize(4); //arbitrary font size
    set->setLabelFont(*qFont);
    //reinitialize vector each time you get the specials
    std::vector<int> numForSpecial = {0,0,0,0,0,0,0,0};
    //Num for special indeces: injury: 0, burn: 1, radiation: 2,
    //                        psychiatric: 3, respiratory: 4, cardiac: 5, scan: 6
    for (auto i = 0; i < h.get_size();++i)
    {
      std::string special = h.get_bed(i).get_special(); //get the special for each bed
      if (special.length() > minSpecialLength && special.length() < maxSpecialLength)  //checks to make sure special length is valid
      {
        if (special.find("injur") != std::string::npos)
        {
          numForSpecial[0]++;
        }
        if (special.find("burn") != std::string::npos)
        {
          numForSpecial[1]++;
        }
        if (special.find("virus") != std::string::npos)
        {
          numForSpecial[2]++;
        }
        if (special.find("radiat") != std::string::npos)
        {
          numForSpecial[3]++;
        }
        if (special.find("psych") != std::string::npos)
        {
          numForSpecial[4]++;
        }
        if (special.find("respirat") != std::string::npos)
        {
          numForSpecial[5]++;
        }
        if (special.find("cardi") != std::string::npos)
        {
          numForSpecial[6]++;
        }
        if (special.find("scan") != std::string::npos)
        {
          numForSpecial[7]++;
        }
      }
    }
    for (auto i = 0; i < numForSpecial.size(); ++i)
    {
        set->append(numForSpecial[i]); //append for plotting purposes
    }
    series->append(set); //append the set to a series
    chart->addSeries(series); //add series to chart
    chart->setTitleFont(*qFont);
    chart->setTitle("Hospital Bed Data");

    QBarCategoryAxis *xAxis = new QBarCategoryAxis(); //create x axis
    QStringList specialsList = (QStringList() << "injury" << "burn" << "virus" << "radiation"
                                << "psychiatric" << "respiratory" << "cardiac" << "scan");
    xAxis->append(specialsList); //append the special list containing each bed to the hospital
    xAxis->setLabelsAngle(270);
    chart->addAxis(xAxis, Qt::AlignBottom); //add axis to chart
    series->attachAxis(xAxis); //attach x axis

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0,*std::max_element(numForSpecial.begin(),numForSpecial.end()));
    qFont->setPointSize(5);
    yAxis->setLabelsFont(*qFont);
    chart->addAxis(yAxis, Qt::AlignLeft);

    ui->graphicsView->setChart(chart); //Update the graphics view
}

void HospitalWindow::getStatus()
{
  try
  {
    for (auto i= 0 ; i < hospitals_in_use->size();++i)
    {
      Hospital h = hospitals_in_use->at(i);
      auto rec = hospitalClient->sendRequest("POST", h.jsonify());
      Hospital recHospital(rec);
      hospitals_in_use->at(i) = recHospital;
      QMessageBox::information(this,tr("Msg"), tr("The hospital has patients being treated"));
    }
  }
  catch(std::exception& e) //If unable to connect to server
  {
      std::cout << e.what() << std::endl;
      QMessageBox::information(this,tr("Error"), tr("Sorry the connection seems to be disrupted."));
  }

}

void HospitalWindow::on_addHospital_pressed()
{
    Hospital h; //Create a temp hospital before seeing if it is already in vector

    h.set_location(location.first, location.second);
    h.set_name(hospital_name);
    //Create all bed specifications for the hospital
    utility::string_t specialVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                            utility::conversions::to_string_t("virus"), utility::conversions::to_string_t("radiation"),
                                            utility::conversions::to_string_t("psychiatric"), utility::conversions::to_string_t("respiratory"),
                                            utility::conversions::to_string_t("cardiac"), utility::conversions::to_string_t("scan")};
    for (int i = 0; i < 8; ++i)
    {
      auto bedSpec = web::json::value::object(); //Initialize all pertinent json objects before making request.
      utility::string_t id = utility::conversions::to_string_t("id"); //Get Hospital ID
      utility::string_t isFull = utility::conversions::to_string_t("isFull");
      utility::string_t timestamp = utility::conversions::to_string_t("timestamp");
      utility::string_t handles = utility::conversions::to_string_t("handles");
      utility::string_t special = utility::conversions::to_string_t("special");
      bedSpec[isFull] = web::json::value(true); //set bed to being full for the initial treated bed
      auto now = std::chrono::steady_clock::now();

      //update timestamp
      bedSpec[timestamp] = web::json::value::number(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
      std::set<condition> addedHandles;
      std::set<condition> addedSpecials;
      utility::string_t handleVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                              utility::conversions::to_string_t("virus"), utility::conversions::to_string_t("radiation"),
                                              utility::conversions::to_string_t("psychiatric"), utility::conversions::to_string_t("respiratory"),
                                              utility::conversions::to_string_t("cardiac"), utility::conversions::to_string_t("scan")};

      //add in all necessary handles
      for (auto s = 0; s < handleLength; ++s)
      {
         addedHandles.emplace(conditions_by_name[handleVector[s]].c);
      }
      int count = 0;
      for (auto it : addedHandles)
      {
         bedSpec[handles][count++] = JSTR(name_by_conditions[it]);
      }

      //add in all necessary specials
      for (auto s = 0; s < specialIndeces.size(); ++s)
      {
          if (specialIndeces[s]) //check if special index has been selected
          {
              addedSpecials.emplace(conditions_by_name[specialVector[s]].c);
          }
      }

      //if the specials are not selected for a bed, display an error message
      if (addedSpecials.empty())
      {
          QMessageBox::information(this,tr("Error"), tr("Please enter valid specials for the hospital."));
          return; //return without sending a request
      }

      count = 0;
      for (auto it : addedSpecials)
      {
         bedSpec[special][count++] = JSTR(name_by_conditions[it]);
      }
      bedSpec[id] = bedCount;
      bedCount++;
      //Add one bed upon startup to the hospital
      Bed b(bedSpec); //create a new bed with all parameters from bedSpec json object
      h.add_bed(b); //add bed to the hospital
    }
    hospitalClient->sendRequest("POST", h.jsonify()); //send the post request
    bool hospital_already_added = false;
    if (!hospitals_in_use->empty())
    {
      for (auto i = 0; i < hospitals_in_use->size(); ++i)
      {
        if (h.get_name() == hospitals_in_use->at(i).get_name())
        {
          hospital_already_added = true; //set this to true to denote a hospital already being added
          break;
        }
      }
    }
    if (!hospital_already_added)
    {
      h.set_id(hospitalIndex);
      hospitalIndex++;
      hospitals_in_use->push_back(h); //add the hospital so it can be polled for data
    }
  generateBedData(h); //generates the bed data plot for the given hospital
  uncheck(); //uncheck each of the check boxes after the bed data has been generated
}

/*
* Note that the source used for the latitude and longitude coordinates
* https://www.latlong.net/convert-address-to-lat-long.html
* this was used to find the latitude and longitude for the provided hospitals
* given their address which was taken from their individual websites.
*/

void HospitalWindow::on_Christiansburg_pressed()
{
    location = std::make_pair(37.089081, -80.505592);
    hospital_name = "New River Valley Medical Center";
}

void HospitalWindow::on_Roanoke_pressed()
{
    location = std::make_pair(37.252090, -79.942436);
    hospital_name = "Carilion Roanoke Memorial Hospital";
}

void HospitalWindow::on_Lynchburg_pressed()
{
    location = std::make_pair(37.416648, -79.169753);
    hospital_name = "Centra Lynchburg General Hospital";
}

void HospitalWindow::on_Princeton_pressed()
{
    location = std::make_pair(37.363190, -81.113136);
    hospital_name = "Princeton Community Hospital";
}

void HospitalWindow::on_Bristol_pressed()
{
    location = std::make_pair(36.584577,-82.251241);
    hospital_name = "Bristol Regional Medical Center";
}

void HospitalWindow::on_deleteHospitalButton_pressed()
{
    Hospital h;
    h.set_location(location.first, location.second);
    h.set_name(hospital_name);
    bool hospital_exists = false;
    int index_to_erase = -1;
    for (auto i = 0; i < hospitals_in_use->size();++i)
    {
      if (h.get_name() == hospitals_in_use->at(i).get_name())
      {
        index_to_erase = i;
        hospital_exists = true; //hospital to delete exists 
        break;
      }
    }
    if (hospital_exists) //if hospital is found
    {
      QChart *chart = new  QChart();
      ui->graphicsView->setChart(chart); //clear the graphics view
      hospitalClient->sendRequest("DELETE", h.jsonify()); //send the delete request for the hospital
      if (index_to_erase != -1)
      {
        hospitals_in_use->erase(hospitals_in_use->begin()+index_to_erase); //choose the hospital to delete from user control
      }
    }
    else
    {
      QMessageBox::information(this,tr("Error"), tr("No hospitals are in use yet, please select hospital"));
    }

}

void HospitalWindow::on_bedSlider_valueChanged(int value)
{
    beds2Add = value;
    ui->beds_count_edit->clear();
    ui->beds_count_edit->setText(QString::number(value));
}


void HospitalWindow::on_addBedsButton_pressed()
{
    Hospital h; //Create a temp hospital before seeing if it is already in vector

    h.set_location(location.first, location.second);
    h.set_name(hospital_name);
    bool hospital_available = false;
    if (!hospitals_in_use->empty())
    {
      for (auto i = 0; i < hospitals_in_use->size(); ++i)
      {
        if (h.get_name() == hospitals_in_use->at(i).get_name())
        {
          h.set_id(hospitals_in_use->at(i).get_id());
          hospital_available = true;
          break;
        }
      }
    }
    if (hospital_available)
    {
      //Create all bed specifications for the hospital
      utility::string_t specialVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                              utility::conversions::to_string_t("virus"), utility::conversions::to_string_t("radiation"),
                                              utility::conversions::to_string_t("psychiatric"), utility::conversions::to_string_t("respiratory"),
                                              utility::conversions::to_string_t("cardiac"), utility::conversions::to_string_t("scan")};
      for (auto i = 0; i < beds2Add; ++i)
      {
          auto bedSpec = web::json::value::object(); //Initialize all pertinent json objects before making request.
          utility::string_t id = utility::conversions::to_string_t("id"); //Get Hospital ID
          utility::string_t isFull = utility::conversions::to_string_t("isFull");
          utility::string_t timestamp = utility::conversions::to_string_t("timestamp");
          utility::string_t handles = utility::conversions::to_string_t("handles");
          utility::string_t special = utility::conversions::to_string_t("special");
          bedSpec[isFull] = web::json::value(true); //set bed to being full for the initial treated bed
          auto now = std::chrono::steady_clock::now();

          //update timestamp
          bedSpec[timestamp] = web::json::value::number(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
          std::set<condition> addedHandles;
          std::set<condition> addedSpecials;
          utility::string_t handleVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                                  utility::conversions::to_string_t("virus"), utility::conversions::to_string_t("radiation"),
                                                  utility::conversions::to_string_t("psychiatric"), utility::conversions::to_string_t("respiratory"),
                                                  utility::conversions::to_string_t("cardiac"), utility::conversions::to_string_t("scan")};
          for (auto s = 0; s < handleLength; ++s)
          {
             addedHandles.emplace(conditions_by_name[handleVector[s]].c);
          }
          int count = 0;
          for (auto it : addedHandles)
          {
             bedSpec[handles][count++] = JSTR(name_by_conditions[it]);
          }
          //add in all necessary specials
          for (auto s = 0; s < specialIndeces.size(); ++s)
          {
              if (specialIndeces[s]) //check if special index has been selected
              {
                  addedSpecials.emplace(conditions_by_name[specialVector[s]].c);
              }
          }

          //if the specials are not selected for a bed, display an error message
          if (addedSpecials.empty())
          {
              QMessageBox::information(this,tr("Error"), tr("Please enter valid specials for the hospital."));
              return; //return without sending a request
          }

          count = 0;
          for (auto it : addedSpecials)
          {
             bedSpec[special][count++] = JSTR(name_by_conditions[it]);
          }

          bedSpec[id] = bedCount;
          ++bedCount;
          Bed b(bedSpec); //create a new bed with all parameters from bedSpec json object
          h.add_bed(b);//add the bed to the hospital

      }
      generateBedData(h);
      hospitalClient->sendRequest("POST", h.jsonify()); //send the post request
      uncheck();
    }

}

void HospitalWindow::on_injuryCheckbox_stateChanged(int arg1)
{
    specialIndeces[0] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_burnCheckbox_stateChanged(int arg1)
{
    specialIndeces[1] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_virusCheckbox_stateChanged(int arg1)
{
    specialIndeces[2] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_radiationCheckBox_stateChanged(int arg1)
{
    specialIndeces[3] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_psychiatricCheckbox_stateChanged(int arg1)
{
    specialIndeces[4] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_respiratoryCheckbox_stateChanged(int arg1)
{
    specialIndeces[5] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_cardiacCheckbox_stateChanged(int arg1)
{
    specialIndeces[6] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::on_scanCheckbox_stateChanged(int arg1)
{
    specialIndeces[7] = (arg1 == Qt::Checked) ? true : false;
}

void HospitalWindow::uncheck()
{
  //Uncheck all checkboxes so next hospital can be user configured
  ui->injuryCheckbox->setChecked(Qt::Unchecked);
  ui->scanCheckbox->setChecked(Qt::Unchecked);
  ui->radiationCheckBox->setChecked(Qt::Unchecked);
  ui->cardiacCheckbox->setChecked(Qt::Unchecked);
  ui->burnCheckbox->setChecked(Qt::Unchecked);
  ui->virusCheckbox->setChecked(Qt::Unchecked);
  ui->respiratoryCheckbox->setChecked(Qt::Unchecked);
  ui->psychiatricCheckbox->setChecked(Qt::Unchecked);
  for (auto i = 0; i < specialIndeces.size();++i)
    specialIndeces[i] = false; //set back to false
}
