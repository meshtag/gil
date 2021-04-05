//
// Copyright 2019 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_GIL_IMAGE_PROCESSING_DETAIL_MATH_HPP
#define BOOST_GIL_IMAGE_PROCESSING_DETAIL_MATH_HPP

#include <boost/gil/extension/numeric/convolve.hpp>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <array>
#include <cmath>

#include <iostream>

#include <vector>

namespace boost { namespace gil { namespace detail {

enum class kernel_type
{
    sobel_dx,
    sobel_dy,
};

static constexpr double pi = 3.14159265358979323846;

static constexpr std::array<float, 9> dx_sobel = {{-1, 0, 1, -2, 0, 2, -1, 0, 1}};

static const std::vector<float> dx_sobel_2_5 = {
    1, 0, -2, 0, 1, 4, 0, -8, 0, 4, 6, 0, -12, 0, 6, 4, 0, -8, 0, 4, 1, 0, -2, 0, 1
};

static constexpr std::array<float, 9> dx_scharr = {{-1, 0, 1, -1, 0, 1, -1, 0, 1}};
static constexpr std::array<float, 9> dy_sobel = {{1, 2, 1, 0, 0, 0, -1, -2, -1}};

static const std::vector<float> dy_sobel_2_5 {
    1, 4, 6, 4, 1, 0, 0, 0, 0, 0, -2, -8, -12, -8, -2, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1
};

static constexpr std::array<float, 9> dy_scharr = {{1, 1, 1, 0, 0, 0, -1, -1, -1}};

template <typename T, typename Allocator>
inline detail::kernel_2d<T, Allocator> get_identity_kernel()
{
    detail::kernel_2d<T, Allocator> kernel(1, 0, 0);
    kernel[0] = 1;
    return kernel;
}

inline void fill_kernel_view(gil::gray32f_view_t view, kernel_type type)
{
    if (type == kernel_type::sobel_dx)
    {
        for (std::ptrdiff_t row = 0; row < 3; ++row)
        {
            for (std::ptrdiff_t col = 0; col < 3; ++col)
            {
                view(col, row) = dx_sobel[3 * row + col];
            }
        }
    }
    else if (type == kernel_type::sobel_dy)
    {
        for (std::ptrdiff_t row = 0; row < 3; ++row)
        {
            for (std::ptrdiff_t col = 0; col < 3; ++col)
            {
                view(col, row) = dy_sobel[3 * row + col];
            }
        }
    }
}

inline auto gray32f_view_to_1d_vector(gil::gray32f_view_t view) -> std::vector<float>
{
    std::vector<float> view_vector;
    for (std::ptrdiff_t row = 0; row < static_cast<std::ptrdiff_t>(view.height()); ++row)
    {
        for (std::ptrdiff_t col = 0; col < static_cast<std::ptrdiff_t>(view.width()); ++col)
        {
            view_vector.push_back(view(row, col)[0]);
        }
    }
    return view_vector;
}

inline void view_convolve(gil::gray32f_view_t view1, gil::gray32f_view_t view2,
    gil::gray32f_view_t dst_view)
{
    // std::cout << "View1   " << view1.height() << "  " << view1.width() << "\n";
    // std::cout << "View2   " << view2.height() << "  " << view2.width() << "\n";
    // std::cout << "Dst_view  " << dst_view.height() << "  " << dst_view.width() << "\n";

    // std::cout << " view1  Trial   \n";
    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 5; ++j)
    //     {
    //         std::cout << view1(j, i)[0] << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "Done   \n";


    std::vector<float> kernel_vector = gray32f_view_to_1d_vector(view2);
    detail::kernel_2d<float> kernel(kernel_vector.begin(), kernel_vector.size(),
        view2.height() / 2, view2.width() / 2);

    // std::cout << "Trial   \n";
    // for (int i = 0; i < 3; ++i)
    // {
    //     for (int j = 0; j < 3; ++j)
    //     {
    //         std::cout << kernel.at(i, j) << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "Done   \n";

