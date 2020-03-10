#include <CL/sycl.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

namespace sycl = cl::sycl;

void inplace_sort_scalar(std::vector<int> &v) {
    std::sort(v.begin(), v.end());
}


void inplace_sort_parallel(std::vector<int> &v) {
    assert((16 <= v.size()) && (v.size() <= 256) && "Unsupported vector size");
    {
        sycl::device dev = sycl::device(sycl::gpu_selector());
        sycl::queue q(dev);
        sycl::buffer<int, 1> d_v(v.data(), v.size());

        sycl::nd_range<1> nd(sycl::range<1>(v.size()), sycl::range<1>(v.size()));
        q.submit([&](sycl::handler &cgh) {
                auto acc = d_v.get_access<sycl::access::mode::read_write>(cgh);
                cgh.parallel_for<class BitonicSort>(nd, [=](sycl::nd_item<1> item) {
                    auto tid = item.get_global_id(0);
                    auto size = item.get_global_range().size();
                    for (uint i = 2; i <= size; i *= 2) {
                        for (uint j = i / 2; j > 0; j /= 2) {
                            auto comp = tid ^ j;
                            if (comp > tid) {
                                if ((tid & i) == 0) {
                                    if (acc[tid] > acc[comp]) {
                                        std::swap(acc[tid], acc[comp]);
                                    }
                                }
                                else {
                                    if (acc[tid] < acc[comp]) {
                                        std::swap(acc[tid], acc[comp]);
                                    }
                                }
                            }
                            item.barrier();
                        }
                    }
                    auto groupID = item.get_group(0);
                });
        });

        const auto host_acc = d_v.get_access<sycl::access::mode::read>();
        for (auto i = 0; i < host_acc.get_count(); ++i) {
            std::cout << host_acc[i] << " ";
        }
        std::cout << std::endl;
        
    }
}

bool verify(std::vector<int> &v1, std::vector<int> &v2) {
    return std::equal(v1.begin(), v1.end(), v2.begin());
}

std::vector<int> generate(const size_t sz) {
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
    const size_t sz = 256;
    auto v1 = generate(sz);
    auto v2 = generate(sz);

    dump(v2);

    inplace_sort_scalar(v1);
    inplace_sort_parallel(v2);
    
    std::cout << "Result correct: " << verify(v1, v2) << std::endl;
    dump(v1);
    dump(v2);
}
