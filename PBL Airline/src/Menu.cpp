#include "../include/Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>

Menu::Menu() : airline("SkyLink Airways") {
    // Attempt to load existing data from data/ directory
    if (!airline.loadData("data/")) {
        std::cout << "[System Info] No existing state file found. Pre-populating sample dataset..." << std::endl;
        airline.loadSampleData();
        airline.saveData("data/");
    } else {
        std::cout << "[System Info] Previous system state successfully restored from file." << std::endl;
    }
}

int Menu::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "[Invalid Input] Please enter a valid number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double Menu::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "[Invalid Input] Please enter a valid decimal number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string Menu::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    while (value.empty()) {
        std::cout << "[Input Cannot Be Empty] " << prompt;
        std::getline(std::cin, value);
    }
    return value;
}

bool Menu::getBoolInput(const std::string& prompt) {
    std::string choice = getStringInput(prompt + " (y/n): ");
    return (choice == "y" || choice == "Y" || choice == "yes" || choice == "YES" || choice == "1");
}

void Menu::run() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n==========================================================" << std::endl;
        std::cout << "         SKYLINK AIRWAYS - MANAGEMENT SYSTEM              " << std::endl;
        std::cout << "==========================================================" << std::endl;
        std::cout << "  1. Flight Management" << std::endl;
        std::cout << "  2. Passenger Management" << std::endl;
        std::cout << "  3. Ticket Booking" << std::endl;
        std::cout << "  4. Cancellation & Refund Operations" << std::endl;
        std::cout << "  5. Reports & Analytics" << std::endl;
        std::cout << "  6. Save Data to File" << std::endl;
        std::cout << "  0. Exit System" << std::endl;
        std::cout << "==========================================================" << std::endl;

        choice = getIntInput("Select an option (0-6): ");

        switch (choice) {
            case 1: handleFlightManagement(); break;
            case 2: handlePassengerManagement(); break;
            case 3: handleBookingOperations(); break;
            case 4: handleCancellation(); break;
            case 5: handleReports(); break;
            case 6:
                if (airline.saveData("data/")) {
                    std::cout << "[Success] Data saved to data/ folder successfully." << std::endl;
                } else {
                    std::cout << "[Error] Failed to save data." << std::endl;
                }
                break;
            case 0:
                std::cout << "\nSaving system state before exiting..." << std::endl;
                airline.saveData("data/");
                std::cout << "Thank you for using SkyLink Airways System. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "[Invalid Choice] Please select between 0 and 6." << std::endl;
        }
    }
}

// -------------------------------------------------------------
// Submenu Handlers
// -------------------------------------------------------------
void Menu::handleFlightManagement() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- FLIGHT MANAGEMENT MENU ---" << std::endl;
        std::cout << "1. List All Flights" << std::endl;
        std::cout << "2. Search Flights by Route" << std::endl;
        std::cout << "3. Search Flights by Date" << std::endl;
        std::cout << "4. Add New Flight" << std::endl;
        std::cout << "5. Remove Flight" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        choice = getIntInput("Option: ");
        switch (choice) {
            case 1:
                airline.listAllFlights();
                break;
            case 2: {
                std::string orig = getStringInput("Enter Origin city: ");
                std::string dest = getStringInput("Enter Destination city: ");
                auto results = airline.searchFlightsByRoute(orig, dest);
                std::cout << "\nFound " << results.size() << " matching flights:" << std::endl;
                for (const auto& f : results) {
                    std::cout << *f << std::endl;
                }
                break;
            }
            case 3: {
                std::string date = getStringInput("Enter Date (YYYY-MM-DD): ");
                auto results = airline.searchFlightsByDate(date);
                std::cout << "\nFound " << results.size() << " matching flights:" << std::endl;
                for (const auto& f : results) {
                    std::cout << *f << std::endl;
                }
                break;
            }
            case 4:
                addFlightForm();
                break;
            case 5: {
                std::string fNum = getStringInput("Enter Flight Number to remove: ");
                if (airline.removeFlight(fNum)) {
                    std::cout << "[Success] Flight " << fNum << " removed." << std::endl;
                } else {
                    std::cout << "[Error] Flight " << fNum << " not found." << std::endl;
                }
                break;
            }
            case 0:
                break;
            default:
                std::cout << "[Invalid Choice]" << std::endl;
        }
    }
}

void Menu::addFlightForm() {
    std::cout << "\n--- ADD NEW FLIGHT ---" << std::endl;
    std::cout << "Select Flight Type:" << std::endl;
    std::cout << "1. Domestic Flight" << std::endl;
    std::cout << "2. International Flight" << std::endl;
    std::cout << "3. Charter Flight" << std::endl;
    int typeChoice = getIntInput("Type (1-3): ");

    std::string fNum = getStringInput("Flight Number (e.g. SL105): ");
    std::string orig = getStringInput("Origin: ");
    std::string dest = getStringInput("Destination: ");
    std::string date = getStringInput("Departure Date (YYYY-MM-DD): ");
    std::string time = getStringInput("Departure Time (HH:MM): ");
    int seats = getIntInput("Total Seats: ");
    double fare = getDoubleInput("Base Fare ($): ");

    std::shared_ptr<Flight> newFlight = nullptr;

    if (typeChoice == 1) {
        double tax = getDoubleInput("State Tax Rate (e.g. 0.05 for 5%): ");
        newFlight = std::make_shared<DomesticFlight>(fNum, orig, dest, date, time, seats, fare, tax);
    } else if (typeChoice == 2) {
        bool visa = getBoolInput("Visa Required?");
        double intlTax = getDoubleInput("International Tax ($): ");
        newFlight = std::make_shared<InternationalFlight>(fNum, orig, dest, date, time, seats, fare, visa, intlTax);
    } else if (typeChoice == 3) {
        std::string holder = getStringInput("Contract Holder Name: ");
        double charterFee = getDoubleInput("Flat Charter Fee ($): ");
        newFlight = std::make_shared<CharterFlight>(fNum, orig, dest, date, time, seats, fare, holder, charterFee);
    } else {
        std::cout << "[Error] Invalid flight type selected." << std::endl;
        return;
    }

    if (newFlight) {
        airline.addFlight(newFlight);
    }
}

