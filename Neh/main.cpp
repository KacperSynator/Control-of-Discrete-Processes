#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

struct Neh
{
    int id;
    vector<int> p;
    int time_sum;
};

int calculate(vector<Neh> &data)
{
    int machines = data[0].p.size();
    int jobs = data.size();
    vector<vector<int>> lut_times;
    lut_times.resize(jobs);
    for(int i=0; i<jobs; i++)
    {
        lut_times[i].resize(machines);
        for(int j=0; j<machines;j++)
        {
            if(j>0 && i>0)
            {
                lut_times[i][j] = max(lut_times[i][j-1],lut_times[i-1][j]) + data[i].p[j];
            }
            else if(i==0 && j==0)
            {
                lut_times[i][j] = max(0,0) + data[i].p[j];
            }
            else if(j==0 && i>0)
            {
                lut_times[i][j] = max(0,lut_times[i-1][j]) + data[i].p[j];
            }
            else if( j>0 && i==0)
            {
                lut_times[i][j] = max(lut_times[i][j-1],0) + data[i].p[j];
            }
        }
    }
    return lut_times[jobs-1][machines-1];
}

int main()
{
    int n,m;
    vector<Neh> data;
    cin >> n >> m;
    data.resize(n);
    for(int  i=0;i<n;i++)
    {
        data[i].p.resize(m);
        data[i].id = i+1;
        int sum=0;
        for(int j=0; j < m; j++)
        {
            cin >> data[i].p[j];
            sum+=data[i].p[j];
        }
        data[i].time_sum = sum;
    }
    stable_sort(data.begin(),data.end(),[] (const Neh& a, const Neh& b){return a.time_sum <= b.time_sum;});
    vector<Neh> result;
    while(!data.empty())
    {
        result.emplace_back(data.back());
        int cmax = numeric_limits<int>::max();
        int index = 0;
        int cmax_temp = numeric_limits<int>::max();
        for(int i=result.size()-1;i+1>0;i--)
        {
            cmax_temp = calculate(result);
            if(cmax_temp<=cmax)
            {
                cmax = cmax_temp;
                index = i;
            }

            for(int i=0; i<result.size();i++)
            {
                cout << result[i].id << " ";
            }
            cout << "abc " << cmax << endl;

            if(i>0)
            {
                swap(result[i],result[i-1]);
            }
        }
        result.erase(result.begin());
        result.insert(result.begin()+index,data.back());
        data.pop_back();
        cout << endl;
        cout << cmax << endl;
        for(int i=0; i<result.size();i++)
        {
            cout << result[i].id << " ";
        }
        cout << endl;
    }
}