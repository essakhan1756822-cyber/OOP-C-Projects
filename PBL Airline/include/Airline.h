#ifndef AIRLINE_H
#define AIRLINE_H

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "Flight.h"
#include "Passenger.h"
#include "Ticket.h"
#include "Exceptions.h"

class Airline {
private:
    std::string name;
    std::vector<std::shared_ptr<Flight>> flights;
    std::vector<std::shared_ptr<Passenger>> passengers;
    std::vector<Ticket> tickets;
    int nextTicketNumber;

    // Helper methods
    std::string generateTicketId();

public:
    explicit Airline(const std::string& airlineName = "SkyLink Airways");

    // Flight Management
    void addFlight(const std::shared_ptr<Flight>& flight);
    bool removeFlight(const std::string& flightNumber);
    std::shared_ptr<Flight> findFlight(const std::string& flightNumber) const;
    std::vector<std::shared_ptr<Flight>> searchFlightsByRoute(const std::string& origin, const std::string& dest) const;
    std::vector<std::shared_ptr<Flight>> searchFlightsByDate(const std::string& date) const;
    void listAllFlights() const;

    // Passenger Management
    void registerPassenger(const std::shared_ptr<Passenger>& passenger);
    bool removePassenger(const std::string& passengerId);
    std::shared_ptr<Passenger> findPassenger(const std::string& passengerId) const;
    void viewPassengerHistory(const std::string& passengerId) const;
    void listAllPassengers() const;

    // Ticket & Booking Operations
    Ticket bookTicket(const std::string& passengerId, const std::string& flightNumber);
    double cancelTicket(const std::string& ticketId, double hoursBeforeDeparture);
    void listAllTickets() const;
    Ticket findTicket(const std::string& ticketId) const;

    // Reports (Functional Requirement 4.2 #5)
    void reportTodaysDepartures(const std::string& date) const;
    void reportFlightOccupancy() const;
    void reportTop5RevenueFlights() const;

    // File Persistence (Functional Requirement 4.2 #6)
    bool saveData(const std::string& dataDir = "data/") const;
    bool loadData(const std::string& dataDir = "data/");
    void loadSampleData();

    // Getters
    std::string getName() const { return name; }
    size_t getFlightCount() const { return flights.size(); }
    size_t getPassengerCount() const { return passengers.size(); }
    size_t getTicketCount() const { return tickets.size(); }
};

#endif // AIRLINE_H
