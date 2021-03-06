#ifndef HOSPITALBEDS_DB_CONNECTION_H
#define HOSPITALBEDS_DB_CONNECTION_H

#include <string>
#include <map>
#include <vector>
#include <cpprest/http_listener.h>
#include <sstream>

#include "sqlite3.h"
#include "hospital.h"

/*!
 * \brief Singleton database connection abstraction
 * Authors: Matt Lamb
 */
class DBConnection {
private:
    static DBConnection *instance;
    sqlite3 * db;
    std::string testing; // temporary testing string to ensure db receive.
    std::vector<std::tuple<std::size_t, Hospital,bool>> hospitals;
public:
    DBConnection();
    ~DBConnection();
    /*!
     * \return A pointer to the current instance of the database connection
     */
    static DBConnection* getInstance();

    /*!
     * \return A vector containing all hospital information currently in the database
     */
    std::vector<std::tuple<std::size_t, Hospital,bool>>* getHospitals();

    /*!
     * Frees the memory related to the database connection
     */
    static void shutdown();

    /*!
     * \brief Registers a new hospital to be remembered by the system
     * \param name The name of the hospital
     * \param msgs Messages to be returned (e.g. Specific error or success message)
     * \return true if successful, false otherwise
     */
    bool registerHospital(Hospital h, std::string & msgs);

    /*!
     * \brief Removes the registration for an existing hospital currently remembered by the system
     * \param name The name of the hospital
     * \param msgs Messages to be returned (e.g. Specific error or success message)
     * \return true if successful, false otherwise
     */
    bool unregisterHospital(const std::string & name, std::string & msgs);

    /*!
     * \brief Retrieves hospital data for currently tracked hospitals
     * \param queries Filters to apply to get a specific hospital
     * \param results All hospitals matching filters
     * \param msgs Messages to be returned (e.g. Specific error or success message)
     * \return true if successful, false otherwise
     */
    bool getHospitalData(const std::vector<std::string> & queries, std::vector<web::json::value> & results, std::string & msgs);

    /*!
     * \brief Imports the database
     * \param msgs Messages to be returned (e.g. Specific error or success message)
     * \return true if successful, false otherwise
     */
    bool importDatabase(std::string & msgs);

    /*!
     * \brief inserts the database contents into the class structure
     * \param argc argument count
     * \param argv argument char * strings
     * \param columnName the column being iterated
     * \return true if successful, false otherwise
     */
    int databaseCallback(int argc, char **argv, char **columnName);


      /*!
     * \brief Updates the database
     * \param index, the index to update
     * \param msgs Messages to be returned (e.g. Specific error or success message)
     * \return true if successful, false otherwise
     */
  bool updateDatabase(Hospital h, std::string & msgs);
};

    /*!
     * \brief takes input database and casts it to the internal class
     * \param data the database input
     * \param argc argument count
     * \param argv argument char * strings
     * \param columnName the column being iterated
     * \return true if successful, false otherwise
     */
    static int C_Callback(void *data, int argc, char **argv, char **columns);
#endif //HOSPITALBEDS_DB_CONNECTION_H
