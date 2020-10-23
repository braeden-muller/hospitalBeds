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
}

Patient::Patient() {
    _id = makeUUID();
    location.first = 37.0;
    location.second = -79.0;
    _treated = false;
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
    j_patient["id"] = JSTR(_id);

    int i = 0;
    for (const auto & a : _ailments) {
        j_patient["ailments"][i++] = JSTR(name_by_conditions[a]);
    }
    j_patient["isTreated"] = JBOOL(_treated);
    j_patient["location"][0] = location.first;
    j_patient["location"][1] = location.second;

    return j_patient;
}

std::string Patient::get_id() {
    return _id;
}
