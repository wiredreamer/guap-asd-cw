#include "list.h"

#include <iostream>

TicketSkipList::TicketSkipList() : head_(nullptr), size_(0), levelCount_(1) {
    rebuildFrom({});
}

TicketSkipList::~TicketSkipList() {
    destroyNodes();
}

void TicketSkipList::destroyNodes() {
    if (!head_)
        return;
    Node* p = head_->forward.empty() ? nullptr : head_->forward[0];
    while (p) {
        Node* next = p->forward.empty() ? nullptr : p->forward[0];
        delete p;
        p = next;
    }
    delete head_;
    head_ = nullptr;
}

void TicketSkipList::mergeSort(std::vector<Ticket>& a, int lo, int hi, std::vector<Ticket>& buf) {
    if (hi - lo <= 1)
        return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, lo, mid, buf);
    mergeSort(a, mid, hi, buf);

    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (a[i].ticketNumber <= a[j].ticketNumber)
            buf[k++] = a[i++];
        else
            buf[k++] = a[j++];
    }
    while (i < mid)
        buf[k++] = a[i++];
    while (j < hi)
        buf[k++] = a[j++];
    for (int t = lo; t < hi; ++t)
        a[t] = buf[t];
}

void TicketSkipList::mergeSort(std::vector<Ticket>& a) {
    if (a.size() < 2)
        return;
    std::vector<Ticket> buf(a.size());
    mergeSort(a, 0, static_cast<int>(a.size()), buf);
}

void TicketSkipList::rebuildFrom(std::vector<Ticket> its) {
    destroyNodes();
    mergeSort(its);
    const int n = static_cast<int>(its.size());
    size_       = n;

    int lv = 1;
    while ((1 << lv) <= n)
        ++lv;
    levelCount_ = lv;

    head_ = new Node;
    head_->forward.assign(levelCount_, nullptr);
    if (n == 0)
        return;

    std::vector<Node*> nodes(n);
    for (int i = 0; i < n; ++i) {
        int h = 1;
        while (h < levelCount_ && (i % (1 << h)) == 0)
            ++h;
        Node* nd = new Node;
        nd->data = its[i];
        nd->forward.assign(h, nullptr);
        nodes[i] = nd;
    }

    for (int l = 0; l < levelCount_; ++l) {
        Node* prev = head_;
        for (int i = 0; i < n; ++i) {
            if (static_cast<int>(nodes[i]->forward.size()) > l) {
                prev->forward[l] = nodes[i];
                prev             = nodes[i];
            }
        }
    }
}

bool TicketSkipList::containsTicketNumber(const std::string& key) const {
    Node* x = head_;
    for (int l = levelCount_ - 1; l >= 0; --l) {
        while (x->forward[l] && x->forward[l]->data.ticketNumber < key)
            x = x->forward[l];
    }
    Node* next = x->forward.empty() ? nullptr : x->forward[0];
    return next && next->data.ticketNumber == key;
}

bool TicketSkipList::insert(const Ticket& t) {
    if (containsTicketNumber(t.ticketNumber))
        return false;
    std::vector<Ticket> its = items();
    its.push_back(t);
    rebuildFrom(std::move(its));
    return true;
}

bool TicketSkipList::removeByTicketNumber(const std::string& key) {
    if (!containsTicketNumber(key))
        return false;
    std::vector<Ticket> kept;
    for (const Ticket& t : items())
        if (t.ticketNumber != key)
            kept.push_back(t);
    rebuildFrom(std::move(kept));
    return true;
}

int TicketSkipList::removeByPassenger(const std::string& passportNumber) {
    std::vector<Ticket> kept;
    int removed = 0;
    for (const Ticket& t : items()) {
        if (t.passportNumber == passportNumber)
            ++removed;
        else
            kept.push_back(t);
    }
    if (removed > 0)
        rebuildFrom(std::move(kept));
    return removed;
}

int TicketSkipList::removeByFlight(const std::string& flightNumber) {
    std::vector<Ticket> kept;
    int removed = 0;
    for (const Ticket& t : items()) {
        if (t.flightNumber == flightNumber)
            ++removed;
        else
            kept.push_back(t);
    }
    if (removed > 0)
        rebuildFrom(std::move(kept));
    return removed;
}

void TicketSkipList::clear() {
    rebuildFrom({});
}

std::vector<Ticket> TicketSkipList::items() const {
    std::vector<Ticket> result;
    result.reserve(size_);
    for (Node* p = head_->forward[0]; p; p = p->forward[0])
        result.push_back(p->data);
    return result;
}

void TicketSkipList::dump() const {
    std::cout << "  Слоёный список билетов (уровней: " << levelCount_ << ", билетов: " << size_
              << "):\n";
    for (int l = levelCount_ - 1; l >= 0; --l) {
        std::cout << "    Уровень " << l << ": ";
        for (Node* p = head_->forward[l]; p; p = p->forward[l])
            std::cout << p->data.ticketNumber << " ";
        std::cout << "\n";
    }
}
