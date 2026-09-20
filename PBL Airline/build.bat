@echo off
echo Building SkyLink Airways C++ Project...
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Flight.cpp src/Passenger.cpp src/Ticket.cpp src/Airline.cpp src/Menu.cpp -o skylink_app.exe
if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Run skylink_app.exe to start the application.
) else (
    echo Build Failed! Please check compilation errors above.
)
