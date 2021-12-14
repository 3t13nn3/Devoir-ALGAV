#include "BTrie.hpp"

#include <iostream>
BTrie::BTrie() {
    for (int i = 0; i < 2; ++i) {
        _number[i] = nullptr;
    }
}

void BTrie::CheckAndInsert(const std::shared_ptr<Node> &n) {
    // conserve the root
    auto curr = this;

    for (size_t i = 0; i < n->_value.size(); i++) {
        int idx = n->_value[i] - 48;
        // adding the value if we haven't it in our BTrie

        if (curr->_number[idx] == nullptr) {
            curr->_number[idx] = new BTrie();
        }
        // moving to the next BTrie for the next number
        curr = curr->_number[idx];
    }

    if (curr->_ptr != n) {
        // std::cout << n->_value << std::endl;
        curr->_ptr = n;
    }
}

std::shared_ptr<Node> BTrie::Find(std::string key) {
    if (_number == nullptr) {
        return nullptr;
    }

    auto curr = this;

    for (size_t i = 0; i < key.size(); i++) {
        int idx = key[i] - 48;

        // browse
        curr = curr->_number[idx];

        // nullptr
        if (curr == nullptr) {
            return nullptr;
        }
    }
    return curr->_ptr;
}

void BTrie::getAllValues(BTrie *&t) {
    if (t == nullptr) {
        return;
    }
    if (t->_ptr != nullptr) {
        std::cout << t->_ptr->_value << std::endl;
        // return;
    }

    for (int i = 0; i < 2; ++i) {
        if (t->_number[i] != nullptr) {
            getAllValues(t->_number[i]);
        }
    }
}

void BTrie::Display() {
    for (int i = 0; i < 2; ++i) {
        if (_number[i] != nullptr) {
            getAllValues(_number[i]);
        }
    }
}

void BTrie::Remove(std::string &key) {
    if (_number == nullptr) {
        return;
    }

    auto curr = this;

    for (size_t i = 0; i < key.size(); i++) {
        int idx = key[i] - 48;
        // browse
        curr = curr->_number[idx];
    }
    curr->_ptr = nullptr;
}

void BTrie::RemoveAll() {
    for (int i = 0; i < 2; ++i) {
        if (_number[i] != nullptr) {
            removeAllAux(_number[i]);
        }
    }
}

void BTrie::removeAllAux(BTrie *&t) {
    if (t == nullptr) return;

    // recursive case (go to end of trie)
    for (int i(0); i < 2; i++) {
        removeAllAux(t->_number[i]);
    }

    delete t;
}