void Menu::handlePassengerManagement() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- PASSENGER MANAGEMENT MENU ---" << std::endl;
        std::cout << "1. List All Passengers" << std::endl;
        std::cout << "2. Register New Passenger" << std::endl;
        std::cout << "3. View Passenger Booking History" << std::endl;
        std::cout << "4. Remove Passenger" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        choice = getIntInput("Option: ");
        switch (choice) {
            case 1:
                airline.listAllPassengers();
                break;
            case 2:
                registerPassengerForm();
                break;
            case 3: {
                std::string pId = getStringInput("Enter Passenger ID: ");
                try {
                    airline.viewPassengerHistory(pId);
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                }
                break;
            }
            case 4: {
                std::string pId = getStringInput("Enter Passenger ID to remove: ");
                if (airline.removePassenger(pId)) {
                    std::cout << "[Success] Passenger " << pId << " removed." << std::endl;
                } else {
                    std::cout << "[Error] Passenger " << pId << " not found." << std::endl;
                }
                break;
            }
            case 0:
                break;
            default:
                std::cout << "[Invalid Choice]" << std::endl;
        }
    }
}

void Menu::registerPassengerForm() {
    std::cout << "\n--- REGISTER NEW PASSENGER ---" << std::endl;
    std::cout << "Select Passenger Category:" << std::endl;
    std::cout << "1. Economy Class" << std::endl;
    std::cout << "2. Business Class" << std::endl;
    std::cout << "3. First Class" << std::endl;
    int typeChoice = getIntInput("Category (1-3): ");

    std::string id = getStringInput("Passenger ID (e.g. P009): ");
    std::string name = getStringInput("Full Name: ");
    std::string email = getStringInput("Email Address: ");
    std::string phone = getStringInput("Phone Number: ");

    std::shared_ptr<Passenger> newPassenger = nullptr;

    if (typeChoice == 1) {
        newPassenger = std::make_shared<EconomyPassenger>(id, name, email, phone);
    } else if (typeChoice == 2) {
        newPassenger = std::make_shared<BusinessPassenger>(id, name, email, phone);
    } else if (typeChoice == 3) {
        newPassenger = std::make_shared<FirstClassPassenger>(id, name, email, phone);
    } else {
        std::cout << "[Error] Invalid category selected." << std::endl;
        return;
    }

    if (newPassenger) {
        airline.registerPassenger(newPassenger);
    }
}

void Menu::handleBookingOperations() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- BOOKING OPERATIONS ---" << std::endl;
        std::cout << "1. Book a Ticket" << std::endl;
        std::cout << "2. View All Tickets" << std::endl;
        std::cout << "3. Search Ticket by Ticket ID" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        choice = getIntInput("Option: ");
        switch (choice) {
            case 1: {
                std::string pId = getStringInput("Passenger ID: ");
                std::string fNum = getStringInput("Flight Number: ");
                try {
                    airline.bookTicket(pId, fNum);
                } catch (const std::exception& ex) {
                    std::cout << "\n[BOOKING FAILED] " << ex.what() << std::endl;
                }
                break;
            }
            case 2:
                airline.listAllTickets();
                break;
            case 3: {
                std::string tId = getStringInput("Enter Ticket ID: ");
                try {
                    Ticket t = airline.findTicket(tId);
                    std::cout << "\nTicket Details:" << std::endl;
                    std::cout << t << std::endl;
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                }
                break;
            }
            case 0:
                break;
            default:
                std::cout << "[Invalid Choice]" << std::endl;
        }
    }
}

void Menu::handleCancellation() {
    std::cout << "\n--- CANCELLATION & REFUND CALCULATOR ---" << std::endl;
    std::string tId = getStringInput("Enter Ticket ID to cancel: ");
    double hours = getDoubleInput("Hours remaining before flight departure: ");

    try {
        airline.cancelTicket(tId, hours);
    } catch (const std::exception& ex) {
        std::cout << "\n[CANCELLATION REJECTED] " << ex.what() << std::endl;
    }
}

void Menu::handleReports() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- REPORTS & ANALYTICS ---" << std::endl;
        std::cout << "1. Today's Departures Report" << std::endl;
        std::cout << "2. Flight Occupancy Percentage Report" << std::endl;
        std::cout << "3. Top 5 Highest-Revenue Flights Report" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        choice = getIntInput("Option: ");
        switch (choice) {
            case 1: {
                std::string date = getStringInput("Enter date (YYYY-MM-DD) [Default: 2026-09-20]: ");
                if (date.empty()) date = "2026-09-20";
                airline.reportTodaysDepartures(date);
                break;
            }
            case 2:
                airline.reportFlightOccupancy();
                break;
            case 3:
                airline.reportTop5RevenueFlights();
                break;
            case 0:
                break;
            default:
                std::cout << "[Invalid Choice]" << std::endl;
        }
    }
}
