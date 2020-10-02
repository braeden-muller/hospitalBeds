//
// Created by bmuller on 10/2/20.
//

#include <iostream>
#include <cpprest/json.h>
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
        receptacle->emplace(conditions[j_condition.as_string()].c);
    }
}

// This is for debug, will be removed in a future version. Don't bother updating it.
std::string set_to_str(const std::set<condition> & my_set) {
    std::string out_val = "[";
    for (condition entry : my_set) {
        switch (entry) {
            case burn:
                out_val += "burn,";
                break;
            case injury:
                out_val += "injury,";
                break;
            case radiation:
                out_val += "radiation,";
                break;
            case scan:
                out_val += "scan,";
                break;
            case unknown:
                out_val += "unknown,";
                break;
            case virus:
                out_val += "virus,";
                break;
        }
    }

    if (out_val.size() > 1)
        out_val.pop_back();

    return out_val + "]";
}

Bed::Bed(web::json::value & spec) {
    id = spec["id"].as_integer();
    isFull = spec["isFull"].as_bool();
    timestamp = spec["timestamp"].as_integer();
    addConditions(&handles, spec["handles"]);
    addConditions(&special, spec["special"]);

    // DEBUG
    std::cout << "bed_" << id << " {" << std::endl
        << "    timestamp : " << timestamp << std::endl
        << "    isFull : " << (isFull ? "true" : "false") << std::endl
        << "    handles : " << set_to_str(handles) << std::endl
        << "    special : " << set_to_str(special) << std::endl
        << "}" << std::endl;
}
