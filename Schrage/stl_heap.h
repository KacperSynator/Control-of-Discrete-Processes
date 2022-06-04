#pragma once

#include <algorithm>
#include <vector>

#include "heap.h"
#include "rpq.h"

class STLHeap : public Heap {
public:
    void Make(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
    void Push(std::vector<RPQ>& vec, const RPQ& rpq, const std::function<bool(RPQ, RPQ)>& comp) override;
    RPQ Pop(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
    void Sort(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) override;
};

void STLHeap::Make(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)> &comp) {
    std::make_heap(vec.begin(), vec.end(), comp);
}

void STLHeap::Push(std::vector<RPQ>& vec, const RPQ& rpq, const std::function<bool(RPQ, RPQ)>& comp) {
    vec.emplace_back(rpq);
    std::push_heap(vec.begin(), vec.end(), comp);
}

RPQ STLHeap::Pop(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) {
    std::pop_heap(vec.begin(), vec.end(), comp);
    auto result = vec.back();
    vec.pop_back();
    return result;
}

void STLHeap::Sort(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)> &comp) {
    std::vector<RPQ> result;
    Make(vec, comp);
    while (!vec.empty())
        result.emplace_back(Pop(vec, comp));
    std::reverse(result.begin(), result.end());
    vec = std::move(result);
}
