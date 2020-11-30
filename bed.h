#ifndef HOSPITALBEDS_BED_H
#define HOSPITALBEDS_BED_H

#include <set>
#include <string>
#include <map>
#include "json_def.h"
#include "conditions.h"
#include "patient.h"

class Patient;

/*!
 * This represents all the data associated with a single bed in a hospital
 * Authors: Braeden Muller, Matt Lamb
 */
class Bed {
public:
    /// Creates a Bed from a JSON specification
    explicit Bed(web::json::value & spec);
    /// Creates a Bed with no internal data
    Bed();

    /// Sets the bed's database id
    void set_id(int id);
    /// Sets what types of conditions this bed can handle
    void set_handles(const std::set<condition> & handles);
    /// Sets what types of conditions this bed specializes in
    void set_special(const std::set<condition> & special);
    /// Sets whether a bed is occupied by a patient or not
    void set_full(bool isFull);
    /// Sets the timestamp of when the bed was last updated
    void set_timestamp(int timestamp);

    /// Gets the bed's database id
    int get_id() const;
    /// Gets what conditions this bed is equipped to handle
    std::string get_handles();
    /// Gets what conditions this bed specializes in handling
    std::string get_special();
    /// Gets whether the bed is occupied by a patient or not
    bool get_full();
    /// Gets the timestamp of when the bed was last modified
    int get_timestamp();

    /// Returns a value 0-1.5 based on how good of a fit this bed would be for a Patient
    double rank(const Patient & patient) const;
    /// Converts this object to json
    web::json::value jsonify();

    friend bool operator<(const Bed& l, const Bed& r);
private:
    int _id;
    bool _isFull;
    int _timestamp; // Remember to turn your computer off before 3:14 AM on January 19th 2038
    std::set<condition> _handles;
    std::set<condition> _special;
};

#endif //HOSPITALBEDS_BED_H
