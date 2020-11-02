#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"

HospitalWindow::HospitalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HospitalWindow) {
    ui->setupUi(this);
    hospitalClient = new Client();
  /*  hospitals = new std::vector<std::pair<std::pair<int,bool>, Hospital>>();
    Hospital h;
    for (auto i = 0; i < 5; ++i){
        hospitals->push_back(std::make_pair(std::make_pair(i,false), h));
    }*/
    hospitalIndex = 0;
    bedCount = 0;
    hospital_name = "New River Valley Medical Center";
    location = std::make_pair(0.0,0.0); //just give default location of 0,0
    start = std::chrono::steady_clock::now();
}

HospitalWindow::~HospitalWindow() {
    delete ui;
    if (hospitalClient != nullptr)
    {
      delete hospitalClient;
      hospitalClient = nullptr;
    }
    /*if (hospitals != nullptr)
    {
        delete hospitals;
        hospitals = nullptr;
    }
    */
}

//Method will be used to generate the bed data and display on the UI chart
void HospitalWindow::generateBedData(void)
{
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


void HospitalWindow::on_addHospital_pressed()
{
    Hospital h; //Create a temp hospital before seeing if it is already in vector

    h.set_location(location.first, location.second);
    h.set_name(hospital_name);
    //Create all bed specifications for the hospital
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
    utility::string_t specialVector[] = {utility::conversions::to_string_t("injury"), utility::conversions::to_string_t("burn"),
                                            utility::conversions::to_string_t("virus")};

    for (auto s = 0; s < handleLength; ++s)
    {
       addedHandles.emplace(conditions_by_name[handleVector[s]].c);
    }
    int count = 0;
    for (auto it : addedHandles)
    {
       bedSpec[handles][count++] = JSTR(name_by_conditions[it]);
    }
    for (auto s = 0; s < 3; ++s)
    {
       addedSpecials.emplace(conditions_by_name[specialVector[s]].c);
    }
    count = 0;
    for (auto it : addedSpecials)
    {
       bedSpec[special][count++] = JSTR(name_by_conditions[it]);
    }
    bedSpec[id] = bedCount;
    ++bedCount;
    Bed b(bedSpec); //create a new bed with all parameters from bedSpec json object
    if (h.add_bed(b)) //add the bed to the hospital
    {
      hospitalClient->sendRequest("POST", h.jsonify()); //send the post request
    }
}

void HospitalWindow::on_Christiansburg_pressed()
{
    location = std::make_pair(37.089081, -80.505592);
    hospitalIndex = 0; //Christiansburg is always index 0
    hospital_name = "New River Valley Medical Center";
}

void HospitalWindow::on_Roanoke_pressed()
{
    location = std::make_pair(37.252090, -79.942436);
    hospitalIndex = 1; //Roanoke index is always 1
    hospital_name = "Carilion Roanoke Memorial Hospital";
}

void HospitalWindow::on_Lynchburg_pressed()
{
    location = std::make_pair(37.416648, -79.169753);
    hospitalIndex = 2; //Lynchburg index is always 2
    hospital_name = "Centra Lynchburg General Hospital";
}

void HospitalWindow::on_Princeton_pressed()
{
    location = std::make_pair(37.363190, -81.113136);
    hospitalIndex = 3; //Princeton index is always 3
    hospital_name = "Princeton Community Hospital";
}

void HospitalWindow::on_Bristol_pressed()
{
    location = std::make_pair(36.584577,-82.251241);
    hospitalIndex = 4; //Bristol index is always 4
    hospital_name = "Bristol Regional Medical Center";
}
