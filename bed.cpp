#include <set>
#include "bed.h"

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
