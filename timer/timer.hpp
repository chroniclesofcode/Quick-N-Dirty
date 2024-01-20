#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <vector>

namespace qnd {

class timer {
public:
    timer(const char filename[]) {
        out.open(filename);
        tot = 0;
        count = 0;
        small = 999999999;
        big = 0;
        out << "**** TIMES ****\n\n";
    }

    ~timer() {
        out.close();
    }

    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_time);
        int64_t stoplen = duration.count();

        tot += stoplen;
        count++;
        times.push_back(stoplen);
        small = std::min(small, stoplen);
        big = std::max(big, stoplen);

        out << stoplen << " milliseconds" << '\n';
    }

    void reset(const char filename[]) {
        out.close();
        out.open(filename);
        times.clear();
        tot = 0;
        count = 0;
        small = 999999999;
        big = 0;
        out << "**** TIMES ****\n\n";
    }

    void printStats() {
        out << "\n**** STATISTICS ****\n\n";
        double mean = (double)tot / count;
        out << "count: " << count << '\n';
        out << "average: " << mean << " milliseconds\n";
        double stddev = 0;
        for (int i = 0; i < times.size(); i++) {
            stddev += ((double)times[i] - mean) * ((double)times[i] - mean);
        }
        stddev /= count;
        out << "variance: " << stddev << " milliseconds^2\n";
        out << "standard deviation: " << sqrt(stddev) << " milliseconds\n";
        out << "min: " << small << " milliseconds\n";
        out << "max: " << big << " milliseconds\n";
    }

private:
    std::ofstream out;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    int64_t tot;
    int64_t count;
    std::vector<int64_t> times;
    int64_t small;
    int64_t big;
};

}
