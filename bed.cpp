#include <set>
#include "bed.h"
#include "patient.h"

Bed::Bed() {
    _id = 0;
    _isFull = false;
    _timestamp = 0;
}

Bed::Bed(web::json::value & spec) {
    _id = spec["id"].as_integer();
    _isFull = spec["isFull"].as_bool();
    _timestamp = spec["timestamp"].as_integer();
    addConditions(&_handles, spec["handles"]);
    addConditions(&_special, spec["special"]);
}

void Bed::set_id(int id) {
    _id = id;
}

void Bed::set_handles(const std::set<condition> & handles) {
    _handles = handles;
}

void Bed::set_special(const std::set<condition> & special) {
    _special = special;
}

void Bed::set_full(bool isFull) {
    _isFull = isFull;
}

void Bed::set_timestamp(int timestamp) {
    _timestamp = timestamp;
}

int Bed::get_id() const {
    return _id;
}

std::string Bed::get_handles() {
    std::stringstream out;
    for (const auto & handle : _handles) {
        out<<JSTR(name_by_conditions[handle])<<", ";
    }
    std::string output = out.str();
    output.erase(std::remove(output.begin(), output.end(), '\"'), output.end());
    output.erase(output.end() - 2, output.end());
    return output;
}

std::string Bed::get_special() {
    std::stringstream out;
    for (const auto & special : _special) {
        out<<JSTR(name_by_conditions[special])<<", ";
    }
    std::string output = out.str();
    output.erase(std::remove(output.begin(), output.end(), '\"'), output.end());
    output.erase(output.end() - 2, output.end());
    return output;
}

bool Bed::get_full() {
    return _isFull;
}

int Bed::get_timestamp() {
    return _timestamp;
}

web::json::value Bed::jsonify() {
    web::json::value j_bed;
    j_bed["id"] = _id;
    j_bed["timestamp"] = _timestamp;
    j_bed["isFull"] = JBOOL(_isFull);

    int i = 0;
    for (const auto & handle : _handles) {
        j_bed["handles"][i++] = JSTR(name_by_conditions[handle]);
    }

    i = 0;
    for (const auto & spec : _special) {
        j_bed["special"][i++] = JSTR(name_by_conditions[spec]);
    }

    return j_bed;
}

bool operator<(const Bed& l, const Bed& r) {
    return l._timestamp < r._timestamp;
}

double Bed::rank(const Patient & patient) const {
    int specHits = 0;
    auto ailments = patient.get_ailments();
    for (auto & ailment : ailments) {
        if (_handles.count(ailment) == 0)
            return 0;
        if (_special.count(ailment) > 0)
            specHits++;
    }

    return 1 + (((double)specHits) / (2.0 * (double)ailments.size()));
}
