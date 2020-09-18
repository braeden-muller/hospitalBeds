#ifndef HOSPITALBEDS_DB_CONNECTION_H
#define HOSPITALBEDS_DB_CONNECTION_H

#include <string>
#include <map>
#include <vector>
#include <cpprest/http_listener.h>

/*!
 * \brief Singleton database connection abstraction
 */
class DBConnection {
private:
    static DBConnection *instance;
    std::map<std::string, std::string> hospitals; // Temporary, proof of concept. Replace with database.
    DBConnection() = default;
public:
    /*!
     * \return A pointer to the current instance of the database connection
     */
    static DBConnection* getInstance();

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
    bool registerHospital(const std::string & name, std::string & msgs);

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
};

#endif //HOSPITALBEDS_DB_CONNECTION_H
