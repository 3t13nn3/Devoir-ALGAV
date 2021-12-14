#ifndef __BTRIE_HPP__
#define __BTRIE_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "Node.h"

class BTrie {
   public:
    BTrie();

    void CheckAndInsert(const std::shared_ptr<Node> &n);

    std::shared_ptr<Node> Find(std::string key);

    void Display();

    void Remove(std::string &key);

    void RemoveAll();

   private:
    BTrie *_number[2];
    std::shared_ptr<Node> _ptr;

    void getAllValues(BTrie *&t);

    void removeAllAux(BTrie *&t);
};

#endif