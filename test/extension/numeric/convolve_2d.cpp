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

using namespace boost::gil;

template <typename SrcView>
auto image_correlate(SrcView src_view, std::vector<float> kernel) 
    -> std::ptrdiff_t
{
    typename SrcView::iterator src_view_it = src_view.begin();
    using pixel_t = typename SrcView::value_type;
    std::vector<typename SrcView::value_type> src_view_vec;
    pixel_t zero_pixel;
    pixel_zeros_t<pixel_t>()(zero_pixel);

    while (src_view_it != src_view.end())
    {
        src_view_vec.push_back(*src_view_it);
        ++src_view_it;
    }

    std::ptrdiff_t ans = std::inner_product(src_view_vec.begin(), src_view_vec.end(),
                            kernel.begin(), zero_pixel, pixel_plus_t<pixel_t, pixel_t, pixel_t>(),
                            pixel_multiplies_scalar_t<pixel_t, float, pixel_t>());

    return ans;
}



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

int main()
{
    test_convolve_2d_with_normalized_mean_filter();

    gil::gray8_image_t img_in(5, 5), img_out(5, 5);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            nth_channel_view(gil::view(img_in), 0)(i, j)[0] = i + j;

    std::vector<float> v = {0, 0, 0, 0, 0, 0, 0, 1, 0};
    std::vector<float> p = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);
    for (int i = 0 ; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << static_cast<int>(kernel.at(i, j)) << " ";
        std::cout << "\n";
    }
    gil::detail::correlate_2d(gil::const_view(img_in), kernel, gil::view(img_out));

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
            std::cout << static_cast<int>(nth_channel_view(gil::view(img_in), 0)(j, i)[0]) << " ";
        std::cout << "\n";
    }

    std::cout << "\n\n";

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
            std::cout << static_cast<int>(nth_channel_view(gil::view(img_out), 0)(j, i)[0]) << " ";
        std::cout << "\n";
    }

    gil::gray8_image_t img_obtained(5, 5);
    for (std::ptrdiff_t view_row = 1; view_row < gil::view(img_in).height() - 1; ++view_row)
    {
        for (std::ptrdiff_t view_col = 1; view_col < gil::view(img_in).width() - 1; ++view_col)
        {
            gil::view(img_obtained)(view_col, view_row) = image_correlate(gil::subimage_view(
                gil::view(img_in), view_row - 1, view_col - 1, 3, 3), p);
        }
    }

    std::cout << "\n\n";

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
            std::cout << static_cast<int>(nth_channel_view(gil::view(img_obtained), 0)(j, i)[0]) << " ";
        std::cout << "\n";
    }

    return ::boost::report_errors();
}
