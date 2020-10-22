#include "patient.h"

Patient::Patient() {
    _id = 0;
    location.first = 37.0;
    location.second = -79.0;
    _treated = false;
}

void Patient::set_id(int id) {
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

web::json::value Patient::jsonify() {
    web::json::value j_patient;
    j_patient["id"] = _id;

    int i = 0;
    for (const auto & a : _ailments) {
        j_patient["ailments"][i++] = JSTR(name_by_conditions[a]);
    }
    j_patient["isTreated"] = JBOOL(_treated);
    j_patient["location"][0] = location.first;
    j_patient["location"][1] = location.second;

    return j_patient;
}
