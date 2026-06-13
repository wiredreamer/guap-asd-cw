#pragma once

#include <string>
#include <vector>

#include "model.h"

class TicketSkipList {
public:
    TicketSkipList();
    ~TicketSkipList();

    TicketSkipList(const TicketSkipList&)            = delete;
    TicketSkipList& operator=(const TicketSkipList&) = delete;

    bool insert(const Ticket& t);
    bool containsTicketNumber(const std::string& ticketNumber) const;
    bool removeByTicketNumber(const std::string& ticketNumber);
    int removeByPassenger(const std::string& passportNumber);
    int removeByFlight(const std::string& flightNumber);
    void clear();

    bool empty() const {
        return size_ == 0;
    }
    int size() const {
        return size_;
    }
    int levels() const {
        return levelCount_;
    }

    std::vector<Ticket> items() const;
    void dump() const;

private:
    struct Node {
        Ticket data;
        std::vector<Node*> forward;
    };

    Node* head_;
    int size_;
    int levelCount_;

    static void mergeSort(std::vector<Ticket>& a);
    static void mergeSort(std::vector<Ticket>& a, int lo, int hi, std::vector<Ticket>& buf);

    void rebuildFrom(std::vector<Ticket> items);
    void destroyNodes();
};
