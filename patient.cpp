#include "patient.h"

Patient::Patient() {
    _id = "-1";
    location.first = 37.0;
    location.second = -79.0;
    _treated = false;
}

void Patient::set_id(std::string id) {
    _id = id;
}

void Patient::set_ailments(const std::set<condition> & ailments) {
    _ailments = ailments;
}

void Patient::set_location(const double latitude, const double longitude) {
    location.first = latitude;
    location.second = longitude;
}

void Patient::set_treated(const bool treated){
  _treated = treated;
}

std::pair<double,double> Patient::get_location(void) const{
  return location;
}

bool Patient::is_treated(void) const{
  return _treated;
}

std::set<condition> Patient::get_ailments(void) const{
  return _ailments;
}

std::string Patient::get_id(void) const{
  return _id;
}

web::json::value Patient::jsonify() {
    web::json::value j_patient;
    web::json::value j_data;
    j_data["id"] = JSTR(_id);

    int i = 0;
    for (const auto & a : _ailments) {
        j_data["ailments"][i++] = JSTR(name_by_conditions[a]);
    }
    j_data["isTreated"] = JBOOL(_treated);
    j_data["location"][0] = location.first;
    j_data["location"][1] = location.second;
    j_patient["patient"] = j_data;

    return j_patient;
}
