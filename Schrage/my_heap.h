#pragma once

#include "heap.h"

class MyHeap : public Heap {
public:
    void Make(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
    void Push(std::vector<RPQ>& vec, const RPQ& rpq, const std::function<bool(RPQ, RPQ)>& comp) override;
    RPQ Pop(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
    void Sort(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
};

void MyHeap::Make(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)> &comp) {
    for(unsigned int i = 1; i < vec.size(); i++)
        for (auto k = i; k > 0  && comp(vec[(k-1)/2], vec[k]); k = --k / 2)
            std::swap(vec[k], vec[(k-1)/2]);
}

void MyHeap::Push(std::vector<RPQ>& vec, const RPQ& rpq, const std::function<bool(RPQ, RPQ)>& comp) {
    vec.emplace_back(rpq);
    for (unsigned int i = vec.size() - 1; i > 0 && comp(vec[(i-1)/2], vec[i]); i = --i / 2)
        std::swap(vec[i], vec[(i-1)/2]);
}

RPQ MyHeap::Pop(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) {
    std::swap(vec.front(), vec.back());
    auto result = vec.back();
    vec.pop_back();
    unsigned int k = 0;
    for (unsigned int i = 1; i < vec.size(); i = 2 * i + 1) {
        if (i + 1 < vec.size() && comp(vec[i], vec[i+1])) i++;
        if (comp(vec[i], vec[k])) break;
        std::swap(vec[k], vec[i]);
        k = i;
    }

    return result;
}

void MyHeap::Sort(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) {
    std::vector<RPQ> result;
    Make(vec, comp);
    while (!vec.empty())
        result.emplace_back(Pop(vec, comp));
    std::reverse(result.begin(), result.end());
    vec = std::move(result);
}
