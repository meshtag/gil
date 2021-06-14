//
// Copyright 2019 Miral Shah <miralshah2211@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/gil.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>
#include <boost/gil/extension/numeric/convolve_novel.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cstddef>

#include <boost/gil/extension/numeric/algorithm.hpp>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <boost/gil/extension/numeric/pixel_numeric_operations.hpp>

#include <boost/gil/algorithm.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/gil/metafunctions.hpp>

#include <boost/assert.hpp>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <vector>

namespace gil = boost::gil;

std::uint8_t img[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 255, 0, 0, 0, 255, 0, 0,
    0, 0, 0, 255, 0, 255, 0, 0, 0,
    0, 0, 0, 0, 255, 0, 0, 0, 0,
    0, 0, 0, 255, 0, 255, 0, 0, 0,
    0, 0, 255, 0, 0, 0, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

std::uint8_t output[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 28, 28, 28, 0, 28, 28, 28, 0,
    0, 28, 56, 56, 56, 56, 56, 28, 0,
    0, 28, 56, 85, 85, 85, 56, 28, 0,
    0, 0, 56, 85, 141, 85, 56, 0, 0,
    0, 28, 56, 85, 85, 85, 56, 28, 0,
    0, 28, 56, 56, 56, 56, 56, 28, 0,
    0, 28, 28, 28, 0, 28, 28, 28, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

void test_convolve_2d_with_normalized_mean_filter()
{
    gil::gray8c_view_t src_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(img), 9);

    gil::image<gil::gray8_pixel_t> temp_img(src_view.width(), src_view.height());
    typename gil::image<gil::gray8_pixel_t>::view_t temp_view = view(temp_img);
    gil::gray8_view_t dst_view(temp_view);

    std::vector<float> v(9, 1.0f / 9.0f);
    gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);

    gil::detail::convolve_2d(src_view, kernel, dst_view);

    gil::gray8c_view_t out_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(output), 9);

    BOOST_TEST(gil::equal_pixels(out_view, dst_view));
}

void test_modified_correlate_2d_with_normalized_filter()
{
    gil::gray8c_view_t src_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(img), 9);

    gil::image<gil::gray8_pixel_t> temp_img(src_view.width(), src_view.height());
    typename gil::image<gil::gray8_pixel_t>::view_t temp_view = view(temp_img);
    gil::gray8_view_t dst_view(temp_view);

    std::vector<float> v(9, 1.0f / 9.0f);
    gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);

    // gil::detail::correlate_2d(src_view, kernel, dst_view);
    gil::detail::image_correlate(src_view, v, dst_view);

    gil::gray8c_view_t out_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(output), 9);

    /*Debug*/
    // for (std::ptrdiff_t row = 0; row < src_view.height(); ++row)
    // {
    //     for (std::ptrdiff_t col = 0; col < src_view.width(); ++col)
    //         std::cout << static_cast<int>(src_view(col, row)[0]) << " ";
    //     std::cout << "\n";
    // }
    // std::cout << "\n\n";
    // for (std::ptrdiff_t row = 0; row < out_view.height(); ++row)
    // {
    //     for (std::ptrdiff_t col = 0; col < out_view.width(); ++col)
    //         std::cout << static_cast<int>(out_view(col, row)[0]) << " ";
    //     std::cout << "\n";
    // }
    // std::cout << "\n\n";
    // for (std::ptrdiff_t row = 0; row < dst_view.height(); ++row)
    // {
    //     for (std::ptrdiff_t col = 0; col < dst_view.width(); ++col)
    //         std::cout << static_cast<int>(dst_view(col, row)[0]) << " ";
    //     std::cout << "\n";
    // }
    /*Debug*/

    BOOST_TEST(gil::equal_pixels(out_view, dst_view));
}

int main()
{
    test_convolve_2d_with_normalized_mean_filter();
    test_modified_correlate_2d_with_normalized_filter();

    // gil::gray8_image_t img_in(5, 5), img_out(5, 5);
    // for (int i = 0; i < 5; ++i)
    //     for (int j = 0; j < 5; ++j)
    //         nth_channel_view(gil::view(img_in), 0)(i, j)[0] = i + j;

    // std::vector<float> v = {0, 0, 0, 0, 0, 0, 0, 1, 0};
    // std::vector<float> p = {0, 0, 0, 0, 0, 0, 0, 1, 0};
    // gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);
    // for (int i = 0 ; i < 3; ++i)
    // {
    //     for (int j = 0; j < 3; ++j)
    //         std::cout << static_cast<int>(kernel.at(i, j)) << " ";
    //     std::cout << "\n";
    // }
    // gil::detail::correlate_2d(gil::const_view(img_in), kernel, gil::view(img_out));

    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 5; ++j)
    //         std::cout << static_cast<int>(nth_channel_view(gil::view(img_in), 0)(j, i)[0]) << " ";
    //     std::cout << "\n";
    // }

    // std::cout << "\n\n";

    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 5; ++j)
    //         std::cout << static_cast<int>(nth_channel_view(gil::view(img_out), 0)(j, i)[0]) << " ";
    //     std::cout << "\n";
    // }

    // gil::gray8_image_t img_obtained(5, 5);
    // gil::detail::image_correlate(gil::const_view(img_in), p, gil::view(img_obtained));

    // std::cout << "\n\n";
    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 5; ++j)
    //         std::cout << static_cast<int>(nth_channel_view(gil::view(img_obtained), 0)(j, i)[0]) << " ";
    //     std::cout << "\n";
    // }

    return ::boost::report_errors();
}
