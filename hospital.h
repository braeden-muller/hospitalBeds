#ifndef HOSPITALBEDS_HOSPITAL_H
#define HOSPITALBEDS_HOSPITAL_H

#include <string>
#include <vector>
#include "bed.h"
#include "json_def.h"

/*!
 * This represents all the data associated with a single hospital
 */
class Hospital {
public:
    Hospital() = default;
    void set_name(const std::string & name);
    void set_location(double lat, double lon);
    void set_id(int id);
    bool add_bed(const Bed& bed);

    std::string get_name();
    Bed get_bed(int index);
    size_t get_size();
    /// Converts this object to json
    web::json::value jsonify();
private:
    std::string _name;
    std::pair<double, double> location;
    std::vector<Bed> beds;
    std::size_t max_beds;
    int _id;

    void set_max_beds(); //helper function used to set max beds
};


#endif //HOSPITALBEDS_HOSPITAL_H
