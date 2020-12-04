#ifndef HOSPITALBEDS_PATIENT_H
#define HOSPITALBEDS_PATIENT_H

#include <set>
#include <string>
#include <map>
#include "json_def.h"
#include "bed.h"


/*!
 * This represents all the features contained in an individual patient
 * Authors: Braeden Muller, Matt Lamb
 */
class Patient {
public:
    /// Creates a patient from an incoming json specification
    explicit Patient(web::json::value & spec);
    /// Creates a patient with default data
    Patient();

    /// Gets the patient's UUID
    std::string get_id();
    /// Sets what conditions the patient has
    void set_ailments(const std::set<condition> & ailments);
    /// Sets the patient's location
    void set_location(double latitude, double longitude);
    /// Sets whether the patient has been treated
    void set_treated(bool treated);
    /// Sets the hospital that the patient is assigned to
    void set_assigned_hospital(const std::string& name);

    /// Gets the location that patient is at
    std::pair<double,double> get_location() const;
    /// Gets whether the patient has been assigned to a bed
    bool is_treated() const;
    /// Gets the list of conditions that the patient has
    std::set<condition> get_ailments() const;
    /// Gets the patient's UUID
    std::string get_id() const;
    /// Gets the hospital that this patient is assigned to
    std::string get_assigned_hospital() const;
    /*!
     * Notes that a patient has attempted to be assigned to a hospital so it may
     * not be routed to the same hospital more than once
     */
    void add_attempt(const std::string & hospital);
    /*!
     * Checks if this patient has already attempted to be assigned to a
     * particular hospital
     */
    bool has_attempted(const std::string & hospital) const;
    /// Converts this hospital to json
    web::json::value jsonify();
private:
    std::string _id; // uuid
    std::set<condition> _ailments; // List of conditions that this partient has
    std::pair<double, double> location; // Latitude, longitude of patient
    std::string _assignedHospital; // The hospital that this patient is assigned to
    std::set<std::string> _attempts; // List of hospitals that this patient has attempted to be assigned to
    bool _treated; // Whether this patient has been assigned a bed (ie. no longer needs to be routed)
};

#endif //HOSPITALBEDS_PATIENT_H
