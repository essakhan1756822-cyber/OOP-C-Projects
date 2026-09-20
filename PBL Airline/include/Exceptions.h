#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Thrown when trying to book a seat on a flight that has 0 available seats
class FlightFullException : public std::runtime_error {
public:
    explicit FlightFullException(const std::string& flightNum)
        : std::runtime_error("Booking Error: Flight " + flightNum + " is fully booked.") {}
};

// Thrown when cancellation rules are violated or ticket is already cancelled
class InvalidCancellationException : public std::runtime_error {
public:
    explicit InvalidCancellationException(const std::string& message)
        : std::runtime_error("Cancellation Error: " + message) {}
};

// Thrown when a passenger attempts to book the same flight twice
class DuplicateBookingException : public std::runtime_error {
public:
    DuplicateBookingException(const std::string& passengerId, const std::string& flightNum)
        : std::runtime_error("Booking Error: Passenger " + passengerId + " already has a booking on flight " + flightNum + ".") {}
};

// Thrown when a requested passenger ID is not found in the airline system
class PassengerNotFoundException : public std::runtime_error {
public:
    explicit PassengerNotFoundException(const std::string& passengerId)
        : std::runtime_error("Lookup Error: Passenger ID " + passengerId + " not found.") {}
};

// Thrown when a requested flight number is not found
class FlightNotFoundException : public std::runtime_error {
public:
    explicit FlightNotFoundException(const std::string& flightNum)
        : std::runtime_error("Lookup Error: Flight " + flightNum + " not found.") {}
};

// Thrown when a ticket ID is not found
class TicketNotFoundException : public std::runtime_error {
public:
    explicit TicketNotFoundException(const std::string& ticketId)
        : std::runtime_error("Lookup Error: Ticket ID " + ticketId + " not found.") {}
};

#endif // EXCEPTIONS_H
