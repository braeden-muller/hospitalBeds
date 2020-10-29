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
    std::string get_id();
    void set_ailments(const std::set<condition> & ailments);
    void set_location(double latitude, double longitude);
    void set_treated(bool treated);
    void set_assigned_hospital(const std::string& name);

    std::pair<double,double> get_location() const;
    bool is_treated() const;
    std::set<condition> get_ailments() const;
    std::string get_id() const;
    std::string get_assigned_hospital() const;
    web::json::value jsonify();
private:
    std::string _id;
    std::set<condition> _ailments;
    std::pair<double, double> location;
    std::string _assignedHospital;
    bool _treated;
};

#endif //HOSPITALBEDS_PATIENT_H
