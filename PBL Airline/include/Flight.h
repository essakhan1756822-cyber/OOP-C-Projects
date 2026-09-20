#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <iostream>

// Abstract Base Class: Flight
class Flight {
protected:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureDate; // Format: YYYY-MM-DD
    std::string departureTime; // Format: HH:MM
    int totalSeats;
    int availableSeats;
    double baseFare;

public:
    Flight(const std::string& fNum, const std::string& orig, const std::string& dest,
           const std::string& date, const std::string& time, int seats, double fare);

    virtual ~Flight() = default;

    // Pure Virtual Functions (Abstraction & Polymorphism)
    virtual double calculateBaseFare() const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual std::string serialize() const = 0;

    // Common Member Functions
    bool bookSeat();
    bool releaseSeat();
    double getOccupancyPercentage() const;

    // Getters
    std::string getFlightNumber() const { return flightNumber; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    std::string getDepartureDate() const { return departureDate; }
    std::string getDepartureTime() const { return departureTime; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }
    double getRawBaseFare() const { return baseFare; }

    // Setters
    void setAvailableSeats(int seats) { availableSeats = seats; }
    void setBaseFare(double fare) { baseFare = fare; }

    // Operator Overloading for displaying Flight details
    friend std::ostream& operator<<(std::ostream& os, const Flight& flight);
};

// Derived Class 1: DomesticFlight
class DomesticFlight : public Flight {
private:
    double stateTaxRate; // e.g. 0.05 for 5%

public:
    DomesticFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                   const std::string& date, const std::string& time, int seats, double fare,
                   double taxRate = 0.05);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "Domestic"; }
    std::string serialize() const override;

    double getStateTaxRate() const { return stateTaxRate; }
};

// Derived Class 2: InternationalFlight
class InternationalFlight : public Flight {
private:
    bool visaRequired;
    double internationalTax;

public:
    InternationalFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                        const std::string& date, const std::string& time, int seats, double fare,
                        bool visaReq, double intlTax = 50.0);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "International"; }
    std::string serialize() const override;

    bool getVisaRequired() const { return visaRequired; }
    double getInternationalTax() const { return internationalTax; }
};

// Derived Class 3: CharterFlight
class CharterFlight : public Flight {
private:
    std::string contractHolder;
    double flatCharterFee;

public:
    CharterFlight(const std::string& fNum, const std::string& orig, const std::string& dest,
                  const std::string& date, const std::string& time, int seats, double fare,
                  const std::string& holder, double charterFee);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getTypeName() const override { return "Charter"; }
    std::string serialize() const override;

    std::string getContractHolder() const { return contractHolder; }
    double getFlatCharterFee() const { return flatCharterFee; }
};

#endif // FLIGHT_H
