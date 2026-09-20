#ifndef MENU_H
#define MENU_H

#include "Airline.h"
#include <string>

class Menu {
private:
    Airline airline;

    // Helper functions for user input validation
    static int getIntInput(const std::string& prompt);
    static double getDoubleInput(const std::string& prompt);
    static std::string getStringInput(const std::string& prompt);
    static bool getBoolInput(const std::string& prompt);

    // Submenu handlers
    void handleFlightManagement();
    void handlePassengerManagement();
    void handleBookingOperations();
    void handleCancellation();
    void handleReports();

    // Specific action handlers
    void addFlightForm();
    void registerPassengerForm();
    void searchFlightsForm();

public:
    Menu();
    void run();
};

#endif // MENU_H
