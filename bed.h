//
// Created by bmuller on 10/2/20.
//

#ifndef HOSPITALBEDS_BED_H
#define HOSPITALBEDS_BED_H

#include <set>
#include <string>
#include <map>

/*!
 * A condition represents a distinct capability a bed has
 */
enum condition {
    burn,
    injury,
    radiation,
    scan,
    unknown,
    virus
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
static auto conditions = []{ // NOLINT(cert-err58-cpp)
    // The map stores a __condition instead of a condition so if an undefined condition is specified, it is categorized
    // as unknown instead of something random
    std::map<std::string, __condition> m;
    m["burn"] = (__condition){burn};
    m["injury"] = (__condition){injury};
    m["radiation"] = (__condition){radiation};
    m["scan"] = (__condition){scan};
    m["virus"] = (__condition){virus};
    return m;
}();

/*!
 * This represents all the data associated with a single bed in a hospital
 */
class Bed {
public:
    explicit Bed(web::json::value & spec);
private:
    int id;
    bool isFull;
    int timestamp; // Remember to turn your computer off before 3:14 AM on January 19th 2038
    std::set<condition> handles;
    std::set<condition> special;
};

#endif //HOSPITALBEDS_BED_H
