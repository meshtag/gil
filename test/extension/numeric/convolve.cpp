//
// Copyright 2019-2020 Mateusz Loskot <mateusz at loskot dot net>
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
#include "core/test_fixture.hpp"
#include "core/image/test_fixture.hpp"

#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/convolve_novel.hpp>

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct test_image_1x1_kernel_1x1_identity
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        auto const img = fixture::create_image<image_t>(1, 1, 7);
        image_t img_out(img);

        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const kernel = fixture::create_kernel<channel_t>({1});
        gil::detail::convolve_1d<pixel_t>(gil::const_view(img_out), kernel, gil::view(img_out));

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
        auto const img = fixture::create_image<Image>(1, 1, 7);
        image_t img_out(img);

        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const kernel = fixture::create_kernel<channel_t>({0, 0, 0, 0, 1, 0, 0, 0, 0});
        gil::detail::convolve_1d<pixel_t>(gil::const_view(img_out), kernel, gil::view(img_out));

        BOOST_TEST(gil::const_view(img).front() == gil::const_view(img_out).front());
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_image_1x1_kernel_3x3_identity{});
    }
};

struct test_image_3x3_kernel_3x3_identity
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const img = fixture::generate_image<image_t>(3, 3, fixture::random_value<channel_t>{});
        image_t img_out(img);

        auto const kernel = fixture::create_kernel<channel_t>({0, 0, 0, 0, 1, 0, 0, 0, 0});
        gil::detail::convolve_1d<pixel_t>(gil::const_view(img_out), kernel, gil::view(img_out));

        BOOST_TEST(gil::equal_pixels(gil::const_view(img), gil::const_view(img_out)));
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_image_3x3_kernel_3x3_identity{});
    }
};

struct test_image_5x5_kernel_3x3_identity
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using pixel_t = typename image_t::value_type;
        using channel_t = typename gil::channel_type<pixel_t>::type;
        auto const img = fixture::generate_image<image_t>(5, 5, fixture::random_value<channel_t>{});
        image_t img_out(img);

        auto const kernel = fixture::create_kernel<channel_t>({0, 0, 0, 0, 1, 0, 0, 0, 0});
        gil::detail::convolve_1d<pixel_t>(gil::const_view(img_out), kernel, gil::view(img_out));
        // TODO: Test different boundary options

        BOOST_TEST(gil::equal_pixels(gil::const_view(img), gil::const_view(img_out)));
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_image_5x5_kernel_3x3_identity{});
    }
};

void test_check()
{
    gil::gray8_image_t img_256;
    gil::read_image("resize_256_gray.png", img_256, gil::png_tag{});
    gil::gray8_image_t img_out_256(gil::view(img_256).width(), gil::view(img_256).height());
    gil::gray8_image_t opencv_img_out_256(gil::view(img_256).width(), gil::view(img_256).height());
    gil::read_image("output.png", opencv_img_out_256, gil::png_tag{});
    std::vector<float> v{std::vector<float>(9, 1.0f / 9.0f)};
    gil::detail::kernel_2d<float> kernel{gil::detail::kernel_2d<float>(v.begin(), v.size(), 1, 1)};
    gil::detail::correlate_2d(gil::const_view(img_256), kernel, 
        gil::view(img_out_256));

    BOOST_TEST(gil::equal_pixels(gil::const_view(img_out_256), gil::const_view(opencv_img_out_256)));
    
}

int main()
{
    test_image_1x1_kernel_1x1_identity::run();
    test_image_1x1_kernel_3x3_identity::run();
    test_image_3x3_kernel_3x3_identity::run();
    test_image_5x5_kernel_3x3_identity::run();
    test_check();

    // gil::gray8_image_t img(5, 5);
    // std::vector<int> vec;
    // for (int i = 1; i < 26; ++i)
    //     vec.push_back(i);
    // for (int i = 0; i < 5; ++i)
    //     for (int j = 0; j < 5; ++j)
    //         nth_channel_view(gil::view(img), 0)(j, i)[0] = vec[5 * i + j];

    // auto sub_view = gil::subimage_view(gil::view(img), 0, 0, 3, 3);

    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 5; ++j)
    //         std::cout << static_cast<int>(nth_channel_view(gil::view(img), 0)(j, i)[0]) << " ";
    //     std::cout << "\n";
    // }
    // std::cout << "\n\n";
    // for (int i = 0; i < 3; ++i)
    // {
    //     for (int j = 0; j < 3; ++j)
    //         std::cout << static_cast<int>(nth_channel_view(sub_view, 0)(j, i)[0]) << " ";
    //     std::cout << "\n";
    // }

    // auto it = gil::view(img).begin();
    // while (it != gil::view(img).end())
    //     std::cout << static_cast<int>(*it) << " ", ++it;
    // std::cout << "\n";

    return ::boost::report_errors();
}
