#pragma once

#include <string>
#include <utility>
#include <vector>

#include "avl_tree.h"
#include "hash_table.h"
#include "list.h"
#include "model.h"

enum class SellResult {
    Ok,
    PassengerNotFound,
    FlightNotFound,
    NoSeats,
    DuplicateTicket,
};

class Airline {
public:
    bool registerPassenger(const Passenger& p);
    bool deletePassenger(const std::string& passport, int& removedTickets);
    void clearPassengers();
    const HashTable& passengers() const {
        return passengers_;
    }

    const Passenger* findPassenger(
        const std::string& passport, std::vector<std::string>& flightNumbers
    ) const;

    std::vector<Passenger> findPassengersByFio(const std::string& fio) const;

    bool addFlight(const Flight& f);
    bool deleteFlight(const std::string& flightNumber, int& removedTickets);
    void clearFlights();
    const AvlTree& flights() const {
        return flights_;
    }

    const Flight* findFlight(
        const std::string& flightNumber, std::vector<std::pair<std::string, std::string>>& buyers
    ) const;

    std::vector<Flight> findFlightsByArrivalFragment(const std::string& fragment) const;

    SellResult sellTicket(const Ticket& t);
    bool returnTicket(const std::string& ticketNumber);
    const TicketSkipList& tickets() const {
        return tickets_;
    }

private:
    HashTable passengers_;
    AvlTree flights_;
    TicketSkipList tickets_;
};
