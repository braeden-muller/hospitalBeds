#include "hospitalwindow.h"
#include "doctorwindow.h"
#include "server.h"

#include <QApplication>
#include <QString>
#include <iostream>
#include <cstdlib>

/*!
 * \brief main Entry point of program. Has three operating modes: doctor (-d), hospital (-h), and server (-s)
 * \param argc Argument count
 * \param argv Arguments. Please specify operating mode.
 * \return Exit status
 * Authors: Braeden Muller
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Check for operating mode specification
    // -h : Hospital Mode
    // -d : Doctor Mode
    // -s : Server Mode
    if (argc > 1) {
        QString arg = argv[1];
        QMainWindow* w = nullptr;

        if (arg == "-h")
            w = new HospitalWindow();
        else if (arg == "-d")
            w = new DoctorWindow();
        else if (arg == "-s") {
            Server s("http://localhost:8080/hospital");
            return 0;
        }

        // Show a window if one was made
        if (w)
            w->show();
    }
    else {
        std::cerr << "No operating mode specified! Use -h -d or -s!" << std::endl;
        return EXIT_FAILURE;
    }

    return QApplication::exec();
}
