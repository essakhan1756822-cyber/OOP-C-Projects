# SkyLink Airways - UML Class Diagram Documentation

This document contains the complete UML Class Diagram for the **SkyLink Airways Airline Reservation & Flight Management System**, illustrating class attributes, methods, inheritance hierarchies, and aggregation relationships.

---

## Mermaid Class Diagram

```mermaid
classDiagram
    %% Abstract Flight Hierarchy
    class Flight {
        <<Abstract>>
        # string flightNumber
        # string origin
        # string destination
        # string departureDate
        # string departureTime
        # int totalSeats
        # int availableSeats
        # double baseFare
        +calculateBaseFare()* double
        +displayDetails()* void
        +getTypeName()* string
        +serialize()* string
        +bookSeat() bool
        +releaseSeat() bool
        +getOccupancyPercentage() double
    }

    class DomesticFlight {
        -double stateTaxRate
        +calculateBaseFare() double
        +displayDetails() void
        +serialize() string
    }

    class InternationalFlight {
        -bool visaRequired
        -double internationalTax
        +calculateBaseFare() double
        +displayDetails() void
        +serialize() string
    }

    class CharterFlight {
        -string contractHolder
        -double flatCharterFee
        +calculateBaseFare() double
        +displayDetails() void
        +serialize() string
    }

    Flight <|-- DomesticFlight : Inherits
    Flight <|-- InternationalFlight : Inherits
    Flight <|-- CharterFlight : Inherits

    %% Base Passenger Hierarchy
    class Passenger {
        <<Abstract>>
        # string passengerId
        # string name
        # string email
        # string phone
        # int loyaltyPoints
        +getBaggageAllowanceKg()* double
        +getLoyaltyMultiplier()* double
        +getRefundPercentage(double hours)* double
        +displayDetails()* void
        +getTypeName()* string
        +serialize()* string
        +addLoyaltyPoints(int points) void
    }

    class EconomyPassenger {
        +getBaggageAllowanceKg() double
        +getLoyaltyMultiplier() double
        +getRefundPercentage(double hours) double
        +displayDetails() void
    }

    class BusinessPassenger {
        +getBaggageAllowanceKg() double
        +getLoyaltyMultiplier() double
        +getRefundPercentage(double hours) double
        +displayDetails() void
    }

    class FirstClassPassenger {
        +getBaggageAllowanceKg() double
        +getLoyaltyMultiplier() double
        +getRefundPercentage(double hours) double
        +displayDetails() void
    }

    Passenger <|-- EconomyPassenger : Inherits
    Passenger <|-- BusinessPassenger : Inherits
    Passenger <|-- FirstClassPassenger : Inherits

    %% Ticket Class
    class Ticket {
        -string ticketId
        -string passengerId
        -string flightNumber
        -int seatNumber
        -double farePaid
        -string bookingStatus
        -string bookingDate
        +cancelTicket() void
        +isConfirmed() bool
        +isCancelled() bool
        +serialize() string
        +deserialize(string line)$ Ticket
    }

    %% Airline Aggregator Class
    class Airline {
        -string name
        -vector~shared_ptr~Flight~~ flights
        -vector~shared_ptr~Passenger~~ passengers
        -vector~Ticket~ tickets
        -int nextTicketNumber
        +addFlight(Flight) void
        +removeFlight(string) bool
        +registerPassenger(Passenger) void
        +bookTicket(string, string) Ticket
        +cancelTicket(string, double) double
        +reportTodaysDepartures(string) void
        +reportFlightOccupancy() void
        +reportTop5RevenueFlights() void
        +saveData(string) bool
        +loadData(string) bool
    }

    %% System Exception Hierarchy
    class std_runtime_error {
        <<std::runtime_error>>
    }
    class FlightFullException {
        +FlightFullException(string fNum)
    }
    class InvalidCancellationException {
        +InvalidCancellationException(string msg)
    }
    class DuplicateBookingException {
        +DuplicateBookingException(string pId, string fNum)
    }

    std_runtime_error <|-- FlightFullException
    std_runtime_error <|-- InvalidCancellationException
    std_runtime_error <|-- DuplicateBookingException

    %% Relationships
    Airline "1" o-- "*" Flight : Aggregates
    Airline "1" o-- "*" Passenger : Aggregates
    Airline "1" o-- "*" Ticket : Aggregates
    Ticket "1" ..> "1" Passenger : Links Passenger ID
    Ticket "1" ..> "1" Flight : Links Flight Number
```

---

## Detailed Relationship Explanations

1. **Inheritance ($\rightarrow$)**:
   - `DomesticFlight`, `InternationalFlight`, and `CharterFlight` extend abstract base class `Flight`.
   - `EconomyPassenger`, `BusinessPassenger`, and `FirstClassPassenger` extend `Passenger`.
2. **Aggregation ($o-$)**:
   - `Airline` aggregates dynamic vectors of `Flight` pointers (`std::shared_ptr<Flight>`), `Passenger` pointers (`std::shared_ptr<Passenger>`), and `Ticket` objects.
3. **Association / Dependency ($\cdot\cdot>$)**:
   - `Ticket` references `passengerId` and `flightNumber` to connect a registered passenger to a scheduled flight.
