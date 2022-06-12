#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <limits>

#include "stl_heap.h"
#include "rpq.h"


/*! Simple Timer */
namespace ch = std::chrono;
class Timer{
    ch::high_resolution_clock::time_point start;
    ch::duration<float> duration;
public:
    Timer()
    {
        start = ch::high_resolution_clock::now();
    }
    ~Timer()
    {
        duration = ch::high_resolution_clock::now() - start;
        std::cout << "Time: " << duration.count() << "s" << "\n";
    }
};


/*! Schrage algorithm */
int schrage(std::vector<RPQ>& data, std::unique_ptr<Heap>& heap)
{
    heap->Sort(data, RPQ::compare_r);
    std::vector<RPQ> copy = std::move(data);
    std::vector<RPQ> tmp_vec;

    int t=0, c=0;
    while(!tmp_vec.empty() || !copy.empty())
    {
        // min r value always at the beginning of copy vector
        while(!copy.empty() && copy.front().r <= t)
        {
            heap->Push(tmp_vec, copy.front(), RPQ::compare_q);
            copy.erase(copy.begin());
        }
        if(tmp_vec.empty())
        {
            t = copy.front().r;
            continue;
        }
        data.emplace_back(heap->Pop(tmp_vec, RPQ::compare_q));
        t += data.back().p;
        c = std::max(c, t + data.back().q);
    }
    return  c;
}

/*! Schrage algorithm with process dividing (preemption) */
int prmt_schrage(std::vector<RPQ> data, std::unique_ptr<Heap>& heap)
{
    heap->Sort(data, RPQ::compare_r);
    std::vector<RPQ> copy = std::move(data);
    std::vector<RPQ> tmp_vec;

    RPQ l{0, 0, 0, std::numeric_limits<int>::max()};
    int t=0, c=0;
    while(!tmp_vec.empty() || !copy.empty())
    {
        // min r value at the beginning of copy vector
        while(!copy.empty() && copy.front().r <= t)
        {
            heap->Push(tmp_vec, copy.front(), RPQ::compare_q);
            copy.erase(copy.begin());
            if(tmp_vec.front().q > l.q)
            {
                l.p = t - tmp_vec.front().r;
                t = tmp_vec.front().r;
                if(l.p > 0) heap->Push(tmp_vec, l, RPQ::compare_q);
            }
        }
        if(tmp_vec.empty())
        {
            t = copy.front().r;
            continue;
        }
        l = heap->Pop(tmp_vec, RPQ::compare_q);
        t += l.p;
        c = std::max(c, t + l.q);
    }
    return  c;
}

/*! find block (a, b) and reference c if c exists return true otherwise false */
bool find_ref_c(std::vector<RPQ>& data, int cmax, unsigned long long& b, unsigned long long& c)
{
    auto a = std::numeric_limits<unsigned long long>::max();
    c = std::numeric_limits<unsigned long long>::max();
    // find b
    for(auto j = data.size() - 1; j >= 0; j--)
    {
        int tmp = 0;
        for(auto i = 0; i <= j; i++) tmp = std::max(tmp, data[i].r) + data[i].p;
        if(cmax == tmp + data[j].q)
        {
            b = j;
            break;
        }
    }
    // find a
    for(auto j = 0; j < data.size(); j++)
    {
        int sum = 0;
        for(auto i = j; i <= b; i++) sum += data[i].p;
        if(cmax == data[j].r + data[b].q + sum)
        {
            a = j;
            break;
        }
    }
    //find c if possible
    for(auto j = b; j >= a; j--)
    {
        if(data[j].q < data[b].q)
        {
            c = j;
            break;
        }
    }
    return c != std::numeric_limits<unsigned long long>::max();
}


/*! Carlier algorithm */
void carlier(std::vector<RPQ>& data, int& ub, std::vector<RPQ>& result, std::unique_ptr<Heap>& heap)
{
    static int depth = 0;
    if(depth > 100) return;
    depth++;

    int u = schrage(data, heap);
    if(u < ub)
    {
        result = data;
        ub = u;
    }

    unsigned long long c, b;
    if(!find_ref_c(data, u, b, c)) return;

    RPQ _rpq{0, std::numeric_limits<int>::max(), 0, std::numeric_limits<int>::max()};
    for(auto i = c + 1; i <= b; i++)
    {
        _rpq.r = std::min(_rpq.r, data[i].r);
        _rpq.q = std::min(_rpq.q, data[i].q);
        _rpq.p += data[i].p;
    }
    int r_copy = data[c].r;
    int r_copy_id = data[c].id;
    data[c].r = std::max(data[c].r, _rpq.r + _rpq.p);
    if(prmt_schrage(data, heap) < ub) carlier(data, ub, result, heap);
    for(auto& elem: data)
    {
        if(elem.id == r_copy_id)
        {
            elem.r = r_copy;
            break;
        }
    }

    int q_copy = data[c].q;
    int q_copy_id = data[c].id;
    data[c].q = std::max(data[c].q, _rpq.q + _rpq.p);
    if(prmt_schrage(data, heap) < ub) carlier(data, ub, result, heap);
    for(auto& elem: data)
    {
        if (elem.id == q_copy_id)
        {
            elem.q = q_copy;
            break;
        }
    }
    depth--;
}

/*! help function to calculate Cmax of data from Carlier algorithm needs data id sorted (id_ordered_data) */
int calculate_cmax(std::vector<RPQ>& data, std::vector<RPQ>& id_ordered_data)
{
    int c=0, t=0;
    for(auto& elem: data)
    {
        auto _data = id_ordered_data[elem.id - 1];
        t = std::max(t, _data.r) + _data.p;
        c = std::max(c, t + _data.q);
    }
    return c;
}

int main()
{
    // Start timer, time displayed when main ends
    Timer timer;

    // open file
    std::fstream fs("../carl.data.txt");
    if(!fs.is_open()) throw std::invalid_argument("File not found");
    std::string str;

    for(int i = 0; i <= 8; i++)
    {
        std::string name = "data.00" + std::to_string(i) + ":";

        // skip to data
        std::cout << "----------------\nData: " << name << "\n";
        while (str != name) fs >> str;

        // read data
        std::vector<RPQ> data;
        int N; fs >> N;
        for (int i = 0; i < N; i++)
        {
            RPQ tmp{};
            tmp.id = i + 1;
            fs >> tmp.r >> tmp.p >> tmp.q;
            data.emplace_back(tmp);
        }
        // run algorithm and print results
        std::vector<RPQ> result;
        std::unique_ptr<Heap> heap = std::make_unique<STLHeap>();
        int ub = schrage(data, heap);
        carlier(data, ub, result, heap);
        std::stable_sort(data.begin(), data.end(), [](RPQ a, RPQ b){return a.id < b.id;});
        int cmax = calculate_cmax(result, data);
        std::cout << "Cmax: " << cmax << "\nOrder: ";
        for (auto &elem: result)
            std::cout << elem.id - 1 << " ";
        std::cout << "\n";
    }

    return 0;
}
