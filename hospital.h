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
    void add_bed(const Bed& bed);
    /// Converts this object to json
    web::json::value jsonify();
private:
    std::string _name;
    std::pair<double, double> location;
    std::vector<Bed> beds;
};


#endif //HOSPITALBEDS_HOSPITAL_H
