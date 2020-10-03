#ifndef HOSPITALBEDS_HOSPITAL_H
#define HOSPITALBEDS_HOSPITAL_H

#include "beds.h"

class Hospital 
{
public:
    Hospital() : location("2900 Lamb Cir, Christiansburg, VA 24073"), name("Carilion New River Valley Medical Center"),
                 totalBeds(110)
    {
        Beds b;
        for (auto i = 0; i < totalBeds; ++i)
            beds.push_back(b);
    }

    Hospital(std::string newLoc);
    ~Hospital();

    std::vector<Beds> beds;
    std::size_t totalBeds;
    std::string location;
    std::string name;
};

#endif