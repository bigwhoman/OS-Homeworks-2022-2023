#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>

using namespace std;

mutex pol;
map<pair<char, char>, int> adjs;
map<vector<pair<char, char>>, int> road_car_num;
map<pair<char, char>, mutex> Moonitor;
float all_pollution = 0;


void car_function(vector<pair<char, char>> paths, int p, int t_num, int path_num) {
    fstream fio;
    mkdir("outputs", 0777);
    fio.open("./outputs/" + to_string(path_num) + "-" + to_string(t_num) + ".txt", ios::trunc | ios::out | ios::in);
    for (auto &path: paths) {

        Moonitor[path].lock();
        struct timeval time_now{};
        gettimeofday(&time_now, nullptr);
        auto start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
        float pollution = 0;
        for (int i = 0; i <= 10000000; ++i) {
            pollution += floor(float(i) / (1000000 * float(p) * float(adjs[path])));
        }
        pol.lock();
        all_pollution += pollution;
        pol.unlock();
        gettimeofday(&time_now, nullptr);
        auto finish = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
        fio << path.first << "," << start << "," << path.second << "," << finish << "," << pollution << ","
            << all_pollution << endl;
        Moonitor[path].unlock();
    }
    fio.close();
}

int main(int argc, char **argv) {
    ifstream myfile;
    myfile.open(argv[1]);
    string mystring;
    vector<pair<char, char>> roads;
    road_car_num.clear();
    if (myfile.is_open()) { // always check whether the file is open
        bool p1 = true;
        while (myfile) {
            getline(myfile, mystring);

            if (mystring[0] == '#') {
                p1 = false;
                continue;
            }
            if (p1) {
                smatch m;
                regex_search(mystring, m, regex("([A-Z]) - ([A-Z]) - ([\\d]+)"));
                pair<char, char> r = {string(m[1])[0], string(m[2])[0]};
                Moonitor[r];
                adjs[r] = stoi(m[3]);
            } else if (mystring != "") {
                bool match = regex_search(mystring, regex("[\\d]+.*"));
                if (match) {
                    road_car_num[roads] = stoi(mystring);
                    roads.clear();
                } else {
                    smatch res;
                    regex exp("([A-Z])");
                    string::const_iterator searchStart(mystring.cbegin());
                    int k = 0;
                    pair<char, char> pr;
                    vector<char> stops;
                    while (regex_search(searchStart, mystring.cend(), res, exp)) {
                        stops.push_back(string(res[0])[0]);
                        searchStart = res.suffix().first;
                    }
                    vector<pair<char, char>> bh;
                    for (int i = 0; i < stops.size() - 1; i++) {
                        roads.push_back(pair<char, char>{stops[i], stops[i + 1]});
                    }
                }
            }
        }
        myfile.close();


        vector<thread> threads;
        int t_num = 1;
        int path_num = 1;
        srand(time(0));
        for (auto &c: road_car_num) {
            for (int i = 0; i < c.second; i++) {
                thread t(car_function, c.first, rand()%10+1, t_num, path_num);
                threads.push_back(move(t));
                t_num++;
            }
            // t_num = 1;
            path_num++;
        }

        for (auto &tt: threads)
            tt.join();

        cout << all_pollution;
    }
    return 0;
}
