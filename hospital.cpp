#include "hospital.h"

void Hospital::set_name(const std::string &name) {
    _name = name;
    set_max_beds();
}

void Hospital::set_location(double lat, double lon) {
    location.first = lat;
    location.second = lon;
}

void Hospital::set_id(int id) {
  _id = id;
}

bool Hospital::add_bed(const Bed& bed) {
    bool ok = beds.size() < max_beds;
    if (ok)
      beds.push_back(bed);
    return ok;
}

//max beds were researched and found from each hospital website
//(links to source are included in ReadMe.md)
void Hospital::set_max_beds(void)
{
  max_beds = (_name.find("New River") != std::string::npos) ? 110 :
             (_name.find("Roanoke") != std::string::npos) ? 703 :
             (_name.find("Princeton") != std::string::npos) ? 267 :
             (_name.find("Lynchburg") != std::string::npos) ? 358 :
             (_name.find("Bristol") != std::string::npos) ? 269 : 0;
}

web::json::value Hospital::jsonify() {
    web::json::value j_hospital;
    j_hospital["hospital"] = JSTR(_name);
    j_hospital["location"][0] = location.first;
    j_hospital["location"][1] = location.second;
    j_hospital["id"] = _id;

    for (int i = 0; i < beds.size(); i++) {
        j_hospital["beds"][i] = beds[i].jsonify();
    }

    return j_hospital;
}
