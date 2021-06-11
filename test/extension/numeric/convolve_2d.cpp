//
// Copyright 2019 Miral Shah <miralshah2211@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/gil.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>

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

template <typename SrcView, typename DstView>
void image_correlate(SrcView src_view, std::vector<float> kernel, DstView dst_view)
{
    typename SrcView::iterator src_view_it = src_view.begin();
    std::vector<typename SrcView::value_type> src_view_vec;
    typename SrcView::value_type zero_pixel;
    pixel_zeros_t<typename SrcView::value_type>()(zero_pixel);
    using p = typename SrcView::value_type;

    while (src_view_it != src_view.end())
    {
        src_view_vec.push_back(*src_view_it);
        ++src_view_it;
    }
    auto it = src_view_vec.begin();
    auto it_dst = dst_view.begin();

    while (it != src_view_vec.end())
    {
        pixel_assigns_t<p, p>()(
        std::inner_product(
            it,
            it + kernel.size(),
            kernel.begin(),
            zero_pixel,
            pixel_plus_t<p, p, p>(),
            pixel_multiplies_scalar_t<p, float, p>()),
        *it_dst);
        ++it;
        ++it_dst;

    std::cout << "\n\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << static_cast<unsigned int>(nth_channel_view(dst_view, 0)(i, j)[0]) << " ";
        std::cout << "\n";
    }

    }
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

    gray8_image_t img(3, 3), img_out(3, 3), img_out_lib(3, 3);
    std::vector<float> kernel = {10};
    gil::detail::kernel_2d<float> kernel1(kernel.begin(), kernel.size(), 0, 0);


    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            nth_channel_view(view(img), 0)(i, j)[0] = i + j;
    gil::detail::convolve_2d(view(img), kernel1, view(img_out_lib));

    image_correlate(view(img), kernel, view(img_out));
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << static_cast<unsigned int>(nth_channel_view(view(img), 0)(i, j)[0]) << " ";
        std::cout << "\n";
    }
    std::cout << "\n\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << static_cast<unsigned int>(nth_channel_view(view(img_out), 0)(i, j)[0]) << " ";
        std::cout << "\n";
    }
    std::cout << "\n\n";
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << static_cast<unsigned int>(nth_channel_view(view(img_out_lib), 0)(i, j)[0]) << " ";
        std::cout << "\n";
    }

    BOOST_TEST(gil::equal_pixels(view(img_out), view(img_out_lib)));

    return ::boost::report_errors();
}
