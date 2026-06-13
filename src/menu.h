#pragma once

#include "airline.h"

class Menu {
public:
    void run();

private:
    void registerPassenger();
    void deletePassenger();
    void viewPassengers() const;
    void findPassengerByPassport() const;
    void findPassengerByFio() const;

    void addFlight();
    void deleteFlight();
    void viewFlights() const;
    void showPostorder() const;
    void findFlightByNumber() const;
    void findFlightByArrival() const;

    void sellTicket();
    void returnTicket();
    void viewTickets() const;

    void loadDemo();

    Airline airline_;
};
