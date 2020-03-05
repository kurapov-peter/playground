#include <CL/sycl.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>


void inplace_sort_scalar(std::vector<int> &v) {
    std::sort(v.begin(), v.end());
}

void inplace_sort_parallel(std::vector<int> &v) {
    using namespace cl::sycl;
    buffer<int, 1> d_v(v);
    queue q;
    q.submit([&](handler &cgh) {
            auto acc = d_v.get_access<access::mode::read_write>();
            cgh.parallel_for<class BitonicSort>(range<1> {v.size()}, [=](id<1> i) {
                acc[i] = 4;
            });
    });
}

bool verify(std::vector<int> &v1, std::vector<int> &v2) {
    return std::equal(v1.begin(), v1.end(), v2.begin());
}

std::vector<int> generate(size_t sz) {
    assert((sz & (sz-1)) == 0 && "Size must be of power of 2");
    /*std::random_device seed;
    std::mt19937 eng {seed()};*/
    std::mt19937 eng(0);
    std::uniform_int_distribution<int> distr {0, 100};
    auto gen = [&]() { return distr(eng); };
    std::vector<int> res(sz);
    std::generate(begin(res), end(res), gen);
    return res;
}

void dump(std::vector<int> v) {
    for (auto &c : v) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

int main() {
    const size_t sz = 8;
    auto v1 = generate(sz);
    auto v2 = generate(sz);

    inplace_sort_scalar(v1);
    inplace_sort_parallel(v2);
    
    std::cout << "Result correct: " << verify(v1, v2) << std::endl;
    dump(v1);
    dump(v2);
}
