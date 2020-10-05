#ifndef HOSPITALBEDS_HOSPITAL_H
#define HOSPITALBEDS_HOSPITAL_H

#include "beds.h"
#include <stdio.h>

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

    bool findBed(std::string ailment); //with the ailment you can look at specificty in the bed

    std::string stringify(void); //return the hospital class as a string

    std::string listBeds(void); //list each bed in the hospital and then whether that bed is occupied

private:
    std::vector<Beds> beds;
    std::size_t totalBeds;
    std::string location;
    std::string name;
};

#endif