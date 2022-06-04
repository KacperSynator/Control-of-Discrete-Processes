#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <limits>
#include <memory>

#include "stl_heap.h"
#include "heap.h"
#include "rpq.h"
#include "my_heap.h"


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

int main()
{
    // Start timer, time displayed when main ends
    Timer timer;

    // open file
    std::fstream fs("../schr.data.txt");
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

//        // STL heap
        std::unique_ptr<Heap> stl_heap( new STLHeap() );
        std::cout << "Preemption Schrage: " << prmt_schrage(data, stl_heap) << "\n"
                  << "Schrage: " << schrage(data, stl_heap) << "\nOrder: ";

        // My Heap
        std::unique_ptr<Heap> my_heap( new MyHeap() );
        std::cout << "Preemption Schrage: " << prmt_schrage(data, my_heap) << "\n"
                  << "Schrage: " << schrage(data, my_heap) << "\nOrder: ";

//        for (auto &elem: data)
//            std::cout << elem.id << " ";
//        std::cout << "\n";
    }

    return 0;
}
