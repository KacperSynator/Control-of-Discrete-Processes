#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <string>


/*! struct for holding data about processes */
struct WiTi {
    int id; /*!< process id */
    int p;  /*!< processing time */
    int w;  /*!< weight (penalty for every time unit of delay) */
    int d;  /*!< desired ending time */
};

/*! calculate cost of delay */
int calculate_cost(const std::vector<WiTi>& data, int c, int data_i)
{
    c = std::max(c - data[data_i].d, 0);
    return c * data[data_i].w;
}

/*! calculate ending time of permutation */
int calculate_c(const std::vector<WiTi>& data, int i)
{
    int c=0, k=0;
    for(int j = 1; j <= i; j = j << 1)
    {
        if(i & j) c += data[k].p;
        k++;
    }
    return c;
}

/*! dynamic programming algorithm */
int pd_algorithm(const std::vector<WiTi>& data, std::vector<int>& order)
{
    std::vector<int> lut; // look up table
    std::vector<std::vector<int>> lut_order;
    lut.resize(static_cast<unsigned long long>( pow(2, data.size()) ));
    lut_order.resize(static_cast<unsigned long long>( pow(2, data.size()) ));
    for (int i = 1; i < lut.size(); i++)
    {
        lut[i] = std::numeric_limits<int>::max();
        int c = calculate_c(data, i), idx = 0;
        for (int j = 1, k = 0; j <= i; j = j << 1, k++)
        {
            if (j & i)
            {
                int cost = lut[i - j] + calculate_cost(data ,c ,k);
                if(lut[i] > cost)
                {
                    lut[i] = cost;
                    lut_order[i] = lut_order[i - j];
                    idx = k;
                }
                else if (lut[i] == cost && (lut_order[i].empty() || lut_order[i] > lut_order[i - j]))
                {
                    lut_order[i] = lut_order[i - j];
                    idx = k;
                }
            }
        }
        lut_order[i].emplace_back(data[idx].id);
    }
    order = std::move(lut_order.back());
    return lut.back();
}

int main() {
    // open file
    std::fstream fs("../witi.data.txt");
    if(!fs.is_open()) throw std::invalid_argument("file not found");
    // run algorithm for each data
    for(int data_suffix = 10; data_suffix <= 20; data_suffix++)
    {
        // create next data name
        std::string data_name = "data." + std::to_string(data_suffix) + ":";
        // skip to data name
        std::string str;
        while(str != data_name) fs >> str;
        // read data size
        int n; fs >> n;
        // read data
        std::vector<WiTi> data;
        for (int i = 1; i <= n; i++)
        {
            WiTi tmp{};
            fs >> tmp.p >> tmp.w >> tmp.d;
            tmp.id = i;
            data.emplace_back(tmp);
        }
        // run algorithm
        std::vector<int> order;
        int result = pd_algorithm(data, order);
        // print result
        std::cout << "----------------\n";
        std::cout << data_name << "\n" << result << "\n";
        for(const auto& elem: order)
            std::cout << elem << " ";
        std::cout << "\n";
    }
    return 0;
}
