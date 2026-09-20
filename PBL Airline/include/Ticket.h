#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <iostream>

class Ticket {
private:
    std::string ticketId;
    std::string passengerId;
    std::string flightNumber;
    int seatNumber;
    double farePaid;
    std::string bookingStatus; // "CONFIRMED" or "CANCELLED"
    std::string bookingDate;   // YYYY-MM-DD

public:
    Ticket();
    Ticket(const std::string& tId, const std::string& pId, const std::string& fNum,
           int seat, double fare, const std::string& status = "CONFIRMED",
           const std::string& bDate = "2026-09-20");

    // Operations
    void cancelTicket();
    bool isConfirmed() const { return bookingStatus == "CONFIRMED"; }
    bool isCancelled() const { return bookingStatus == "CANCELLED"; }

    // Getters
    std::string getTicketId() const { return ticketId; }
    std::string getPassengerId() const { return passengerId; }
    std::string getFlightNumber() const { return flightNumber; }
    int getSeatNumber() const { return seatNumber; }
    double getFarePaid() const { return farePaid; }
    std::string getBookingStatus() const { return bookingStatus; }
    std::string getBookingDate() const { return bookingDate; }

    // Serialization for file persistence
    std::string serialize() const;
    static Ticket deserialize(const std::string& line);

    // Operator Overloading
    // 1. << operator for displaying Ticket details
    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);

    // 2. == operator for comparing tickets by ID
    bool operator==(const Ticket& other) const {
        return this->ticketId == other.ticketId;
    }
};

#endif // TICKET_H
