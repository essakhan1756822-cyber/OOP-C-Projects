# SkyLink Airways - OOP C++ Design & Implementation Report

**Course**: Object-Oriented Programming (C++)  
**Project**: Airline Reservation & Flight Management System  
**Author**: Student Software Engineer  
**Standard**: C++17  

---

## 1. Executive Summary & Architecture Overview

The **SkyLink Airways Flight Management System** was developed to replace spreadsheet-based airline operations with a robust, memory-safe console application built in modern C++17. The design strictly adheres to object-oriented principles, utilizing clean separation of concerns:

- **Entity Model Layer** (`Flight`, `Passenger`, `Ticket`): Defines the domain model, business constraints, pricing rules, and refund policies.
- **Generic Utility Layer** (`SearchUtils`, `Exceptions`): Provides reusable template functions, filtering engines, and custom exception handling.
- **Service/Aggregation Layer** (`Airline`): Manages object lifetimes via `std::shared_ptr`, handles file I/O persistence, and executes operational reports.
- **Presentation Layer** (`Menu`): Handles interactive console I/O, user navigation, and robust input validation.

---

## 2. Detailed Justification of OOP Features Used

### 2.1 Four Pillars of OOP

#### 1. Abstraction
- **Implementation**: The `Flight` class is an abstract base class featuring pure virtual functions (`calculateBaseFare()`, `displayDetails()`, `getTypeName()`, `serialize()`).
- **Rationale**: External modules do not need to know the specific pricing math for domestic tax, international customs, or charter contracts. They interact uniformly via the `Flight` interface.

#### 2. Encapsulation
- **Implementation**: Data fields (`flightNumber`, `passengerId`, `availableSeats`, `loyaltyPoints`) are declared `private` or `protected`. State modifications occur exclusively through validated getters/setters (`bookSeat()`, `releaseSeat()`, `addLoyaltyPoints()`).
- **Rationale**: Prevents accidental corruption of sensitive state (e.g., negative available seats or direct modification of ticket status).

#### 3. Inheritance
- **Implementation**:
  - `Flight` $\rightarrow$ `DomesticFlight`, `InternationalFlight`, `CharterFlight`.
  - `Passenger` $\rightarrow$ `EconomyPassenger`, `BusinessPassenger`, `FirstClassPassenger`.
- **Rationale**: Models the real-world domain where distinct flight types and passenger classes share core properties but introduce specialized behaviors and pricing attributes.

#### 4. Polymorphism
- **Implementation**: Virtual functions enable dynamic dispatch at runtime:
  - **Fare Calculation**: `flight->calculateBaseFare()` computes class-specific pricing based on tax rates, visa levies, or flat charter splits.
  - **Refund Calculation**: `passenger->getRefundPercentage(hours)` dynamically evaluates refund eligibility based on passenger class and time prior to departure.

---

### 2.2 Memory Safety & Resource Management

- **No Raw Pointer Memory Leaks**: The system uses `std::shared_ptr<Flight>` and `std::shared_ptr<Passenger>` stored in STL containers. Dynamic allocations (`std::make_shared`) are automatically reference-counted and freed when objects leave scope.
- **Virtual Destructors**: Both base classes `Flight` and `Passenger` define `virtual ~Flight() = default;` and `virtual ~Passenger() = default;` to ensure proper destruction of derived instances.

---

### 2.3 Operator Overloading & Templates

- **Operator Overloading**:
  - `operator<<` for `Flight` and `Ticket`: Formats entity outputs cleanly for stream operations (`std::cout << *flight`).
  - `operator==` for `Ticket`: Enables direct ID comparison between ticket instances.
- **Templates**:
  - Generic function template `searchItems(container, predicate)` filters any standard container matching a lambda condition.
  - Generic class template `SearchEngine<T>` provides filter algorithms for flight search queries by date or route.

---

### 2.4 Custom Exception Handling

Custom exception classes inheriting from `std::runtime_error` prevent system crashes on invalid operations:
1. `FlightFullException`: Thrown when trying to book a seat on a flight with 0 available seats.
2. `InvalidCancellationException`: Thrown when attempting to cancel an already cancelled ticket or invalid ticket ID.
3. `DuplicateBookingException`: Thrown when a passenger attempts to double-book the same flight.
4. `PassengerNotFoundException` & `FlightNotFoundException`: Thrown when invalid IDs are entered.

---

## 3. Data Persistence Strategy

The system utilizes pipe-delimited (`|`) flat text files stored in `data/`:
- `flights.txt`: Serializes type tag (`DOMESTIC`, `INTERNATIONAL`, `CHARTER`), basic flight info, and type-specific attributes.
- `passengers.txt`: Serializes passenger category, personal details, and accumulated loyalty points.
- `tickets.txt`: Serializes ticket ID, passenger ID, flight number, seat number, fare paid, status, and booking date.

On application startup, `Airline::loadData()` parses these files, reconstituting polymorphic object instances automatically. If no data file exists, sample data is automatically generated.

---

## 4. Verification & Test Case Suite

| Test ID | Scenario Description | Expected Outcome | Result |
|---|---|---|---|
| **TC-01** | Book ticket for valid passenger and available flight | Ticket created (`TICK1001`), available seats decremented, loyalty points awarded | **PASSED** |
| **TC-02** | Book ticket on fully booked flight | Throws `FlightFullException`, displays user error | **PASSED** |
| **TC-03** | Double booking by same passenger on same flight | Throws `DuplicateBookingException` | **PASSED** |
| **TC-04** | Cancel ticket > 48h before departure (Economy) | 85% refund computed, ticket status set to `CANCELLED`, seat released | **PASSED** |
| **TC-05** | Cancel ticket < 24h before departure (Economy) | 0% refund computed, seat released | **PASSED** |
| **TC-06** | Cancel ticket > 24h before departure (First Class) | 100% full refund computed | **PASSED** |
| **TC-07** | Cancel already cancelled ticket | Throws `InvalidCancellationException` | **PASSED** |
| **TC-08** | Generate occupancy report | Flights listed in descending order of occupancy % using `std::sort` | **PASSED** |
| **TC-09** | Save and restart application | State restored identically from `data/*.txt` files | **PASSED** |
| **TC-10** | Non-numeric input in console menu | Input buffer cleared, prompts user to re-enter without infinite loop | **PASSED** |

---

## 5. Viva Defense Quick Reference

- **Q: Why use `std::shared_ptr` instead of raw pointers (`Flight*`)?**  
  *A*: `std::shared_ptr` automates memory management using reference counting, preventing dangling pointers and memory leaks without requiring manual `delete` calls.
- **Q: How does dynamic polymorphism work in your fare calculation?**  
  *A*: `Flight` defines a pure virtual function `calculateBaseFare()`. When invoked through a base pointer `Flight*` or `std::shared_ptr<Flight>`, C++ uses the Virtual Method Table (VTABLE) to execute the derived implementation at runtime.
- **Q: How did you implement templates?**  
  *A*: We created `SearchUtils.h` with generic function template `searchItems` and class template `SearchEngine<T>`, allowing generic predicate filtering across any standard container.
