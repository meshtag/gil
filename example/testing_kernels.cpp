#include <boost/gil/detail/math.hpp>
#include <bits/stdc++.h>

namespace gil = boost::gil;

int main()
{
    auto vec = gil::detail::kernel_convolve(2, 0, 3);

    for(auto v : vec)
        std::cout << v << "  ";
}