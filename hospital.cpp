#include "hospital.h"

void Hospital::set_name(const std::string &name) {
    _name = name;
}

void Hospital::set_location(double lat, double lon) {
    location.first = lat;
    location.second = lon;
}

void Hospital::add_bed(const Bed& bed) {
    beds.push_back(bed);
}

web::json::value Hospital::jsonify() {
    web::json::value j_hospital;
    j_hospital["hospital"] = JSTR(_name);
    j_hospital["location"][0] = location.first;
    j_hospital["location"][1] = location.second;

    for (int i = 0; i < beds.size(); i++) {
        j_hospital["beds"][i] = beds[i].jsonify();
    }

    return j_hospital;
}
