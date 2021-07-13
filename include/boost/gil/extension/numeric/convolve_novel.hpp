#ifndef BOOST_GIL_EXTENSION_NUMERIC_CONVOLVE_NOVEL_HPP
#define BOOST_GIL_EXTENSION_NUMERIC_CONVOLVE_NOVEL_HPP

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
#include <iostream>

#include "immintrin.h" // for AVX 
#include <malloc.h>

namespace boost { namespace gil { namespace detail { 

template <typename SrcView, typename DstView, typename Kernel>
void correlate_2d_impl(SrcView const& src_view, DstView const& dst_view, Kernel const& kernel)
{
    int row_boundary, col_boundary;
    float aux_total;
    for (std::ptrdiff_t view_row = 0; view_row < src_view.height(); ++view_row)
    {
        for (std::ptrdiff_t view_col = 0; view_col < src_view.width(); ++view_col)
        {
            aux_total = 0.0f;
            for (std::size_t kernel_row = 0; kernel_row < kernel.size(); ++kernel_row)
            {
                for (std::size_t kernel_col = 0; kernel_col < kernel.size(); ++kernel_col)
                {
                    // index of input signal, used for checking boundary
                    if (kernel_row <= kernel.center_y())
                        row_boundary = view_row - (kernel.center_y() - kernel_row);
                    else 
                        row_boundary = view_row + kernel_row - 1;

                    if (kernel_col <= kernel.center_x())
                        col_boundary = view_col - (kernel.center_x() - kernel_col);
                    else 
                        col_boundary = view_col + kernel_col - 1;

                    // ignore input samples which are out of bound
                    if (row_boundary >= 0 && row_boundary < src_view.height() &&
                        col_boundary >= 0 && col_boundary < src_view.width())
                    {
                        aux_total +=
                            src_view(col_boundary, row_boundary) *
                            kernel.at(kernel_row, kernel_col);
                    }
                }
            }
            dst_view(view_col, view_row) = aux_total;
        }
    }
}

template <typename SrcView, typename DstView, typename Kernel>
void correlate_2d(SrcView const& src_view, Kernel const& kernel, DstView const& dst_view)
{
    BOOST_ASSERT(src_view.dimensions() == dst_view.dimensions());
    BOOST_ASSERT(kernel.size() != 0);

    gil_function_requires<ImageViewConcept<SrcView>>();
    gil_function_requires<MutableImageViewConcept<DstView>>();
    static_assert(color_spaces_are_compatible
    <
        typename color_space_type<SrcView>::type,
        typename color_space_type<DstView>::type
    >::value, "Source and destination views must have pixels with the same color space");

    for (std::size_t i = 0; i < src_view.num_channels(); i++)
    {
        detail::correlate_2d_impl(
            nth_channel_view(src_view, i),
            nth_channel_view(dst_view, i),
            kernel
        );
    }
}

template <typename SrcView, typename DstView>
void image_correlate(SrcView src_view, std::vector<float> kernel, DstView dst_view) 
{
    long long kernel_dimension = std::sqrt(kernel.size());
    auto img_in_modified_col = gil::extend_col(src_view, kernel_dimension / 2,
        gil::boundary_option::extend_zero);
    auto img_in_modified = gil::extend_row(gil::view(img_in_modified_col), kernel_dimension / 2, 
        gil::boundary_option::extend_zero);

    using pixel_t = typename SrcView::value_type;
    pixel_t zero_pixel;
    pixel_zeros_t<pixel_t>()(zero_pixel);

    using pixel_t = typename SrcView::value_type;
    std::vector<pixel_t> buffer(kernel_dimension * gil::view(img_in_modified).width());

    auto buffer_view = gil::transposed_view(gil::subimage_view(gil::view(img_in_modified), 
            0, 0, gil::view(img_in_modified).width(), kernel_dimension));

    std::copy(buffer_view.begin(), buffer_view.end(), buffer.begin());

    for (std::ptrdiff_t row = 0; 
        row <= gil::view(img_in_modified).height() - kernel_dimension; ++row)
    {
        if (row)
        {
            for (std::ptrdiff_t buffer_index = 0, view_col = 0; buffer_index < buffer.size(); 
                buffer_index += kernel_dimension, ++view_col)
            {
                std::rotate(buffer.begin() + buffer_index, buffer.begin() + buffer_index + 1, 
                    buffer.begin() + buffer_index + kernel_dimension);
                buffer[buffer_index + kernel_dimension - 1] = 
                    gil::view(img_in_modified)(view_col, row + kernel_dimension - 1);
            }
        }

        __m128* kernel_broadcasted = (__m128* )(alloca(16 * kernel.size()));
        __m128 acc0 = _mm_setzero_ps(), acc1 = _mm_setzero_ps();
        __m128 acc2 = _mm_setzero_ps(), acc3 = _mm_setzero_ps();
        __m128 block0, block1, block2, block3;
        __m128 prod0, prod1, prod2, prod3;
        
        for (int i = 0; i < kernel.size(); ++i)
            kernel_broadcasted[i] = _mm_set1_ps(kernel[i]);

        for (std::ptrdiff_t index = 0; index < buffer.size() - 13; index += 16)
        {
            for (std::ptrdiff_t start_k = 0; start_k < 9; ++start_k)
            {
                auto pointer0 = &buffer[index + start_k], pointer1 = &buffer[index + start_k + 1];
                auto pointer2 = &buffer[index + start_k + 2], pointer3 = &buffer[index + start_k + 3];

                block0 = _mm_loadu_ps((float *)pointer0);
                block1 = _mm_loadu_ps((float *)pointer1);
                block2 = _mm_loadu_ps((float *)pointer2);
                block3 = _mm_loadu_ps((float *)pointer3);

                prod0 = _mm_mul_ps(block0, kernel_broadcasted[start_k]);
                prod1 = _mm_mul_ps(block1, kernel_broadcasted[start_k]);
                prod2 = _mm_mul_ps(block2, kernel_broadcasted[start_k]);
                prod3 = _mm_mul_ps(block3, kernel_broadcasted[start_k]);

                acc0 = _mm_add_ps(acc0, prod0);
                acc1 = _mm_add_ps(acc1, prod1);
                acc2 = _mm_add_ps(acc2, prod2);
                acc3 = _mm_add_ps(acc3, prod3);
            }
            float ans_dummy0[4], ans_dummy1[4], ans_dummy2[4], ans_dummy3[4];
            _mm_storeu_ps(ans_dummy0, acc0);
            _mm_storeu_ps(ans_dummy1, acc1);
            _mm_storeu_ps(ans_dummy2, acc2);
            _mm_storeu_ps(ans_dummy3, acc3);
        }

    }
}
} } } // namespace boost::gil::detail

#endif
