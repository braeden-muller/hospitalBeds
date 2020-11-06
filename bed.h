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
 */
class Bed {
public:
    explicit Bed(web::json::value & spec);
    Bed();
    void set_id(int id);
    void set_handles(const std::set<condition> & handles);
    void set_special(const std::set<condition> & special);
    void set_full(bool isFull);
    void set_timestamp(int timestamp);

    int get_id() const;
    std::string get_handles();
    std::string get_special();
    bool get_full();
    int get_timestamp();
  
    /// Converts this object to json
    double rank(const Patient & patient) const;
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
