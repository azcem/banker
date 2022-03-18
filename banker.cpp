#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <utility>
using namespace std;

bool lessthan(vector<int> need, vector<int> work)
{
    int n = need.size();
    for (int i = 0; i < n; i++)
    {
        if (need[i] > work[i]) return false;
    }
    return true;
}

pair <bool, vector<string>> isSafe(vector <int> avail, vector <vector<int>> need, vector <vector<int>> alloc)
{
    int m = avail.size();
    int n = need.size();
    vector<int> work(avail);
    vector<bool> finish(n, false);
    vector<string> safe;
    int i =0;
    while (true)
    {
        //step 2
        if (i == n - 1) i = 0;
        while (i < n)
        {
            if (finish[i] == false && lessthan(need[i], work)) break; //found such i
            if (i == n - 1) goto step4; // vector finished and condiotions not met
            i++;
        }
        //step 3
        for (int j = 0; j < m; j++)
        {
            work[j] += alloc[i][j];
            finish[i] = true;
        }
        safe.push_back('P' + to_string(i));
    }
    //step 4
step4:
    if (count(finish.begin(), finish.end(), false) == 0)
    {
        return make_pair(true, safe);
    }
    else
    {
        return make_pair(false, safe);
    }
}

pair <bool, vector<string>> Request(vector <int> avail, vector <vector<int>> need, vector <vector<int>> alloc, vector<int> req, int i)
{
    vector <string> safe;
    if (!lessthan(req, need[i])) return make_pair(false, safe);
    if (!lessthan(req, avail)) return make_pair(false, safe);
    int m = avail.size();
    for (int j = 0; j < m; j++)
    {
        avail[j] -= req[j];
        alloc[i][j] += req[j];
        need[i][j] -= req[j];
    }
    auto s = isSafe(avail, need, alloc);
    for (int j = 0; j < m; j++)
    {
        avail[j] += req[j];
        alloc[i][j] -= req[j];
        need[i][j] += req[j];
    }
    return s;
}


int main()
{
begin:
    char in = 0;
    int m, n;
    cout << "Enter number of processes\n";
    cin >> n;
    cout << "Enter number of resource types\n";
    cin >> m;
    cout << "Enter Allocation matrix\n";
    vector<vector<int>> alloc(n);
    for (int i = 0; i < n; i++)
    {
        alloc[i].reserve(m);
        for (int j = 0; j < m; j++)
        {
            int num;
            cin >> num;
            alloc[i].push_back(num);
        }
    }
    cout << "Enter Max matrix\n";
    vector<vector<int>> max(n);
    vector<vector<int>> need(n);
    for (int i = 0; i < n; i++)
    {
        max[i].reserve(m);
        need[i].reserve(m);
        for (int j = 0; j < m; j++)
        {
            int num;
            cin >> num;
            max[i].push_back(num);
            need[i].push_back(max[i][j] - alloc[i][j]);
        }
    }
    cout << "Enter Available vector\n";
    vector <int> avail(m);
    for (int i = 0; i < m; i++)
    {
        int num;
        cin >> num;
        avail[i] = (num);
    }
    //printing need matrix
    cout << "Need matrix: \n   ";
    char res = 'A';
    for (int i = 0; i < m; i++)
    {
        cout << (char)(res + i) << ' ';
    }
    cout << "\n";
    for (int i = 0; i < n; i++)
    {
        cout << 'P' << i << ' ';
        for (int j = 0; j < m; j++)
        {
            cout << need[i][j] << ' ';
        }
        cout << '\n';
    }

    while (in != 'e')
    {
        cout << "Press s to enquire about safe state, r to enter a request, n to enter new matrices, or e to exit\n";
        cin >> in;
        if (in == 'n') goto begin;
        if (in == 's')
        {
            auto str = isSafe(avail, need, alloc);
            if (str.first)
            {
                cout << "Yes, Safe state <";
                for (int i = 0; i < n; i++)
                {
                    cout << str.second[i];
                    if (i != n - 1) cout << ',';
                }
                cout << ">\n";
            }
            else cout << "No, there's no safe state.\n";
        }
        if (in == 'r')
        {
            string p;
            int r;
            vector<int> req(m);
            cout << "Enter process\n";
            cin >> p;
            cout << "Enter request vector\n";
            for (int i = 0; i < m; i++)
            {
                cin >> r;
                req[i] = r;
            }
            p = *(p.begin() + 1);
            stringstream process(p);
            int i;
            process >> i;
            auto str = Request(avail, need, alloc, req, i);
            if (str.first)
            {
                cout << "Yes request can be granted with safe state , Safe state <P" << i << "req, ";
                for (int j = 0; j < n; j++)
                {
                    cout << str.second[j];
                    if (j != n - 1) cout << ',';
                }
                cout << ">\n";
            }
            else cout << "Request can't be granted.\n";
        }
    }
}
