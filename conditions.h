//
// Created by bmuller on 10/23/20.
//

#ifndef HOSPITALBEDS_CONDITIONS_H
#define HOSPITALBEDS_CONDITIONS_H

/*!
 * A condition represents a distinct capability a bed has
 */
enum condition {
    burn,
    injury,
    radiation,
    scan,
    virus,
    cardiac,
    respiratory,
    unknown
};

/*!
 * This is a condition wrapper that defaults to unknown
 */
struct __condition {
    condition c = unknown;
    __condition() = default;
    explicit __condition(condition cd) { c = cd; };
};

/*!
 * This is a map that allows a string to be used to quickly reference a condition
 */
static auto conditions_by_name = []{ // NOLINT(cert-err58-cpp)
    // The map stores a __condition instead of a condition so if an undefined condition is specified, it is categorized
    // as unknown instead of something random
    std::map<std::string, __condition> m;
    m["burn"] = (__condition){burn};
    m["injury"] = (__condition){injury};
    m["radiation"] = (__condition){radiation};
    m["scan"] = (__condition){scan};
    m["virus"] = (__condition){virus};
    m["cardiac"] = (__condition){cardiac};
    m["respiratory"] = (__condition){respiratory};
    return m;
}();

/*!
 * Reverse of other map
 */
static auto name_by_conditions = []{ // NOLINT(cert-err58-cpp)
    std::map<condition, std::string> m;
    for (const auto& c : conditions_by_name) {
        m[c.second.c] = c.first;
    }
    return m;
}();

static void addConditions(std::set<condition> * receptacle, const web::json::value & j_conditions) {
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

#endif //HOSPITALBEDS_CONDITIONS_H
