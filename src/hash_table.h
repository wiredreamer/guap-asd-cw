#pragma once

#include <string>
#include <vector>

#include "model.h"

class HashTable {
public:
    explicit HashTable(int capacity = 101);

    bool insert(const Passenger& p);
    const Passenger* find(const std::string& passportNumber) const;
    bool remove(const std::string& passportNumber);
    void clear();

    bool empty() const {
        return count_ == 0;
    }
    int size() const {
        return count_;
    }
    int capacity() const {
        return capacity_;
    }

    std::vector<Passenger> items() const;
    void dump() const;
    int h1(const std::string& key) const;

private:
    enum class State { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        Passenger data;
        State state = State::EMPTY;
    };

    static unsigned long long encode(const std::string& key);

    std::vector<Slot> table_;
    int capacity_;
    int count_;
};
