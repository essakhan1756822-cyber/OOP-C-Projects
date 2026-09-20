#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <iostream>

// Base Class: Passenger
class Passenger {
protected:
    std::string passengerId;
    std::string name;
    std::string email;
    std::string phone;
    int loyaltyPoints;

public:
    Passenger(const std::string& id, const std::string& pName,
              const std::string& pEmail, const std::string& pPhone,
              int points = 0);

    virtual ~Passenger() = default;

    // Pure Virtual Functions for Polymorphism
    virtual double getBaggageAllowanceKg() const = 0;
    virtual double getLoyaltyMultiplier() const = 0;
    virtual double getRefundPercentage(double hoursBeforeDeparture) const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual std::string serialize() const = 0;

    // Common Member Functions
    void addLoyaltyPoints(int points);

    // Getters
    std::string getPassengerId() const { return passengerId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    int getLoyaltyPoints() const { return loyaltyPoints; }

    // Setters
    void setEmail(const std::string& pEmail) { email = pEmail; }
    void setPhone(const std::string& pPhone) { phone = pPhone; }
};

// Derived Class 1: EconomyPassenger
class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(const std::string& id, const std::string& pName,
                     const std::string& pEmail, const std::string& pPhone,
                     int points = 0);

    double getBaggageAllowanceKg() const override { return 20.0; }
    double getLoyaltyMultiplier() const override { return 1.0; }
    double getRefundPercentage(double hoursBeforeDeparture) const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "Economy"; }
    std::string serialize() const override;
};

// Derived Class 2: BusinessPassenger
class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(const std::string& id, const std::string& pName,
                      const std::string& pEmail, const std::string& pPhone,
                      int points = 0);

    double getBaggageAllowanceKg() const override { return 35.0; }
    double getLoyaltyMultiplier() const override { return 1.5; }
    double getRefundPercentage(double hoursBeforeDeparture) const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "Business"; }
    std::string serialize() const override;
};

// Derived Class 3: FirstClassPassenger
class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(const std::string& id, const std::string& pName,
                        const std::string& pEmail, const std::string& pPhone,
                        int points = 0);

    double getBaggageAllowanceKg() const override { return 50.0; }
    double getLoyaltyMultiplier() const override { return 2.0; }
    double getRefundPercentage(double hoursBeforeDeparture) const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "FirstClass"; }
    std::string serialize() const override;
};

#endif // PASSENGER_H
