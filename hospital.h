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
    explicit Hospital(const std::string & name, std::pair<double, double> loc, int id);

    std::string get_name() const;
    void set_name(const std::string & name);
    std::pair<double, double> get_location() const;
    void set_location(double lat, double lon);
    int get_id() const;
    void set_id(int id);
    size_t get_size() const;

    bool add_bed(const Bed& bed);
    void update(const Hospital & other);
    bool isMetaAltered() const;

    Bed get_bed(int index) const;
    int bed_index_by_id(int id) const;

    /// Converts this object to json
    web::json::value jsonify();
private:
    std::string _name;
    std::pair<double, double> location;
    std::vector<Bed> beds;
    int _id = -1;

    bool metaAltered = false;
};


#endif //HOSPITALBEDS_HOSPITAL_H
