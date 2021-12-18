#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <chrono>
#include <omp.h>

static std::vector<int> generate_data(size_t size) {
    using value_type = int;

    static std::uniform_int_distribution<value_type> distribution(
            std::numeric_limits<value_type>::min(),
            std::numeric_limits<value_type>::max());
    static std::default_random_engine generator;

    std::vector<value_type> data(size);
    std::generate(data.begin(), data.end(), []() { return distribution(generator); });
    return data;
}

int max(std::vector<int> A, int i, int j, int numThreads) {
    int max_val = A[0];
    #pragma omp parallel for num_threads(numThreads) reduction(max:max_val)
    for (int idx = i; idx < j; idx++)
        max_val = std::max(max_val, A[idx]);
    return max_val;
}


int main(int argc, char *argv[]) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;

    int N = atoi(argv[1]);
    double times[10];
    std::vector<int> vector = generate_data(N);
    for (int nthr = 1; nthr <= 10; ++nthr) {
        std::cout << "Number of threads = " << nthr << std::endl;
        duration<double, std::milli> total_time{};
        int N_MES = 7;
        int max_value;
        for (int nmeasures = 0; nmeasures <= N_MES; ++nmeasures) {
            auto t1 = high_resolution_clock::now();
            max_value = max(vector, 0, vector.size(), nthr);
            auto t2 = high_resolution_clock::now();
            total_time += (t2 - t1);
        }
        double tt = total_time.count() / N_MES;
        std::cout << "Avg execution time = " << tt << " ms" << std::endl;
        times[nthr - 1] = tt;
    }
    for (int i = 0; i < 10; i++) {
        std::cout << times[i] << '\n';
    }

//    dumb testing via sort and getting the first item
//    std::sort(vector.begin(), vector.end(), std::greater<int>());
//    std::cout << "Max value" << vector[0] << std::endl;
    return 0;
}
