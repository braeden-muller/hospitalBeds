#include <uuid/uuid.h>
#include "patient.h"
#include "conditions.h"

std::string makeUUID() {
    uuid_t id;
    uuid_generate(id);
    std::string __id(reinterpret_cast<const char *>(id));
    return __id;
}

Patient::Patient(web::json::value & spec) {
    _id = spec["id"].as_string();
    addConditions(&_ailments, spec["ailments"]);
    location.first = spec["location"][0].as_double();
    location.second = spec["location"][1].as_double();
    _treated = spec["isTreated"].as_bool();
    if (spec.has_field("assigned"))
        _assignedHospital = spec["assigned"].as_string();
}

Patient::Patient() {
    _id = makeUUID();
    location.first = 37.0;
    location.second = -79.0;
    _treated = false;
    _assignedHospital = "NONE"; //set to no assigned hospital upon startup
}

void Patient::set_ailments(const std::set<condition> & ailments) {
    _ailments = ailments;
}

void Patient::set_location(const double latitude, const double longitude) {
    location.first = latitude;
    location.second = longitude;
}

void Patient::set_treated(const bool treated) {
    _treated = treated;
}

void Patient::set_assigned_hospital(const std::string& name) {
    _assignedHospital = name;
}

std::pair<double,double> Patient::get_location() const {
    return location;
}

bool Patient::is_treated() const {
    return _treated;
}

std::set<condition> Patient::get_ailments() const {
    return _ailments;
}

std::string Patient::get_id() const {
    return _id;
}

std::string Patient::get_assigned_hospital() const {
    return _assignedHospital;
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
    j_data["assigned"] = JSTR(_assignedHospital);
    j_patient["patient"] = j_data;

    return j_patient;
}

std::string Patient::get_id() {
    return _id;
}

void Patient::add_attempt(const std::string & hospital) {
    _attempts.insert(hospital);
}

bool Patient::has_attempted(const std::string &hospital) const {
    return _attempts.count(hospital) > 0;
}
