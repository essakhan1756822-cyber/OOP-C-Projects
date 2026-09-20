#include "../include/Ticket.h"
#include <iomanip>
#include <sstream>

Ticket::Ticket()
    : ticketId(""), passengerId(""), flightNumber(""),
      seatNumber(0), farePaid(0.0), bookingStatus("CANCELLED"), bookingDate("2026-09-20") {}

Ticket::Ticket(const std::string& tId, const std::string& pId, const std::string& fNum,
               int seat, double fare, const std::string& status, const std::string& bDate)
    : ticketId(tId), passengerId(pId), flightNumber(fNum),
      seatNumber(seat), farePaid(fare), bookingStatus(status), bookingDate(bDate) {}

void Ticket::cancelTicket() {
    bookingStatus = "CANCELLED";
}

std::string Ticket::serialize() const {
    std::ostringstream oss;
    oss << ticketId << "|" << passengerId << "|" << flightNumber << "|"
        << seatNumber << "|" << farePaid << "|" << bookingStatus << "|" << bookingDate;
    return oss.str();
}

Ticket Ticket::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string tId, pId, fNum, seatStr, fareStr, status, bDate;

    if (std::getline(ss, tId, '|') &&
        std::getline(ss, pId, '|') &&
        std::getline(ss, fNum, '|') &&
        std::getline(ss, seatStr, '|') &&
        std::getline(ss, fareStr, '|') &&
        std::getline(ss, status, '|') &&
        std::getline(ss, bDate, '|')) {
        
        int seat = std::stoi(seatStr);
        double fare = std::stod(fareStr);
        return Ticket(tId, pId, fNum, seat, fare, status, bDate);
    }
    return Ticket();
}

std::ostream& operator<<(std::ostream& os, const Ticket& ticket) {
    os << "Ticket ID  : " << std::left << std::setw(10) << ticket.getTicketId() << " | "
       << "Passenger: " << std::left << std::setw(8) << ticket.getPassengerId() << " | "
       << "Flight: " << std::left << std::setw(8) << ticket.getFlightNumber() << " | "
       << "Seat: " << std::setw(3) << ticket.getSeatNumber() << " | "
       << "Fare: $" << std::fixed << std::setprecision(2) << std::setw(8) << ticket.getFarePaid() << " | "
       << "Status: " << ticket.getBookingStatus();
    return os;
}
