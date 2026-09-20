#include "../include/Passenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Base Passenger Constructor
Passenger::Passenger(const std::string& id, const std::string& pName,
                     const std::string& pEmail, const std::string& pPhone,
                     int points)
    : passengerId(id), name(pName), email(pEmail), phone(pPhone), loyaltyPoints(points) {}

void Passenger::addLoyaltyPoints(int points) {
    if (points > 0) {
        loyaltyPoints += static_cast<int>(points * getLoyaltyMultiplier());
    }
}

// -------------------------------------------------------------
// EconomyPassenger Implementation
// -------------------------------------------------------------
EconomyPassenger::EconomyPassenger(const std::string& id, const std::string& pName,
                                   const std::string& pEmail, const std::string& pPhone,
                                   int points)
    : Passenger(id, pName, pEmail, pPhone, points) {}

double EconomyPassenger::getRefundPercentage(double hoursBeforeDeparture) const {
    if (hoursBeforeDeparture >= 48.0) {
        return 0.85; // 85% refund
    } else if (hoursBeforeDeparture >= 24.0) {
        return 0.50; // 50% refund
    } else {
        return 0.00; // No refund within 24 hours
    }
}

void EconomyPassenger::displayDetails() const {
    std::cout << "[Economy Class Passenger]" << std::endl;
    std::cout << "ID            : " << passengerId << std::endl;
    std::cout << "Name          : " << name << std::endl;
    std::cout << "Email         : " << email << std::endl;
    std::cout << "Phone         : " << phone << std::endl;
    std::cout << "Loyalty Points: " << loyaltyPoints << std::endl;
    std::cout << "Baggage Limit : " << getBaggageAllowanceKg() << " kg" << std::endl;
}

std::string EconomyPassenger::serialize() const {
    std::ostringstream oss;
    oss << "ECONOMY|" << passengerId << "|" << name << "|" << email << "|" << phone << "|" << loyaltyPoints;
    return oss.str();
}

// -------------------------------------------------------------
// BusinessPassenger Implementation
// -------------------------------------------------------------
BusinessPassenger::BusinessPassenger(const std::string& id, const std::string& pName,
                                     const std::string& pEmail, const std::string& pPhone,
                                     int points)
    : Passenger(id, pName, pEmail, pPhone, points) {}

double BusinessPassenger::getRefundPercentage(double hoursBeforeDeparture) const {
    if (hoursBeforeDeparture >= 48.0) {
        return 0.95; // 95% refund
    } else if (hoursBeforeDeparture >= 24.0) {
        return 0.80; // 80% refund
    } else {
        return 0.40; // 40% refund within 24 hours
    }
}

void BusinessPassenger::displayDetails() const {
    std::cout << "[Business Class Passenger]" << std::endl;
    std::cout << "ID            : " << passengerId << std::endl;
    std::cout << "Name          : " << name << std::endl;
    std::cout << "Email         : " << email << std::endl;
    std::cout << "Phone         : " << phone << std::endl;
    std::cout << "Loyalty Points: " << loyaltyPoints << std::endl;
    std::cout << "Baggage Limit : " << getBaggageAllowanceKg() << " kg" << std::endl;
}

std::string BusinessPassenger::serialize() const {
    std::ostringstream oss;
    oss << "BUSINESS|" << passengerId << "|" << name << "|" << email << "|" << phone << "|" << loyaltyPoints;
    return oss.str();
}

// -------------------------------------------------------------
// FirstClassPassenger Implementation
// -------------------------------------------------------------
FirstClassPassenger::FirstClassPassenger(const std::string& id, const std::string& pName,
                                         const std::string& pEmail, const std::string& pPhone,
                                         int points)
    : Passenger(id, pName, pEmail, pPhone, points) {}

double FirstClassPassenger::getRefundPercentage(double hoursBeforeDeparture) const {
    if (hoursBeforeDeparture >= 24.0) {
        return 1.00; // 100% full refund
    } else {
        return 0.90; // 90% refund even within 24 hours
    }
}

void FirstClassPassenger::displayDetails() const {
    std::cout << "[First Class Passenger]" << std::endl;
    std::cout << "ID            : " << passengerId << std::endl;
    std::cout << "Name          : " << name << std::endl;
    std::cout << "Email         : " << email << std::endl;
    std::cout << "Phone         : " << phone << std::endl;
    std::cout << "Loyalty Points: " << loyaltyPoints << std::endl;
    std::cout << "Baggage Limit : " << getBaggageAllowanceKg() << " kg" << std::endl;
}

std::string FirstClassPassenger::serialize() const {
    std::ostringstream oss;
    oss << "FIRSTCLASS|" << passengerId << "|" << name << "|" << email << "|" << phone << "|" << loyaltyPoints;
    return oss.str();
}
