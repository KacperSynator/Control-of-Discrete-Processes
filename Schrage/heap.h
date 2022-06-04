#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "rpq.h"

class Heap {
  public:
    virtual ~Heap() = default;
    virtual void Make(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) = 0;
    virtual void Push(std::vector<RPQ>& vec, const RPQ& rpq, const std::function<bool(RPQ, RPQ)>& comp) = 0;
    virtual RPQ Pop(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) = 0;
    virtual void Sort(std::vector<RPQ>& vec, const std::function<bool(RPQ, RPQ)>& comp) = 0;
};
