#include "../include/Flight.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Base Flight Constructor
Flight::Flight(const std::string& fNum, const std::string& orig, const std::string& dest,
               const std::string& date, const std::string& time, int seats, double fare)
    : flightNumber(fNum), origin(orig), destination(dest),
      departureDate(date), departureTime(time),
      totalSeats(seats), availableSeats(seats), baseFare(fare) {}

bool Flight::bookSeat() {
    if (availableSeats > 0) {
        availableSeats--;
        return true;
    }
    return false;
}

bool Flight::releaseSeat() {
    if (availableSeats < totalSeats) {
        availableSeats++;
        return true;
    }
    return false;
}

double Flight::getOccupancyPercentage() const {
    if (totalSeats <= 0) return 0.0;
    int booked = totalSeats - availableSeats;
    return (static_cast<double>(booked) / totalSeats) * 100.0;
}

std::ostream& operator<<(std::ostream& os, const Flight& flight) {
    os << "[" << std::left << std::setw(13) << flight.getTypeName() << "] "
       << "Flight " << std::left << std::setw(8) << flight.getFlightNumber() << " "
       << flight.getOrigin() << " -> " << flight.getDestination() << " | "
       << flight.getDepartureDate() << " " << flight.getDepartureTime() << " | "
       << "Seats: " << flight.getAvailableSeats() << "/" << flight.getTotalSeats() << " | "
       << "Fare: $" << std::fixed << std::setprecision(2) << flight.calculateBaseFare();
    return os;
}

// -------------------------------------------------------------
// DomesticFlight Implementation
// -------------------------------------------------------------
DomesticFlight::DomesticFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                               const std::string& date, const std::string& time, int seats, double fare,
                               double taxRate)
    : Flight(fNum, orig, dest, date, time, seats, fare), stateTaxRate(taxRate) {}

double DomesticFlight::calculateBaseFare() const {
    // Base fare + state tax
    return baseFare * (1.0 + stateTaxRate);
}

void DomesticFlight::displayDetails() const {
    std::cout << "=== Domestic Flight Details ===" << std::endl;
    std::cout << "Flight Number  : " << flightNumber << std::endl;
    std::cout << "Route          : " << origin << " -> " << destination << std::endl;
    std::cout << "Departure      : " << departureDate << " at " << departureTime << std::endl;
    std::cout << "Total Seats    : " << totalSeats << std::endl;
    std::cout << "Available Seats: " << availableSeats << std::endl;
    std::cout << "Base Fare      : $" << std::fixed << std::setprecision(2) << baseFare << std::endl;
    std::cout << "State Tax Rate : " << (stateTaxRate * 100.0) << "%" << std::endl;
    std::cout << "Calculated Fare: $" << calculateBaseFare() << std::endl;
    std::cout << "Occupancy      : " << getOccupancyPercentage() << "%" << std::endl;
}

std::string DomesticFlight::serialize() const {
    std::ostringstream oss;
    oss << "DOMESTIC|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDate << "|" << departureTime << "|" << totalSeats << "|"
        << availableSeats << "|" << baseFare << "|" << stateTaxRate;
    return oss.str();
}

// -------------------------------------------------------------
// InternationalFlight Implementation
// -------------------------------------------------------------
InternationalFlight::InternationalFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                                         const std::string& date, const std::string& time, int seats, double fare,
                                         bool visaReq, double intlTax)
    : Flight(fNum, orig, dest, date, time, seats, fare),
      visaRequired(visaReq), internationalTax(intlTax) {}

double InternationalFlight::calculateBaseFare() const {
    // Base fare + international tax + optional visa processing fee
    double fee = baseFare + internationalTax;
    if (visaRequired) {
        fee += 15.0; // Visa processing levy
    }
    return fee;
}

void InternationalFlight::displayDetails() const {
    std::cout << "=== International Flight Details ===" << std::endl;
    std::cout << "Flight Number  : " << flightNumber << std::endl;
    std::cout << "Route          : " << origin << " -> " << destination << std::endl;
    std::cout << "Departure      : " << departureDate << " at " << departureTime << std::endl;
    std::cout << "Total Seats    : " << totalSeats << std::endl;
    std::cout << "Available Seats: " << availableSeats << std::endl;
    std::cout << "Base Fare      : $" << std::fixed << std::setprecision(2) << baseFare << std::endl;
    std::cout << "Intl Tax       : $" << internationalTax << std::endl;
    std::cout << "Visa Required  : " << (visaRequired ? "Yes" : "No") << std::endl;
    std::cout << "Calculated Fare: $" << calculateBaseFare() << std::endl;
    std::cout << "Occupancy      : " << getOccupancyPercentage() << "%" << std::endl;
}

std::string InternationalFlight::serialize() const {
    std::ostringstream oss;
    oss << "INTERNATIONAL|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDate << "|" << departureTime << "|" << totalSeats << "|"
        << availableSeats << "|" << baseFare << "|" << (visaRequired ? "1" : "0") << "|"
        << internationalTax;
    return oss.str();
}

// -------------------------------------------------------------
// CharterFlight Implementation
// -------------------------------------------------------------
CharterFlight::CharterFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                             const std::string& date, const std::string& time, int seats, double fare,
                             const std::string& holder, double charterFee)
    : Flight(fNum, orig, dest, date, time, seats, fare),
      contractHolder(holder), flatCharterFee(charterFee) {}

double CharterFlight::calculateBaseFare() const {
    // Base fare per seat = portion of flat charter fee + individual seat base
    if (totalSeats == 0) return baseFare;
    return baseFare + (flatCharterFee / totalSeats);
}

void CharterFlight::displayDetails() const {
    std::cout << "=== Charter Flight Details ===" << std::endl;
    std::cout << "Flight Number  : " << flightNumber << std::endl;
    std::cout << "Contract Holder: " << contractHolder << std::endl;
    std::cout << "Route          : " << origin << " -> " << destination << std::endl;
    std::cout << "Departure      : " << departureDate << " at " << departureTime << std::endl;
    std::cout << "Total Seats    : " << totalSeats << std::endl;
    std::cout << "Available Seats: " << availableSeats << std::endl;
    std::cout << "Flat Charter Fee: $" << std::fixed << std::setprecision(2) << flatCharterFee << std::endl;
    std::cout << "Per-Seat Fare  : $" << calculateBaseFare() << std::endl;
    std::cout << "Occupancy      : " << getOccupancyPercentage() << "%" << std::endl;
}

std::string CharterFlight::serialize() const {
    std::ostringstream oss;
    oss << "CHARTER|" << flightNumber << "|" << origin << "|" << destination << "|"
        << departureDate << "|" << departureTime << "|" << totalSeats << "|"
        << availableSeats << "|" << baseFare << "|" << contractHolder << "|"
        << flatCharterFee;
    return oss.str();
}
