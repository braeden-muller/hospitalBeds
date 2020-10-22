#include <set>
#include "bed.h"

void addConditions(std::set<condition> * receptacle, const web::json::value & j_conditions) {
    // We know the incoming j_conditions is an array of strings, but right now it is just a json::value
    // Cast the incoming value as an array of json values
    auto & j_conditions_arr = j_conditions.as_array();

    // Iterate through every condition and add it to the set being built
    for (auto & j_condition : j_conditions_arr) {
        // Reference the condition in the map of conditions using the string that represents the condition
        // Since this operation returns a map of condition-wrappers, use .c to get the condition
        // Finally add it to the set
        receptacle->emplace(conditions_by_name[j_condition.as_string()].c);
    }
}

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