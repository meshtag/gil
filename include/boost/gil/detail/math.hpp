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
    smoothing
};

static constexpr double pi = 3.14159265358979323846;

static constexpr std::array<float, 9> dx_sobel = {{-1, 0, 1, -2, 0, 2, -1, 0, 1}};
static constexpr std::array<float, 25> dx_sobel_2_5 = {
    1, 0, -2, 0, 1, 4, 0, -8, 0, 4, 6, 0, -12, 0, 6, 4, 0, -8, 0, 4, 1, 0, -2, 0, 1
};
// In variable name "dx_sobel_2_5", "2" indicates that the order of Sobel derivative in x-direction 
// is 2 and "5" indicates that the dimension of kernel created from this array will be 5x5.
static constexpr std::array<float, 9> dx_scharr = {{-1, 0, 1, -1, 0, 1, -1, 0, 1}};
static constexpr std::array<float, 9> dy_sobel = {{1, 2, 1, 0, 0, 0, -1, -2, -1}};
static constexpr std::array<float, 25> dy_sobel_2_5 = {
    1, 4, 6, 4, 1, 0, 0, 0, 0, 0, -2, -8, -12, -8, -2, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1
};
// In variable name "dy_sobel_2_5", "2" indicates that the order of Sobel derivative in y-direction 
// is 2 and "5" indicates that the dimension of kernel created from this array will be 5x5.
static constexpr std::array<float, 9> dy_scharr = {{1, 1, 1, 0, 0, 0, -1, -1, -1}};
static constexpr std::array<float, 9> smoothing_kernel_vector = {1, 2, 1, 2, 4, 2, 1, 2, 1};

template <typename T, typename Allocator>
inline detail::kernel_2d<T, Allocator> get_identity_kernel()
{
    detail::kernel_2d<T, Allocator> kernel(1, 0, 0);
    kernel[0] = 1;
    return kernel;
}

/// \defgroup Kernel_generation
/// \brief A set of helper functions used for generating kernels.

/// \brief Fills a gray32f_view_t view with a kernel specified by argument "type".
/// \ingroup Kernel_generation
/// \param view - Gil image view of type gray32f_view_t which will be filled with a kernel specified 
///               by variable "type".
/// \param type - Enum variable specifying the kernel which is to be filled inside "view".
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
    else if (type == kernel_type::smoothing)
    {
        for (std::ptrdiff_t row = 0; row < 3; ++row)
        {
            for (std::ptrdiff_t col = 0; col < 3; ++col)
            {
                view(col, row) = smoothing_kernel_vector[3 * row + col];
            }
        }
    }
}

/// \brief Converts a gray32f_view_t into a one dimensional kernel in a specific manner as required 
///        for creating a kernel from that vector.
/// \ingroup Kernel_generation
/// \param view - Gil image view of type gray32f_view_t which will be converted into a one 
///               dimensional kernel vector for creating a kernel from it.
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

/// \brief Performs convolution between a gray32f_view_t view and a kernel created from another 
///        gray32f_view_t view using the function "gray32f_view_to_1d_vector()".
/// \ingroup Kernel_generation
/// \param view1 - Gil image view of type gray32f_view_t which will be used as first argument for 
///                convolution operation.
/// \param view2 - Gil image view of type gray32f_view_t which will be used for creating a two 
///                dimensional kernel. This kernel will then be used as second argument in 
///                convolution operation.
/// \param dst_view - Gil image view of type gray32f_view_t which will be used for storing the 
///                   result obtained after applying the convolution operation.
inline void view_convolve(gil::gray32f_view_t view1, gil::gray32f_view_t view2, 
    gil::gray32f_view_t dst_view)
{
    std::vector<float> kernel_vector = gray32f_view_to_1d_vector(view2);
    detail::kernel_2d<float> kernel(kernel_vector.begin(), kernel_vector.size(),
        view2.height() / 2, view2.width() / 2);
    detail::convolve_2d(view1, kernel, dst_view);
}

