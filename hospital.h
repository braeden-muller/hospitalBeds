#ifndef HOSPITALBEDS_HOSPITAL_H
#define HOSPITALBEDS_HOSPITAL_H

#include <string>
#include <vector>
#include "bed.h"
#include "json_def.h"
#include "patient.h"

/*!
 * This represents all the data associated with a single hospital
 * Authors: Braeden Muller, Matt Lamb
 */
class Hospital {
public:
    /// Create a hospital with only default data
    Hospital() = default;
    /// Create a hospital from incoming json data
    explicit Hospital(web::json::value schema);
    /// Create a hospital from a name, position, and id (Unused)
    explicit Hospital(const std::string & name, std::pair<double, double> loc, int id);

    /// Gets the name of the hospital
    std::string get_name() const;
    /// Sets the name of the hospital
    void set_name(const std::string & name);
    /// Gets the location of the hospital
    std::pair<double, double> get_location() const;
    /// Sets the location of the hospital
    void set_location(double lat, double lon);
    /// Gets the id of the hospital
    int get_id() const;
    /// Sets the id of the hospital
    void set_id(int id);
    /// Gets the number of beds available in the hospital
    size_t get_size() const;

    /*!
     * Adds a patient to the patientQueue and also registers this hospital as an
     * attempted assignment for the patient
     */
    void add_patient(Patient &patient);
    /*!
     * Adds a patient to the patientQueue but does not register this hospital as
     * an attempted assignment
     */
    void push_patient(const Patient& patient);
    /// Removes a patient from the patientQueue
    void remove_patient(const std::string& id);

    /// Adds a new bed that may be used by this hospital
    bool add_bed(const Bed& bed);
    /*!
     * Compares this hospital to a more up-to date version of this hospital and
     * requests the database make necessary changes to reflect what changed
     * between the two versions. Also keeps track of what patients the new
     * hospital has accepted or declined between the old and new versions.
     */
    void update(const Hospital & other, std::vector<Patient>& accepted, std::vector<Patient>& declined);
    /// Has metadata for the hospital been altered recently?
    bool isMetaAltered() const;
    /// Retrieves a specific bed by its index in the internal storage
    Bed get_bed(int index) const;
    /// Attempts to find a bed by a specific id and returns its index in internal storage
    int bed_index_by_id(int id) const;
    /*!
     * Gives a score for how well this patient would fit at this hospital by
     * comparing what type of treatment the patient needs to the available beds
     * and weighing that against the distance between this hospital and the patient
     */
    double rank(const Patient & patient) const;

    /// Converts this object to json
    web::json::value jsonify();
private:
    std::string _name; // Name of the hospital
    std::pair<double, double> location; // The location of the hospital
    std::vector<Bed> beds; // The beds contained within the hospital
    std::vector<Patient> patientQueue; // The patients currently waiting on a decision from this hospital
    int _id = -1; // The id of the hospital (requires explicit instantiation on construction)

    bool metaAltered = false; // Keeps track of if the name of the hospital or location has been changed
};


#endif //HOSPITALBEDS_HOSPITAL_H
