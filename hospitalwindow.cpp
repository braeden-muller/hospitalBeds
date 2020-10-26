#include "hospitalwindow.h"
#include "./ui_hospitalwindow.h"

HospitalWindow::HospitalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HospitalWindow) {
    ui->setupUi(this);
    hospitalClient = new Client();
    bedCount = 0;
    start = std::chrono::steady_clock::now();
}

HospitalWindow::~HospitalWindow() {
    delete ui;
    if (hospitalClient != nullptr)
    {
      delete hospitalClient;
      hospitalClient = nullptr;
    }
}

//Method will be used to generate the bed data and display on the UI chart
void HospitalWindow::generateBedData(void)
{
    //auto db = DBConnection::getInstance();

    //auto hospitals = db->getHospitals();
    std::cout << "I got some of the patient data" << std::endl
              << "The hospital size is: " << DBConnection::getInstance()->getHospitals()->size() << std::endl;

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
