#ifndef HOSPITALBEDS_PATIENT_H
#define HOSPITALBEDS_PATIENT_H

#include <set>
#include <string>
#include <map>
#include "json_def.h"
#include "bed.h"


/*!
 * This represents all the features contained in an individual patient
 */
class Patient {
public:
    explicit Patient(web::json::value & spec);
    Patient();
    void set_id(int id);
    void set_ailments(const std::set<condition> & ailments);
    void set_location(const double latitude, const double longitude);
    void set_treated(const bool treated);
    web::json::value jsonify();
private:
    int _id;
    std::set<condition> _ailments;
    std::pair<double, double> location;
    bool _treated;
};

#endif //HOSPITALBEDS_PATIENT_H
