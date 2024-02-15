//
// Created by Tuomas Katajisto on 15.2.2024.
//

#ifndef VKBOOTPROJ_DELQ_H
#define VKBOOTPROJ_DELQ_H

#include "deque"

using namespace std;

struct DelQ
{
    std::deque<std::function<void()>> delq;

    void push(std::function<void()>&& function) {
        delq.push_back(function);
    }

    void cleanupAll() {
        for (auto it = delq.rbegin(); it != delq.rend(); it++) {
            (*it)();
        }
        delq.clear();
    }
};


#endif //VKBOOTPROJ_DELQ_H
