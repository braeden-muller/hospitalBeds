//
// Created by bmuller on 10/14/20.
//

#ifndef HOSPITALBEDS_QUERY_H
#define HOSPITALBEDS_QUERY_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>

class Query {
private:
    std::map<std::string, std::string> params;
public:
    explicit Query(const std::string& q_string) {
        std::stringstream pairs(q_string);
        std::string pair;
        while (getline(pairs, pair, '&')) {
            std::stringstream spair(pair);
            std::string first, second;
            getline(spair, first, '=');
            getline(spair, second);
            params.emplace(first, second);
        }

        std::cout << "    Received: " << q_string << std::endl;
    }

    bool has_key(const std::string& key) {
        return params.find(key) != params.end();
    }

    std::string operator[](const std::string & key) {
        return params[key];
    }
};

#endif //HOSPITALBEDS_QUERY_H