/// \brief Function used for obtaining Sobel kernels.
/// \ingroup Kernel_generation
/// \param order - Contains desired order of Sobel derivative in x and y direction respectively.
/// \param size_desired - Optional argument which specifies the desired size of resultant Sobel 
///                       kernel.
inline auto get_sobel_kernel(std::array<unsigned int, 2> const order,
    unsigned int const size_desired = -1) -> std::vector<float>
{
    unsigned int const x_size = order[0] ? 2 * order[0] + 1 : 0;
    unsigned int const y_size = order[1] ? 2 * order[1] + 1 : 0;
    unsigned int size;
    if (order[0] && order[1] && size_desired == -1)
    {
        size = x_size + y_size - 1;
    }
    else if ((order[0] == 0 || order[1] == 0) && size_desired == -1)
    {
        size = x_size ? x_size : y_size;
    }
    else if (size_desired != -1)
    {
        size = size_desired;
    }

    if (size > 19)
    {
        throw std::length_error("Kernels with size greater than 19x19 are not yet supported\n");
    }

    gil::gray32f_image_t kernel_x_1(3, 3), kernel_y_1(3, 3), resultant_kernel(size, size);
    gil::gray32f_image_t resultant_kernel_y(y_size, y_size);

    if (order[0])
    {
        double const x_repetition = static_cast<unsigned int>(std::log2(order[0] - 1));
        double const x_decrease = std::pow(2, x_repetition);
        double convolve_count = x_decrease;
        unsigned int prev_size = 3;

        fill_kernel_view(view(kernel_x_1), kernel_type::sobel_dx);
        copy_pixels(view(kernel_x_1), subimage_view(view(resultant_kernel),
            size / 2 - 1, size / 2 - 1, 3, 3));
        for (unsigned int i = 0; i < x_repetition; ++i)
        {
            unsigned int intermediate_img_size = prev_size + std::pow(2, i + 1);
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_kernel),
                size / 2 - intermediate_img_size / 2, size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size), subimage_view(
                view(resultant_kernel), size / 2 - prev_size / 2, size / 2 - prev_size / 2,
                prev_size, prev_size), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel),
                size / 2 - intermediate_img_size / 2, size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size));
            prev_size = intermediate_img_size;
        }
        for (unsigned int i = 0; i < order[0] - x_decrease; ++i)
        {
            ++convolve_count;
            unsigned int intermediate_img_size = 2 * convolve_count + 1;
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_kernel),
                size / 2 - convolve_count, size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size),
                view(kernel_x_1), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel),
                size / 2 - convolve_count, size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size));
        }
    }
    if (order[1])
    {
        double const y_repetition = static_cast<unsigned int>(std::log2(order[1] - 1));
        double const y_decrease = std::pow(2, y_repetition);
        double convolve_count = y_decrease;
        unsigned int prev_size = 3;

        fill_kernel_view(view(kernel_y_1), kernel_type::sobel_dy);
        copy_pixels(view(kernel_y_1), subimage_view(view(resultant_kernel_y),
            y_size / 2 - 1, y_size / 2 - 1, 3, 3));
        for (unsigned int i = 0; i < y_repetition; ++i)
        {
            unsigned int intermediate_img_size = prev_size + std::pow(2, i + 1);
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_kernel_y),
                y_size / 2 - intermediate_img_size / 2, y_size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size), subimage_view(
                view(resultant_kernel_y), y_size / 2 - prev_size / 2, y_size / 2 - prev_size / 2,
                prev_size, prev_size), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel_y),
                y_size / 2 - intermediate_img_size / 2, y_size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size));
            prev_size = intermediate_img_size;
        }
        for (unsigned int i = 0; i < order[1] - y_decrease; ++i)
        {
            ++convolve_count;
            unsigned int intermediate_img_size = 2 * convolve_count + 1;
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

    if(order[0] && order[1])
    {
        unsigned int intermediate_img_size = x_size + y_size - 1;
        unsigned int xy_combine_origin = size / 2 - x_size / 2 - y_size / 2;
        gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);

        view_convolve(subimage_view(view(resultant_kernel), xy_combine_origin, xy_combine_origin,
            intermediate_img_size, intermediate_img_size), view(resultant_kernel_y),
            view(intermediate_img));
        copy_pixels(view(intermediate_img), subimage_view(view(resultant_kernel),
            xy_combine_origin, xy_combine_origin, intermediate_img_size, intermediate_img_size));
    }
    else if (order[1])
    {
        unsigned int kernel_y_origin = size / 2 - y_size / 2;
        copy_pixels(view(resultant_kernel_y), subimage_view(view(resultant_kernel),
            kernel_y_origin, kernel_y_origin, y_size, y_size));
    }

    unsigned int smooth_count = (size - (x_size + y_size - 1)) / 2;

    if (size_desired != -1 && smooth_count)
    {
        unsigned int smoothing_kernel_size = 3 + 2 * (smooth_count - 1), prev_size = 3;
        gray32f_image_t smoothing_kernel(3, 3);
        gray32f_image_t resultant_smoothing_kernel(smoothing_kernel_size, smoothing_kernel_size);
        double const smooth_repetition = static_cast<unsigned int>(std::log(smooth_count));
        double const smooth_decrease = std::pow(2, smooth_repetition);
        double convolve_count = smooth_decrease;

        fill_kernel_view(view(smoothing_kernel), kernel_type::smoothing);
        copy_pixels(view(smoothing_kernel), subimage_view(view(resultant_smoothing_kernel), 
            smoothing_kernel_size / 2 - 1, smoothing_kernel_size / 2 - 1, 3, 3));
        for (unsigned int i = 0; i < smooth_repetition; ++i)
        {
            unsigned int intermediate_img_size = 3 + std::pow(2, i + 1);
            gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_smoothing_kernel),
                smoothing_kernel_size / 2 - intermediate_img_size / 2,
                smoothing_kernel_size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size),
                subimage_view(view(resultant_smoothing_kernel),
                smoothing_kernel_size / 2 - prev_size / 2,
                smoothing_kernel_size / 2 - prev_size / 2,
                prev_size, prev_size), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_smoothing_kernel),
                smoothing_kernel_size / 2 - intermediate_img_size / 2,
                smoothing_kernel_size / 2 - intermediate_img_size / 2,
                intermediate_img_size, intermediate_img_size));
            prev_size = intermediate_img_size;
        }
        for (unsigned int i = 0; i < smooth_count - smooth_decrease; ++i)
        {
            ++convolve_count;
            unsigned int intermediate_img_size = 2 * convolve_count + 1;
            gil::gray32f_image_t intermediate_img(intermediate_img_size, intermediate_img_size);
            view_convolve(subimage_view(view(resultant_smoothing_kernel),
                smoothing_kernel_size / 2 - convolve_count,
                smoothing_kernel_size / 2 - convolve_count, intermediate_img_size,
                intermediate_img_size), view(smoothing_kernel), view(intermediate_img));
            copy_pixels(view(intermediate_img), subimage_view(view(resultant_smoothing_kernel),
                smoothing_kernel_size / 2 - convolve_count,
                smoothing_kernel_size / 2 - convolve_count,
                intermediate_img_size, intermediate_img_size));
        }
        gray32f_image_t intermediate_img(size, size);
        view_convolve(subimage_view(view(resultant_kernel), 0, 0,
            size, size), view(resultant_smoothing_kernel),
            view(intermediate_img));
        copy_pixels(view(intermediate_img), view(resultant_kernel));
    }

    if (order[1] & 1)
    {
        view_multiplies_scalar<gray32f_pixel_t>(view(resultant_kernel), -1, view(resultant_kernel));
    }

    std::vector<float> resultant_kernel_flatten;
    for (int i = 0; i < view(resultant_kernel).height(); ++i)
    {
        for (int j = 0; j < view(resultant_kernel).width(); ++j)
        {
            resultant_kernel_flatten.push_back(view(resultant_kernel)(j, i)[0]);
        }
    }
    return resultant_kernel_flatten;
}

}}} // namespace boost::gil::detail

#endif
