#include "airline.h"

#include "search.h"

bool Airline::registerPassenger(const Passenger& p) {
    return passengers_.insert(p);
}

bool Airline::deletePassenger(const std::string& passport, int& removedTickets) {
    if (!passengers_.find(passport)) {
        removedTickets = 0;
        return false;
    }
    for (const Ticket& t : tickets_.items()) {
        if (t.passportNumber == passport) {
            Flight* f = flights_.findMutable(t.flightNumber);
            if (f && f->seatsFree < f->seatsTotal)
                ++f->seatsFree;
        }
    }
    removedTickets = tickets_.removeByPassenger(passport);
    passengers_.remove(passport);
    return true;
}

void Airline::clearPassengers() {
    passengers_.clear();
}

const Passenger* Airline::findPassenger(
    const std::string& passport, std::vector<std::string>& flightNumbers
) const {
    const Passenger* p = passengers_.find(passport);
    if (!p)
        return nullptr;

    flightNumbers.clear();
    for (const Ticket& t : tickets_.items()) {
        if (t.passportNumber == passport)
            flightNumbers.push_back(t.flightNumber);
    }
    return p;
}

std::vector<Passenger> Airline::findPassengersByFio(const std::string& fio) const {
    std::vector<Passenger> result;
    for (const Passenger& p : passengers_.items()) {
        if (p.fio == fio)
            result.push_back(p);
    }
    return result;
}

bool Airline::addFlight(const Flight& f) {
    return flights_.insert(f);
}

bool Airline::deleteFlight(const std::string& flightNumber, int& removedTickets) {
    if (!flights_.find(flightNumber)) {
        removedTickets = 0;
        return false;
    }
    removedTickets = tickets_.removeByFlight(flightNumber);
    flights_.remove(flightNumber);
    return true;
}

void Airline::clearFlights() {
    flights_.clear();
}

const Flight* Airline::findFlight(
    const std::string& flightNumber, std::vector<std::pair<std::string, std::string>>& buyers
) const {
    const Flight* f = flights_.find(flightNumber);
    if (!f)
        return nullptr;

    buyers.clear();
    for (const Ticket& t : tickets_.items()) {
        if (t.flightNumber == flightNumber) {
            const Passenger* p = passengers_.find(t.passportNumber);
            std::string fio    = p ? p->fio : std::string("<нет данных>");
            buyers.emplace_back(fio, t.passportNumber);
        }
    }
    return f;
}

std::vector<Flight> Airline::findFlightsByArrivalFragment(const std::string& fragment) const {
    std::vector<Flight> result;
    for (const Flight& f : flights_.postorder()) {
        if (boyerMooreSearch(f.arrivalAirport, fragment))
            result.push_back(f);
    }
    return result;
}

SellResult Airline::sellTicket(const Ticket& t) {
    if (!passengers_.find(t.passportNumber))
        return SellResult::PassengerNotFound;

    Flight* f = flights_.findMutable(t.flightNumber);
    if (!f)
        return SellResult::FlightNotFound;
    if (tickets_.containsTicketNumber(t.ticketNumber))
        return SellResult::DuplicateTicket;
    if (f->seatsFree <= 0)
        return SellResult::NoSeats;

    tickets_.insert(t);
    --f->seatsFree;
    return SellResult::Ok;
}

bool Airline::returnTicket(const std::string& ticketNumber) {
    for (const Ticket& t : tickets_.items()) {
        if (t.ticketNumber == ticketNumber) {
            Flight* f = flights_.findMutable(t.flightNumber);
            if (f && f->seatsFree < f->seatsTotal)
                ++f->seatsFree;
            tickets_.removeByTicketNumber(ticketNumber);
            return true;
        }
    }
    return false;
}
