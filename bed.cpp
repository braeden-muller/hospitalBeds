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
    id = 0;
    isFull = false;
    timestamp = 0;
}

Bed::Bed(web::json::value & spec) {
    id = spec["id"].as_integer();
    isFull = spec["isFull"].as_bool();
    timestamp = spec["timestamp"].as_integer();
    addConditions(&handles, spec["handles"]);
    addConditions(&special, spec["special"]);
}

void Bed::set_id(int id) {
  id = id;
}

void Bed::set_handles(std::set<condition> handles) {
  handles = handles;
}

void Bed::set_special(std::set<condition> special) {
  special = special;
}

void Bed::set_occupied(bool occupied) {
  isFull = occupied;
}

void Bed::set_timestamp(int timestamp) {
  timestamp = timestamp;
}

web::json::value Bed::jsonify() {
    web::json::value j_bed;
    j_bed["id"] = id;
    j_bed["timestamp"] = timestamp;
    j_bed["isFull"] = JBOOL(isFull);

    int i = 0;
    for (const auto & handle : handles) {
        j_bed["handles"][i++] = JSTR(name_by_conditions[handle]);
    }

    i = 0;
    for (const auto & spec : special) {
        j_bed["special"][i++] = JSTR(name_by_conditions[spec]);
    }

    return j_bed;
}
