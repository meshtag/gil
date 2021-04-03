//
// Copyright 2019 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
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
#include <vector>

namespace boost { namespace gil { namespace detail {

enum class kernel_type
{
    sobel_dx,
    sobel_dy,
};

static constexpr double pi = 3.14159265358979323846;

static constexpr std::array<float, 9> dx_sobel = {{-1, 0, 1, -2, 0, 2, -1, 0, 1}};
static constexpr std::array<float, 25> dx_sobel2 = {{
    -1,-2,0,2,1,-4,-8,0,8,4,-6,-12,0,12,6,-4,-8,0,8,4,-1,-2,0,2,1
}}; 
// In variable name "dx_sobel2", "2" indicates that the order of Sobel derivative in x-direction 
// is 2.
static constexpr std::array<float, 9> dx_scharr = {{-1, 0, 1, -1, 0, 1, -1, 0, 1}};
static constexpr std::array<float, 9> dy_sobel = {{1, 2, 1, 0, 0, 0, -1, -2, -1}};
static constexpr std::array<float, 25> dy_sobel2 = {{
    -1,-4,-6,-4,-1,-2,-8,-12,-8,-2,0,0,0,0,0,2,8,12,8,2,1,4,6,4,1
}};
// In variable name "dy_sobel2", "2" indicates that the order of Sobel derivative in y-direction 
// is 2.
static constexpr std::array<float, 9> dy_scharr = {{1, 1, 1, 0, 0, 0, -1, -1, -1}};
static const std::vector<std::vector<float>> smoothing_kernel_vector 
    {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

template <typename T, typename Allocator>
inline detail::kernel_2d<T, Allocator> get_identity_kernel()
{
    detail::kernel_2d<T, Allocator> kernel(1, 0, 0);
    kernel[0] = 1;
    return kernel;
}

// Please refer https://stackoverflow.com/a/10032882/14958679 for getting an overview of the
// concept applied for obtaining higher order Sobel kernels.

/// \defgroup KernelGeneration
/// \brief Contains documentation for functions used in kernel generation.
///
/// Separate functions are used for generating only those kernels whose dimensions are greater than
/// 5x5. Smaller kernels are fed directly to the algorithm.
///

/// \addtogroup KernelGeneration
/// @{

/// \brief Fills kernel vector given as argument with a second order kernel in horizontal or
/// vertical direction. The type of the kernel which is to be used for filling will be indicated 
/// by the variable 'type'.
/// \param kernel - Kernel vector which will be filled.
/// \param type - Indicates the type of second order derivative kernel which is to be filled inside
/// first argument.
inline void kernel_buffer_fill(boost::gil::gray8_view_t buffer_kernel_view, kernel_type type)
{
    if (type == kernel_type::sobel_dx)
    {
        for (std::ptrdiff_t row = 0; row < 3; ++row)
        {
            for (std::ptrdiff_t col = 0; col < 3; ++col)
            {
                buffer_kernel_view(col, row)[0] = dx_sobel[3 * row + col];
            }
        }
    }
    else if (type == kernel_type::sobel_dy)
    {
        for (std::ptrdiff_t row =0; row < 3; ++row)
        {
            for (std::ptrdiff_t col = 0; col < 3; ++col)
            {
                buffer_kernel_view(col, row)[0] = dy_sobel[3 * row + col];
            }
        }
    }
}

/// \brief Passes parameters to 'kernel_convolve_impl()' repeatedly until kernel vector of desired
/// order is obtained.
/// \param order - Indicates order of derivative whose kernel vector is to be returned.
/// \param type - Indicates the type of kernel vector which is to be returned.
inline auto kernel_convolve(unsigned int order_x, unsigned int order_y, unsigned int dimension)
    -> std::vector<float>
{
    unsigned int smooth_count = dimension - 2 * (order_x + order_y + 1);
    double const order_x_repetition = static_cast<unsigned int>(std::log2(order_x - 1));
    double const order_y_repetition = static_cast<unsigned int>(std::log2(order_y - 1));
    double const smooth_repetition = static_cast<unsigned int>(std::log2(smooth_count - 1));
    boost::gil::gray32_image_t buffer_kernel_img_x(3, 3);
    boost::gil::gray32_image_t buffer_kernel_img_y(3, 3);
    boost::gil::gray32_image_t buffer_xy_image(dimension, dimension);
    boost::gil::gray32_image_t intermediate_img(3, 3);

    boost::gil::gray32_view_t buffer_kernel_view_x = boost::gil::view(buffer_kernel_img_x);
    boost::gil::gray32_view_t buffer_kernel_view_y = boost::gil::view(buffer_kernel_img_y);
    boost::gil::gray32_view_t buffer_xy_view = boost::gil::view(buffer_xy_image);

    if (order_x)
    {
        kernel_buffer_fill(buffer_kernel_view_x, kernel_type::sobel_dx);
        double const order_x_decrease = std::pow(2, order_x_repetition);
        boost::gil::detail::kernel_2d<float> kernel(dx_sobel.begin(),
            dx_sobel.size(), 1, 1);

        copy_pixels(buffer_kernel_view_x, view(intermediate_img));
        
        for (unsigned int i = 0; i < order_x_repetition; ++i)
        {
            boost::gil::detail::kernel_2d<float> kernel_view(buffer_kernel_view_x.begin(),
                buffer_kernel_view_x.size(), 1, 1);
            boost::gil::detail::convolve_2d(buffer_kernel_view_x, kernel_view,
                view(intermediate_img));
            copy_pixels(view(intermediate_img), buffer_kernel_view_x);
        }

        for (unsigned int i = 0; i < order_x - 1 - order_x_decrease; ++i)
        {
            boost::gil::detail::convolve_2d(buffer_kernel_view_x, kernel, view(intermediate_img));
            copy_pixels(view(intermediate_img), buffer_kernel_view_x);
        }
    }

    if (order_y)
    {
        kernel_buffer_fill(buffer_kernel_view_y, kernel_type::sobel_dy);
        double const order_y_decrease = std::pow(2, order_y_repetition);
        boost::gil::detail::kernel_2d<float> kernel(dy_sobel.begin(),
            dy_sobel.size(), 1, 1);

        copy_pixels(buffer_kernel_view_y, view(intermediate_img));

        for(unsigned int i = 0; i < order_y_repetition; ++i)
        {
            boost::gil::detail::kernel_2d<float> kernel_view(buffer_kernel_view_y.begin(),
                buffer_kernel_view_y.size(), 1, 1);
            boost::gil::detail::convolve_2d(buffer_kernel_view_y, kernel_view,
                view(intermediate_img));
            copy_pixels(view(intermediate_img), buffer_kernel_view_y);
        }

        for (unsigned int i = 0; i < order_y - 1 - order_y_decrease; ++i)
        {
            boost::gil::detail::convolve_2d(buffer_kernel_view_y, kernel, view(intermediate_img));
            copy_pixels(view(intermediate_img), buffer_kernel_view_y);
        }
    }

    unsigned int convolve_start = dimension / 2 - 1, convolve_end = dimension / 2 - 1;

    if(order_x && order_y)
    {
        boost::gil::detail::kernel_2d<float> kernel_y(buffer_kernel_view_y.begin(),
            buffer_kernel_view_y.size(), 1, 1);
        boost::gil::detail::convolve_2d(buffer_kernel_view_x, kernel_y,
            boost::gil::subimage_view(buffer_xy_image, convolve_start, convolve_start,
            convolve_end, convolve_end));
    }

    boost::gil::detail::kernel_2d<float> smoothing_kernel(smoothing_kernel_vector.begin(),
        smoothing_kernel_vector.size(), 1, 1);



    // for(unsigned int i = 0; i < smooth_count - 1 - smooth_repetition; ++i)
    // {
    //     --convolve_start, ++convolve_end;
    //     boost::gil::detail::convolve_2d(subimage_view(buffer_xy, convolve_start, convolve_start, 
    //         convolve_end, convolve_end), smoothing_kernel, boost::gil::subimage_view(
    //         buffer_xy, convolve_start, convolve_start, convolve_end, convolve_end));
    // }

    // std::vector<std::vector<float>> smoothing_dummy = smoothing_kernel;

    // // Variable 'smooth_repetition' will store the number of times we need to convolve 
    // // 'smoothing_dummy' with itself. This number when used as a power of 2 in its exponentiation,
    // // will result in a number which is the largest power of 2 smaller than 'order - 2'.
    

    // for (unsigned int i = 0; i < smooth_repetition; ++i)
    // {
    //     smoothing_dummy = kernel_convolve_impl(smoothing_dummy, smoothing_dummy);
    // }

    // convolved_kernel = kernel_convolve_impl(convolved_kernel, smoothing_dummy);

    // // Variable 'order_decrease' will store the amount of decrease in order obtained due to the 
    // // above optimization. It stores the largest power of 2 smaller than 'order - 2'.
    // double const order_decrease = std::pow(2, smooth_repetition);
    // for (unsigned int i = 0; i < order - 2 - order_decrease; ++i)
    // {
    //     convolved_kernel = kernel_convolve_impl(convolved_kernel, smoothing_kernel);
    // }

    std::vector<float> ans;

    for (std::ptrdiff_t row = 0; row < static_cast<std::ptrdiff_t>(buffer_xy_view.height()); ++row)
    {
        for (std::ptrdiff_t col = 0; col < static_cast<std::ptrdiff_t>(buffer_xy_view.width());
            ++col)
        {
            ans.push_back(buffer_xy_view(col, row)[0]);
        }
    }

    return ans;
}
/// @}
}}} // namespace boost::gil::detail
 
#endif
