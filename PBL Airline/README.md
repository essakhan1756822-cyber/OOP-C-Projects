# SkyLink Airways - Airline Reservation & Flight Management System (C++ OOP)

An object-oriented C++17 console application designed for **SkyLink Airways** to manage flight scheduling, passenger registrations, ticket bookings, seat allocation, cancellations with polymorphic refund calculation, state persistence, and operational reports.

---

## Features & Highlights

- **Four Pillars of OOP**:
  - **Abstraction**: Abstract base classes `Flight` and `Passenger` with pure virtual functions (`calculateBaseFare()`, `displayDetails()`, `getRefundPercentage()`).
  - **Encapsulation**: Private and protected member variables accessed safely through public getters/setters and operations.
  - **Inheritance**: 3 derived Flight classes (`DomesticFlight`, `InternationalFlight`, `CharterFlight`) and 3 derived Passenger classes (`EconomyPassenger`, `BusinessPassenger`, `FirstClassPassenger`).
  - **Polymorphism**: Dynamic dispatch for fare calculation, refund rules based on departure proximity, baggage allowances, and serialized outputs.
- **Operator Overloading**:
  - `operator<<`: Formatted stream insertion for `Flight` and `Ticket` objects.
  - `operator==`: Ticket ID equality comparison.
- **C++ Templates**:
  - Generic function template `searchItems(...)` for container predicate filtering.
  - Generic class template `SearchEngine<T>` for filtering and first-match queries.
- **Exception Handling**: Custom exception hierarchy (`FlightFullException`, `InvalidCancellationException`, `DuplicateBookingException`, `PassengerNotFoundException`, `FlightNotFoundException`, `TicketNotFoundException`).
- **STL Integration**: `std::vector`, `std::map`, `std::shared_ptr`, and STL algorithms (`std::sort`, `std::find_if`, `std::remove_if`).
- **File Persistence**: Full state (flights, passengers, tickets) saved to and loaded from plain text files (`data/flights.txt`, `data/passengers.txt`, `data/tickets.txt`).
- **Reports**:
  - Today's departures report.
  - Flight occupancy percentage report (sorted using `std::sort`).
  - Top 5 revenue-generating flights.

---

## File Layout

```
d:/PBL Airline/
├── include/
│   ├── Exceptions.h          # Custom exception classes
│   ├── Flight.h              # Abstract Flight base class & derived classes
│   ├── Passenger.h           # Base Passenger class & derived class hierarchies
│   ├── Ticket.h             # Ticket class with operator overloading
│   ├── SearchUtils.h         # Generic function & class templates
│   ├── Airline.h            # Main manager class aggregating entities
│   └── Menu.h               # Interactive console user interface
├── src/
│   ├── Flight.cpp            # Implementation of Flight hierarchy
│   ├── Passenger.cpp         # Implementation of Passenger hierarchy
│   ├── Ticket.cpp            # Implementation of Ticket methods & operators
│   ├── Airline.cpp           # Core operations, STL logic & file persistence
│   ├── Menu.cpp              # Interactive menu & input validation
│   └── main.cpp              # Application entry point
├── data/
│   ├── flights.txt           # Persistent flight records
│   ├── passengers.txt        # Persistent passenger records
│   └── tickets.txt          # Persistent ticket records
├── Makefile                  # Build script for Linux/macOS/GCC
├── build.bat                 # Windows batch build script
├── README.md                 # User guide and build instructions
├── DESIGN_REPORT.md          # 2-3 page OOP design rationale & test report
└── UML_DIAGRAM.md            # Complete UML class structure and relationships
```

---

## How to Build and Run

### On Windows (using GCC / MinGW)

1. Open PowerShell or Command Prompt in `d:\PBL Airline`.
2. Run the build script:
   ```cmd
   build.bat
   ```
   Or compile directly:
   ```cmd
   g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o skylink_app.exe
   ```
3. Run the application:
   ```cmd
   .\skylink_app.exe
   ```

### On Linux / macOS (using Makefile)

1. Open terminal in the project directory.
2. Run `make`:
   ```bash
   make
   ```
3. Run the application:
   ```bash
   ./skylink_app
   ```
4. Clean build artifacts:
   ```bash
   make clean
   ```

---

## Sample Pre-loaded Data

The application comes with sample data pre-loaded:
- **10 Flights**: 4 Domestic, 4 International, 2 Charter flights.
- **8 Passengers**: 3 Economy, 3 Business, 2 First Class.
- **6 Pre-booked Tickets**: Allowing immediate testing of reports and cancellations.
