#ifndef HOSPITALBEDS_BEDS_H
#define HOSPITALBEDS_BEDS_H

#include <vector>
#include <string>
#include "client.h"

class Beds 
{
    public:
        Beds() : occupied(false)  
        {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            timestamp = std::string(std::ctime(&now));
        }

        ~Beds();
        bool occupied; //boolean to show if the bed is occupied
        void setBed(void);
 
    private:
        std::string timestamp; //timestamp for when the bed is requested  
        std::vector<std::string> specialties; //This will contain the specialties that the bed can have
        std::vector<std::string> handles;  //This will contain ID, Current occupent ailment if occupied

};

#endif