    detail::convolve_2d(view1, kernel, dst_view);
}

inline auto get_sobel_kernel(std::array<unsigned int, 2> order, unsigned int size)
    -> std::vector<float>
{
    unsigned int x_size = 2 * order[0] + 1, y_size = 2 * order[1] + 1;
    gil::gray32f_image_t kernel_x_1(3, 3), kernel_y_1(3, 3), resultant_kernel(size, size);
    gil::gray32f_image_t resultant_kernel_x(x_size, x_size), resultant_kernel_y(y_size, y_size);
    
    double const x_repetition = static_cast<unsigned int>(std::log2(order[0] - 1));
    double const x_decrease = std::pow(2, x_repetition);
    
    double const y_repetition = static_cast<unsigned int>(std::log2(order[1] - 1));
    double const smooth_repetition = static_cast<unsigned int>
        (std::log2((size - 2 * (order[0] + order[1] + 1) - 1) / 2));

    if (order[0])
    {
        unsigned int convolve_count = 1;
        fill_kernel_view(view(kernel_x_1), kernel_type::sobel_dx);
        copy_pixels(view(kernel_x_1), subimage_view(view(resultant_kernel_x),
            x_size / 2 - convolve_count, x_size / 2 - convolve_count, 3, 3));
        unsigned int prev_size = 3;
        
        // for (unsigned int i = 0; i < x_repetition; ++i)
        // {
        //     ++convolve_count;
        //     unsigned int intermediate_img_size = prev_size + std::pow(2, i + 1);
        //     gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
        //     view_convolve(subimage_view(view(resultant_kernel_x),
        //         x_size / 2 - intermediate_img_size / 2, x_size / 2 - intermediate_img_size / 2,
        //         intermediate_img_size, intermediate_img_size), subimage_view(
        //         view(resultant_kernel_x), x_size / 2 - prev_size / 2, x_size / 2 - prev_size,
        //         prev_size, prev_size), view(intermediate_img));
        //     prev_size = intermediate_img_size;
        // }

        for (unsigned int i = 0; i < order[0] - 1 ; ++i)
        {
            ++convolve_count;
            unsigned int intermediate_img_size = 2 * convolve_count + 1;
            // std::cout << "int_size    " << intermediate_img_size << "\n";
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_kernel_x),
                x_size / 2 - convolve_count, x_size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size),
                view(kernel_x_1), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel_x),
                x_size / 2 - convolve_count, x_size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size));
        }
    }
    if (order[1])
    {
        unsigned int convolve_count = 1;
        fill_kernel_view(view(kernel_y_1), kernel_type::sobel_dy);
        copy_pixels(view(kernel_y_1), subimage_view(view(resultant_kernel_y),
            y_size / 2 - convolve_count, y_size / 2 - convolve_count, 3, 3));
        unsigned int prev_size = 3;
        
        // for (unsigned int i = 0; i < x_repetition; ++i)
        // {
        //     ++convolve_count;
        //     unsigned int intermediate_img_size = prev_size + std::pow(2, i + 1);
        //     gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
        //     view_convolve(subimage_view(view(resultant_kernel_x),
        //         x_size / 2 - intermediate_img_size / 2, x_size / 2 - intermediate_img_size / 2,
        //         intermediate_img_size, intermediate_img_size), subimage_view(
        //         view(resultant_kernel_x), x_size / 2 - prev_size / 2, x_size / 2 - prev_size,
        //         prev_size, prev_size), view(intermediate_img));
        //     prev_size = intermediate_img_size;
        // }

        for (unsigned int i = 0; i < order[1] - 1 ; ++i)
        {
            ++convolve_count;
            unsigned int intermediate_img_size = 2 * convolve_count + 1;
            // std::cout << "int_size    " << intermediate_img_size << "\n";
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_kernel_y),
                y_size / 2 - convolve_count, y_size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size),
                view(kernel_y_1), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel_y),
                y_size / 2 - convolve_count, y_size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size));
        }
    }

    std::vector<float> ans;
    for (int i = 0; i < view(resultant_kernel_y).height(); ++i)
    {
        for (int j = 0; j < view(resultant_kernel_y).width(); ++j)
        {
            ans.push_back(view(resultant_kernel_y)(j, i)[0]);
        }
    }
    return ans;
}

}}} // namespace boost::gil::detail

#endif
