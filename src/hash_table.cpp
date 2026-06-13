#include "hash_table.h"

#include <iostream>

HashTable::HashTable(int capacity) : table_(capacity), capacity_(capacity), count_(0) {}

unsigned long long HashTable::encode(const std::string& key) {
    unsigned long long code = 0;
    for (unsigned char ch : key)
        code = code * 31 + ch;
    return code;
}

int HashTable::h1(const std::string& key) const {
    return static_cast<int>(encode(key) % capacity_);
}

bool HashTable::insert(const Passenger& p) {
    const std::string& key = p.passportNumber;
    const int base         = h1(key);
    int firstFree          = -1;

    for (int i = 0; i < capacity_; ++i) {
        int idx       = static_cast<int>((static_cast<long long>(base) + i * i) % capacity_);
        const Slot& s = table_[idx];

        if (s.state == State::OCCUPIED) {
            if (s.data.passportNumber == key)
                return false;
        } else {
            if (firstFree == -1)
                firstFree = idx;
            if (s.state == State::EMPTY)
                break;
        }
    }

    if (firstFree == -1)
        return false;

    table_[firstFree].data  = p;
    table_[firstFree].state = State::OCCUPIED;
    ++count_;
    return true;
}

const Passenger* HashTable::find(const std::string& passportNumber) const {
    const int base = h1(passportNumber);
    for (int i = 0; i < capacity_; ++i) {
        int idx       = static_cast<int>((static_cast<long long>(base) + i * i) % capacity_);
        const Slot& s = table_[idx];

        if (s.state == State::EMPTY)
            return nullptr;
        if (s.state == State::OCCUPIED && s.data.passportNumber == passportNumber)
            return &s.data;
    }
    return nullptr;
}

bool HashTable::remove(const std::string& passportNumber) {
    const int base = h1(passportNumber);
    for (int i = 0; i < capacity_; ++i) {
        int idx = static_cast<int>((static_cast<long long>(base) + i * i) % capacity_);
        Slot& s = table_[idx];

        if (s.state == State::EMPTY)
            return false;
        if (s.state == State::OCCUPIED && s.data.passportNumber == passportNumber) {
            s.state = State::DELETED;
            s.data  = Passenger{};
            --count_;
            return true;
        }
    }
    return false;
}

void HashTable::clear() {
    for (Slot& s : table_) {
        s.state = State::EMPTY;
        s.data  = Passenger{};
    }
    count_ = 0;
}

std::vector<Passenger> HashTable::items() const {
    std::vector<Passenger> result;
    result.reserve(count_);
    for (const Slot& s : table_)
        if (s.state == State::OCCUPIED)
            result.push_back(s.data);
    return result;
}

void HashTable::dump() const {
    std::cout << "  Хеш-таблица пассажиров (размер m=" << capacity_ << ", занято " << count_
              << "):\n";
    for (int i = 0; i < capacity_; ++i) {
        const Slot& s = table_[i];
        if (s.state == State::OCCUPIED) {
            std::cout << "    [" << i << "] " << s.data.passportNumber
                      << "  (h1=" << h1(s.data.passportNumber) << ")  " << s.data.fio << "\n";
        } else if (s.state == State::DELETED) {
            std::cout << "    [" << i << "] <удалено>\n";
        }
    }
}
