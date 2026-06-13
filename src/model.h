#pragma once

#include <string>

struct Passenger {
    std::string passportNumber;
    std::string passportIssue;
    std::string fio;
    std::string birthDate;
};

struct Flight {
    std::string flightNumber;
    std::string airline;
    std::string departureAirport;
    std::string arrivalAirport;
    std::string departureDate;
    std::string departureTime;
    int seatsTotal{};
    int seatsFree{};
};

struct Ticket {
    std::string passportNumber;
    std::string flightNumber;
    std::string ticketNumber;
};

bool isValidPassport(const std::string& s);
bool isValidFlightNumber(const std::string& s);
bool isValidTicketNumber(const std::string& s);
