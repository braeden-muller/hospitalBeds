#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"


//Method: Constructor
//Purpose: Used to initialize the main hospital window and provide for each input
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
    timer->start(60000); //time specified in ms, so poll every 1 minute

    //fill the array for special indeces
    for (auto i = 0; i < 8; ++i)
        specialIndeces.push_back(false);
}

//Destructor: Deletes all heap allocated memory
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

//Method will be used to generate the bed data and display on the UI chart
void HospitalWindow::generateBedData(const Hospital h)
{
    //Note that this code uses HW1 as a reference
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
    qDebug() << "The size of the hospital";
    qDebug() << h.get_size();
    qDebug() << "Is this correct?";
    for (auto i = 0; i < h.get_size();++i)
    {
      std::string special = h.get_bed(i).get_special(); //get the special for each bed
      qDebug() << "The special is: ";
      qDebug() << QString::fromLocal8Bit(special.c_str());
      qDebug() << special.length();
      qDebug() << "Is this ok?";
      if (special.length() > 3)
      {
        if (special.find("injur") != std::string::npos)
        {
          numForSpecial[0]++; //increase the bed count in this column
        }
        if (special.find("burn") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[1]++; //increase the bed count in this column
        }
        if (special.find("virus") != std::string::npos)
        {
          numForSpecial[2]++;
        }
        if (special.find("radiat") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[3]++; //increase the bed count in this column
        }
        if (special.find("psych") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[4]++; //increase the bed count in this column
        }
        if (special.find("respirat") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[5]++; //increase the bed count in this column
        }
        if (special.find("cardi") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[6]++; //increase the bed count in this column
        }
        if (special.find("scan") != std::string::npos) //bed can have multiple specials
        {
          numForSpecial[7]++; //increase the bed count in this column
        }
      }
    }
    for (auto i = 0; i < numForSpecial.size(); ++i)
    {
        set->append(numForSpecial[i]);
    }
    series->append(set); //append the set to a series
    chart->addSeries(series); //add series to chart
    chart->setTitleFont(*qFont);
    chart->setTitle("Hospital Bed Data");

    QBarCategoryAxis *xAxis = new QBarCategoryAxis(); //create x axis
    QStringList specialsList = (QStringList() << "injury" << "burn" << "radiation" << "virus"
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

//This method makes a POST request on a polling basis to get the current status
//of hospitals for plotting purposes
void HospitalWindow::getStatus()
{
  try
  {
    for (auto i= 0 ; i < hospitals_in_use->size();++i)
    {
      Hospital h = hospitals_in_use->at(i);
      //h.set_name(hospitals_in_use->at(i).get_name());
      //h.set_location(hospitals_in_use->at(i).get_location().first, hospitals_in_use->at(i).get_location().second);
      auto rec = hospitalClient->sendRequest("POST", h.jsonify());
      std::cout << "IN HOSPTIAL CLIENT:  " << rec.serialize() << std::endl;
      Hospital recHospital(rec);
      hospitals_in_use->at(i) = recHospital;
    }
  }
  catch(std::exception& e) //If unable to connect to server
  {
      std::cout << e.what() << std::endl;
      QMessageBox::information(this,tr("Error"), tr("Please start the server."));
  }

}

//Slot to call after the addHospital button is pressed
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
  for (auto i = 0; i < specialIndeces.size(); ++i)
  {
      specialIndeces[i] = false;
  }

  uncheck();
}

//Slot for if christainsburg is Checked
void HospitalWindow::on_Christiansburg_pressed()
{
    location = std::make_pair(37.089081, -80.505592);
    hospital_name = "New River Valley Medical Center";
}

//Slot for if Roanoke is pressed
void HospitalWindow::on_Roanoke_pressed()
{
    location = std::make_pair(37.252090, -79.942436);
    hospital_name = "Carilion Roanoke Memorial Hospital";
}

//Slot for if Lynchburg is pressed
void HospitalWindow::on_Lynchburg_pressed()
{
    location = std::make_pair(37.416648, -79.169753);
    hospital_name = "Centra Lynchburg General Hospital";
}

//Slot for if Princeton is pressed
void HospitalWindow::on_Princeton_pressed()
{
    location = std::make_pair(37.363190, -81.113136);
    hospital_name = "Princeton Community Hospital";
}

//Slot for if Bristol is pressed
void HospitalWindow::on_Bristol_pressed()
{
    location = std::make_pair(36.584577,-82.251241);
    hospital_name = "Bristol Regional Medical Center";
}

//signals to delete a hospital from the database
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
        hospital_exists = true;
        break;
      }
    }
    if (hospital_exists)
    {
      hospitalClient->sendRequest("DELETE", h.jsonify()); //send the delete request for the hospital
      if (index_to_erase != -1)
      {
        hospitals_in_use->erase(hospitals_in_use->begin()+index_to_erase);
      }
    }
    else
    {
      QMessageBox::information(this,tr("Error"), tr("No hospitals are in use yet, please select hospital"));
    }

}

//Callback for if the bedSlider is changed
void HospitalWindow::on_bedSlider_valueChanged(int value)
{
    beds2Add = value;
    ui->beds_count_edit->clear();
    ui->beds_count_edit->setText(QString::number(value));
}

//Adds beds to a current hospital
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
      qDebug() << "The hospital is available";
      qDebug() << beds2Add;
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
                  qDebug() << "This special was emplaced";
                  qDebug() << s;
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
      for (auto i = 0; i < h.get_size(); ++i) {
        std::cout << h.get_bed(i).get_special() << '\t' << "SPECIAL" << std::endl;
      }
      hospitalClient->sendRequest("POST", h.jsonify()); //send the post request
    }
    generateBedData(h); //gemerate the bed data from the hospital
}


//The next set of functions are all call backs for each of the checkboxes for when a special value is Checked
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
}
