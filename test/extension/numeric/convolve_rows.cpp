//
// Copyright 2019-2020 Mateusz Loskot <mateusz at loskot dot net>
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>

#include <boost/core/lightweight_test.hpp>

#include <tuple>
#include <type_traits>

#include "test_fixture.hpp"
#include "core/image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct test_image_1x1_kernel_1x1_identity
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        auto const img = fixture::create_image<image_t>(1, 1, 7);
        auto img_out = fixture::create_image<image_t>(1, 1, 0);

        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const kernel = fixture::create_kernel<channel_t>({1});
        gil::convolve_rows<pixel_t>(const_view(img), kernel, view(img_out));

        // 1x1 kernel reduces convolution to multiplication
        BOOST_TEST(gil::const_view(img).front() == gil::const_view(img_out).front());
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_image_1x1_kernel_1x1_identity{});
    }
};

struct test_image_1x1_kernel_3x3_identity
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        auto const img = fixture::create_image<image_t>(1, 1, 7);
        auto img_out = fixture::create_image<image_t>(1, 1, 0);

        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const kernel = fixture::create_kernel<channel_t>({0, 0, 0, 0, 1, 0, 0, 0, 0});
        gil::convolve_rows<pixel_t>(const_view(img), kernel, view(img_out));

        BOOST_TEST(gil::const_view(img).front() == gil::const_view(img_out).front());
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_image_1x1_kernel_3x3_identity{});
    }
};

struct test_image_5x5_kernel_1x9_boundary_extend_reflection
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto img = fixture::generate_image<image_t>(5, 5, fixture::random_value<channel_t>{});
        auto img_view = gil::view(img);
        image_t img_out(img), img_expected(img);
        unsigned int kernel_shift_offset = 2;

        fixture::row_conv1D_offset_img_generator(img_view, gil::view(img_expected),
            kernel_shift_offset);
        fixture::row_conv1D_offset_img_generator(img_view, gil::view(img_expected),
            -1, 0, 1, img_view.height(), 1);
        fixture::row_conv1D_offset_img_generator(img_view, gil::view(img_expected),
            1, 0, 0, img_view.height(), 2);

        auto const kernel = fixture::create_kernel<channel_t>({0, 0, 0, 0, 0, 0, 1, 0, 0});
        gil::convolve_rows<pixel_t>(gil::const_view(img), kernel, gil::view(img_out),
            gil::boundary_option::extend_reflection);

        BOOST_TEST(gil::equal_pixels(gil::const_view(img_out), gil::const_view(img_expected)));
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(
            test_image_5x5_kernel_1x9_boundary_extend_reflection{});
    }
};

int main()
{
    test_image_1x1_kernel_1x1_identity::run();
    test_image_1x1_kernel_3x3_identity::run();
    test_image_5x5_kernel_1x9_boundary_extend_reflection::run();

    return ::boost::report_errors();
}
