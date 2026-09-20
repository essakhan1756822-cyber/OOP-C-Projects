#include "../include/Airline.h"
#include "../include/SearchUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

Airline::Airline(const std::string& airlineName)
    : name(airlineName), nextTicketNumber(1001) {}

std::string Airline::generateTicketId() {
    return "TICK" + std::to_string(nextTicketNumber++);
}

// -------------------------------------------------------------
// Flight Management
// -------------------------------------------------------------
void Airline::addFlight(const std::shared_ptr<Flight>& flight) {
    if (!flight) return;
    for (const auto& f : flights) {
        if (f->getFlightNumber() == flight->getFlightNumber()) {
            std::cout << "[Error] Flight " << flight->getFlightNumber() << " already exists." << std::endl;
            return;
        }
    }
    flights.push_back(flight);
    std::cout << "[Success] Flight " << flight->getFlightNumber() << " added successfully." << std::endl;
}

bool Airline::removeFlight(const std::string& flightNumber) {
    auto it = std::remove_if(flights.begin(), flights.end(),
        [&flightNumber](const std::shared_ptr<Flight>& f) {
            return f->getFlightNumber() == flightNumber;
        });
    if (it != flights.end()) {
        flights.erase(it, flights.end());
        return true;
    }
    return false;
}

std::shared_ptr<Flight> Airline::findFlight(const std::string& flightNumber) const {
    auto it = std::find_if(flights.begin(), flights.end(),
        [&flightNumber](const std::shared_ptr<Flight>& f) {
            return f->getFlightNumber() == flightNumber;
        });
    if (it != flights.end()) {
        return *it;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Flight>> Airline::searchFlightsByRoute(const std::string& origin, const std::string& dest) const {
    // Demonstrates Function Template Usage (searchItems)
    return searchItems(flights, [&origin, &dest](const std::shared_ptr<Flight>& f) {
        return f->getOrigin() == origin && f->getDestination() == dest;
    });
}

std::vector<std::shared_ptr<Flight>> Airline::searchFlightsByDate(const std::string& date) const {
    // Demonstrates Class Template Usage (SearchEngine)
    return SearchEngine<std::shared_ptr<Flight>>::filter(flights, [&date](const std::shared_ptr<Flight>& f) {
        return f->getDepartureDate() == date;
    });
}

void Airline::listAllFlights() const {
    std::cout << "\n=================================== ALL FLIGHTS ===================================" << std::endl;
    if (flights.empty()) {
        std::cout << "No flights currently registered." << std::endl;
        return;
    }
    for (const auto& flight : flights) {
        std::cout << *flight << std::endl;
    }
    std::cout << "====================================================================================" << std::endl;
}

// -------------------------------------------------------------
// Passenger Management
// -------------------------------------------------------------
void Airline::registerPassenger(const std::shared_ptr<Passenger>& passenger) {
    if (!passenger) return;
    for (const auto& p : passengers) {
        if (p->getPassengerId() == passenger->getPassengerId()) {
            std::cout << "[Error] Passenger ID " << passenger->getPassengerId() << " already registered." << std::endl;
            return;
        }
    }
    passengers.push_back(passenger);
    std::cout << "[Success] Passenger " << passenger->getName() << " (" << passenger->getPassengerId() << ") registered." << std::endl;
}

bool Airline::removePassenger(const std::string& passengerId) {
    auto it = std::remove_if(passengers.begin(), passengers.end(),
        [&passengerId](const std::shared_ptr<Passenger>& p) {
            return p->getPassengerId() == passengerId;
        });
    if (it != passengers.end()) {
        passengers.erase(it, passengers.end());
        return true;
    }
    return false;
}

std::shared_ptr<Passenger> Airline::findPassenger(const std::string& passengerId) const {
    auto it = std::find_if(passengers.begin(), passengers.end(),
        [&passengerId](const std::shared_ptr<Passenger>& p) {
            return p->getPassengerId() == passengerId;
        });
    if (it != passengers.end()) {
        return *it;
    }
    return nullptr;
}

void Airline::viewPassengerHistory(const std::string& passengerId) const {
    auto passenger = findPassenger(passengerId);
    if (!passenger) {
        throw PassengerNotFoundException(passengerId);
    }

    std::cout << "\n=================== BOOKING HISTORY FOR " << passenger->getName() << " (" << passengerId << ") ===================" << std::endl;
    passenger->displayDetails();
    std::cout << "------------------------------------------------------------------------------------" << std::endl;
    
    bool found = false;
    for (const auto& ticket : tickets) {
        if (ticket.getPassengerId() == passengerId) {
            std::cout << ticket << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No booking records found for this passenger." << std::endl;
    }
    std::cout << "====================================================================================" << std::endl;
}

void Airline::listAllPassengers() const {
    std::cout << "\n================================= REGISTERED PASSENGERS =================================" << std::endl;
    if (passengers.empty()) {
        std::cout << "No passengers registered." << std::endl;
        return;
    }
    for (const auto& p : passengers) {
        p->displayDetails();
        std::cout << "--------------------------------------------------------" << std::endl;
    }
}

// -------------------------------------------------------------
// Ticket & Booking Operations
// -------------------------------------------------------------
Ticket Airline::bookTicket(const std::string& passengerId, const std::string& flightNumber) {
    auto passenger = findPassenger(passengerId);
    if (!passenger) {
        throw PassengerNotFoundException(passengerId);
    }

    auto flight = findFlight(flightNumber);
    if (!flight) {
        throw FlightNotFoundException(flightNumber);
    }

    // Check duplicate active booking
    for (const auto& t : tickets) {
        if (t.getPassengerId() == passengerId && t.getFlightNumber() == flightNumber && t.isConfirmed()) {
            throw DuplicateBookingException(passengerId, flightNumber);
        }
    }

    // Check flight capacity
    if (flight->getAvailableSeats() <= 0) {
        throw FlightFullException(flightNumber);
    }

    // Book seat on flight
    flight->bookSeat();

    // Assign seat number (Total - Available)
    int seatNo = flight->getTotalSeats() - flight->getAvailableSeats();
    double fare = flight->calculateBaseFare();

    // Award loyalty points to passenger based on flight booking
    passenger->addLoyaltyPoints(static_cast<int>(fare / 10.0));

    std::string tId = generateTicketId();
    Ticket newTicket(tId, passengerId, flightNumber, seatNo, fare, "CONFIRMED", "2026-09-20");
    tickets.push_back(newTicket);

    std::cout << "\n[Success] Ticket booked successfully!" << std::endl;
    std::cout << newTicket << std::endl;

    return newTicket;
}

double Airline::cancelTicket(const std::string& ticketId, double hoursBeforeDeparture) {
    auto it = std::find_if(tickets.begin(), tickets.end(),
        [&ticketId](const Ticket& t) { return t.getTicketId() == ticketId; });

    if (it == tickets.end()) {
        throw TicketNotFoundException(ticketId);
    }

    if (it->isCancelled()) {
        throw InvalidCancellationException("Ticket " + ticketId + " has already been cancelled.");
    }

    // Mark ticket cancelled
    it->cancelTicket();

    // Release seat on flight
    auto flight = findFlight(it->getFlightNumber());
    if (flight) {
        flight->releaseSeat();
    }

    // Compute refund polymorphically based on passenger class
    auto passenger = findPassenger(it->getPassengerId());
    double refundPct = 0.0;
    if (passenger) {
        refundPct = passenger->getRefundPercentage(hoursBeforeDeparture);
    } else {
        refundPct = 0.50; // default fallback if passenger record missing
    }

    double refundAmount = it->getFarePaid() * refundPct;
    std::cout << "\n[Success] Ticket " << ticketId << " cancelled." << std::endl;
    std::cout << "Original Fare Paid : $" << std::fixed << std::setprecision(2) << it->getFarePaid() << std::endl;
    std::cout << "Refund Percentage  : " << (refundPct * 100.0) << "% (" << hoursBeforeDeparture << " hrs before flight)" << std::endl;
    std::cout << "Refund Amount      : $" << refundAmount << std::endl;

    return refundAmount;
}

Ticket Airline::findTicket(const std::string& ticketId) const {
    auto it = std::find_if(tickets.begin(), tickets.end(),
        [&ticketId](const Ticket& t) { return t.getTicketId() == ticketId; });
    if (it != tickets.end()) {
        return *it;
    }
    throw TicketNotFoundException(ticketId);
}

void Airline::listAllTickets() const {
    std::cout << "\n=================================== ALL TICKETS ===================================" << std::endl;
    if (tickets.empty()) {
        std::cout << "No tickets issued yet." << std::endl;
        return;
    }
    for (const auto& t : tickets) {
        std::cout << t << std::endl;
    }
    std::cout << "====================================================================================" << std::endl;
}

// -------------------------------------------------------------
// Reports (Functional Requirement 4.2 #5)
// -------------------------------------------------------------
void Airline::reportTodaysDepartures(const std::string& date) const {
    std::cout << "\n=================== DEPARTURES REPORT (" << date << ") ===================" << std::endl;
    auto todaysFlights = searchFlightsByDate(date);
    if (todaysFlights.empty()) {
        std::cout << "No flights scheduled to depart on " << date << "." << std::endl;
    } else {
        for (const auto& f : todaysFlights) {
            std::cout << *f << std::endl;
        }
    }
    std::cout << "===========================================================================" << std::endl;
}

void Airline::reportFlightOccupancy() const {
    std::cout << "\n======================== FLIGHT OCCUPANCY REPORT ========================" << std::endl;
    if (flights.empty()) {
        std::cout << "No flights to report." << std::endl;
        return;
    }

    // Make a shallow copy vector and sort using STL algorithm std::sort
    std::vector<std::shared_ptr<Flight>> sortedFlights = flights;
    std::sort(sortedFlights.begin(), sortedFlights.end(),
        [](const std::shared_ptr<Flight>& a, const std::shared_ptr<Flight>& b) {
            return a->getOccupancyPercentage() > b->getOccupancyPercentage();
        });

    std::cout << std::left << std::setw(12) << "Flight No"
              << std::setw(15) << "Type"
              << std::setw(20) << "Route"
              << std::setw(15) << "Booked/Total"
              << std::setw(15) << "Occupancy %" << std::endl;
    std::cout << "-------------------------------------------------------------------------";
    std::cout << std::endl;

    for (const auto& f : sortedFlights) {
        int booked = f->getTotalSeats() - f->getAvailableSeats();
        std::string route = f->getOrigin() + "->" + f->getDestination();
        std::string seatsStr = std::to_string(booked) + "/" + std::to_string(f->getTotalSeats());

        std::cout << std::left << std::setw(12) << f->getFlightNumber()
                  << std::setw(15) << f->getTypeName()
                  << std::setw(20) << route
                  << std::setw(15) << seatsStr
                  << std::fixed << std::setprecision(1) << f->getOccupancyPercentage() << "%"
                  << std::endl;
    }
    std::cout << "=========================================================================" << std::endl;
}

void Airline::reportTop5RevenueFlights() const {
    std::cout << "\n==================== TOP 5 HIGHEST-REVENUE FLIGHTS ====================" << std::endl;
    if (flights.empty()) {
        std::cout << "No flights available." << std::endl;
        return;
    }

    // Map flightNumber to total revenue from confirmed tickets
    std::map<std::string, double> flightRevenue;
    for (const auto& f : flights) {
        flightRevenue[f->getFlightNumber()] = 0.0;
    }
    for (const auto& t : tickets) {
        if (t.isConfirmed()) {
            flightRevenue[t.getFlightNumber()] += t.getFarePaid();
        }
    }

    // Create a vector of pairs for sorting using STL std::sort
    std::vector<std::pair<std::string, double>> revList(flightRevenue.begin(), flightRevenue.end());
    std::sort(revList.begin(), revList.end(),
        [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
            return a.second > b.second;
        });

    std::cout << std::left << std::setw(12) << "Rank"
              << std::setw(12) << "Flight No"
              << std::setw(20) << "Route"
              << std::setw(15) << "Total Revenue" << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;

    int rank = 1;
    for (const auto& item : revList) {
        if (rank > 5) break;
        auto f = findFlight(item.first);
        std::string route = f ? (f->getOrigin() + "->" + f->getDestination()) : "N/A";

        std::cout << std::left << std::setw(12) << ("#" + std::to_string(rank))
                  << std::setw(12) << item.first
                  << std::setw(20) << route
                  << "$" << std::fixed << std::setprecision(2) << item.second
                  << std::endl;
        rank++;
    }
    std::cout << "===================================================================" << std::endl;
}

// -------------------------------------------------------------
// File Persistence (Functional Requirement 4.2 #6)
// -------------------------------------------------------------
bool Airline::saveData(const std::string& dataDir) const {
    // Save flights
    std::ofstream fFile(dataDir + "flights.txt");
    if (!fFile.is_open()) return false;
    for (const auto& f : flights) {
        fFile << f->serialize() << "\n";
    }
    fFile.close();

    // Save passengers
    std::ofstream pFile(dataDir + "passengers.txt");
    if (!pFile.is_open()) return false;
    for (const auto& p : passengers) {
        pFile << p->serialize() << "\n";
    }
    pFile.close();

    // Save tickets
    std::ofstream tFile(dataDir + "tickets.txt");
    if (!tFile.is_open()) return false;
    for (const auto& t : tickets) {
        tFile << t.serialize() << "\n";
    }
    tFile.close();

    return true;
}

bool Airline::loadData(const std::string& dataDir) {
    std::ifstream fFile(dataDir + "flights.txt");
    if (!fFile.is_open()) return false;

    flights.clear();
    std::string line;
    while (std::getline(fFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string type, fNum, orig, dest, date, time, totalSStr, availSStr, fareStr, extra1, extra2;

        std::getline(ss, type, '|');
        std::getline(ss, fNum, '|');
        std::getline(ss, orig, '|');
        std::getline(ss, dest, '|');
        std::getline(ss, date, '|');
        std::getline(ss, time, '|');
        std::getline(ss, totalSStr, '|');
        std::getline(ss, availSStr, '|');
        std::getline(ss, fareStr, '|');
        std::getline(ss, extra1, '|');
        std::getline(ss, extra2, '|');

        int totalS = std::stoi(totalSStr);
        int availS = std::stoi(availSStr);
        double fare = std::stod(fareStr);

        std::shared_ptr<Flight> flight = nullptr;
        if (type == "DOMESTIC") {
            double tax = extra1.empty() ? 0.05 : std::stod(extra1);
            flight = std::make_shared<DomesticFlight>(fNum, orig, dest, date, time, totalS, fare, tax);
        } else if (type == "INTERNATIONAL") {
            bool visa = (extra1 == "1" || extra1 == "true");
            double intlTax = extra2.empty() ? 50.0 : std::stod(extra2);
            flight = std::make_shared<InternationalFlight>(fNum, orig, dest, date, time, totalS, fare, visa, intlTax);
        } else if (type == "CHARTER") {
            std::string holder = extra1;
            double charterFee = extra2.empty() ? 5000.0 : std::stod(extra2);
            flight = std::make_shared<CharterFlight>(fNum, orig, dest, date, time, totalS, fare, holder, charterFee);
        }

        if (flight) {
            flight->setAvailableSeats(availS);
            flights.push_back(flight);
        }
    }
    fFile.close();

    // Load Passengers
    std::ifstream pFile(dataDir + "passengers.txt");
    if (pFile.is_open()) {
        passengers.clear();
        while (std::getline(pFile, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string type, id, name, email, phone, ptsStr;

            std::getline(ss, type, '|');
            std::getline(ss, id, '|');
            std::getline(ss, name, '|');
            std::getline(ss, email, '|');
            std::getline(ss, phone, '|');
            std::getline(ss, ptsStr, '|');

            int pts = ptsStr.empty() ? 0 : std::stoi(ptsStr);

            std::shared_ptr<Passenger> p = nullptr;
            if (type == "ECONOMY") {
                p = std::make_shared<EconomyPassenger>(id, name, email, phone, pts);
            } else if (type == "BUSINESS") {
                p = std::make_shared<BusinessPassenger>(id, name, email, phone, pts);
            } else if (type == "FIRSTCLASS") {
                p = std::make_shared<FirstClassPassenger>(id, name, email, phone, pts);
            }
            if (p) {
                passengers.push_back(p);
            }
        }
        pFile.close();
    }

    // Load Tickets
    std::ifstream tFile(dataDir + "tickets.txt");
    if (tFile.is_open()) {
        tickets.clear();
        int maxTicketNum = 1000;
        while (std::getline(tFile, line)) {
            if (line.empty()) continue;
            Ticket t = Ticket::deserialize(line);
            if (!t.getTicketId().empty()) {
                tickets.push_back(t);
                // Extract ticket number counter
                if (t.getTicketId().find("TICK") == 0) {
                    try {
                        int num = std::stoi(t.getTicketId().substr(4));
                        if (num > maxTicketNum) maxTicketNum = num;
                    } catch (...) {}
                }
            }
        }
        nextTicketNumber = maxTicketNum + 1;
        tFile.close();
    }

    return true;
}

void Airline::loadSampleData() {
    flights.clear();
    passengers.clear();
    tickets.clear();

    // 10 Pre-loaded Flights (Requirement: At least 10 flights)
    flights.push_back(std::make_shared<DomesticFlight>("SL101", "New York", "Chicago", "2026-09-20", "08:00", 120, 150.0, 0.05));
    flights.push_back(std::make_shared<DomesticFlight>("SL102", "Chicago", "Dallas", "2026-09-20", "11:30", 100, 180.0, 0.06));
    flights.push_back(std::make_shared<DomesticFlight>("SL103", "Los Angeles", "San Francisco", "2026-09-21", "14:15", 80, 120.0, 0.04));
    flights.push_back(std::make_shared<DomesticFlight>("SL104", "Seattle", "Denver", "2026-09-21", "17:00", 90, 160.0, 0.05));

    flights.push_back(std::make_shared<InternationalFlight>("SL501", "New York", "London", "2026-09-20", "20:00", 250, 650.0, false, 75.0));
    flights.push_back(std::make_shared<InternationalFlight>("SL502", "Los Angeles", "Tokyo", "2026-09-21", "13:00", 280, 850.0, true, 90.0));
    flights.push_back(std::make_shared<InternationalFlight>("SL503", "Miami", "Paris", "2026-09-22", "18:45", 220, 720.0, true, 80.0));
    flights.push_back(std::make_shared<InternationalFlight>("SL504", "Chicago", "Frankfurt", "2026-09-22", "22:15", 200, 780.0, false, 70.0));

    flights.push_back(std::make_shared<CharterFlight>("SL901", "Boston", "Aspen", "2026-09-20", "09:30", 12, 400.0, "Acme Corp", 4800.0));
    flights.push_back(std::make_shared<CharterFlight>("SL902", "Las Vegas", "Cabo", "2026-09-23", "16:00", 16, 500.0, "Apex Tech", 6400.0));

    // 8 Pre-loaded Passengers (Requirement: At least 8 passengers)
    passengers.push_back(std::make_shared<EconomyPassenger>("P001", "Alice Smith", "alice@gmail.com", "+1-555-0101", 120));
    passengers.push_back(std::make_shared<EconomyPassenger>("P002", "Bob Jones", "bob@yahoo.com", "+1-555-0102", 80));
    passengers.push_back(std::make_shared<EconomyPassenger>("P003", "Charlie Brown", "charlie@outlook.com", "+1-555-0103", 45));

    passengers.push_back(std::make_shared<BusinessPassenger>("P004", "Diana Prince", "diana@wayne.com", "+1-555-0104", 450));
    passengers.push_back(std::make_shared<BusinessPassenger>("P005", "Evan Wright", "evan@stark.com", "+1-555-0105", 380));
    passengers.push_back(std::make_shared<BusinessPassenger>("P006", "Fiona Gallagher", "fiona@chicago.org", "+1-555-0106", 290));

    passengers.push_back(std::make_shared<FirstClassPassenger>("P007", "George Clark", "george@clark.io", "+1-555-0107", 1200));
    passengers.push_back(std::make_shared<FirstClassPassenger>("P008", "Hannah Abbott", "hannah@hogwarts.edu", "+1-555-0108", 950));

    // Pre-loaded Bookings
    try {
        bookTicket("P001", "SL101");
        bookTicket("P004", "SL501");
        bookTicket("P007", "SL501");
        bookTicket("P002", "SL102");
        bookTicket("P005", "SL502");
        bookTicket("P008", "SL901");
    } catch (...) {}
}
