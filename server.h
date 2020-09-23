#ifndef HOSPITALBEDS_SERVER_H
#define HOSPITALBEDS_SERVER_H

#include <string>
#include "db_connection.h"

/*!
 * The server manages outside connections and the internal database
 */
class Server {
public:
    /*!
     * Creates a new server at the specified address. Can only respond to GETs and POSTs
     * \param addr The URL at which to listen on
     */
    explicit Server(const std::string & addr);
    ~Server();
private:
    std::string address;
};


#endif //HOSPITALBEDS_SERVER_H