# hospitalBeds

hospitalBeds is an executable with 3 different operating modes. These modes are as follows:

  - doctor: Displays the doctor window which allows for the user to request a hospital bed for a patient in the
            following locations: Christiansburg, Roanoke, Lynchburg, Princeton, Bristol. The user is required to 
            select at least one ailment for their patient. These ailments are given in a checkbox panel and multiple
            ailments may be selected. The doctor sends POST request to the server which will then register and store
            individual hospitals in the database. To run this ui type hospitalBeds -d. 
            
  - server: This establishes the connection from the doctor to the database as well as the hospital to the database
            and listens for POST requests sent from either the doctor or hospital. It is configured at port 8080 of \
            localhost. To run this executable type hospitalBeds -s.
   
 - hospital: Displays the hospital window which allows for the hospital to request bed data and populate charts for 
             each hospital that display the number of occupied beds for each specific hospital. To run this 
             executable type hospitalBeds -h at the command line. 
             
# required Libraries
  
  - SQLite3 
    Installation: Linux: sudo apt-get install libsqlite3-dev
  - Qt5 - QtWidget and QtCharts: 
    Installation: Linux - Install QtCreator community with widgets and charts
  - casablanca for REST SDK 
    Installation: Linux - https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux
  - lcurses
    Installation: Linux - sudo apt-get install libncurses5-dev libncursesw5-dev
    
# Other Details
  
  Project built using CMake, and programming languages used are C++ and C. REST service is used for communication and 
  SQLite is used for database.
  

