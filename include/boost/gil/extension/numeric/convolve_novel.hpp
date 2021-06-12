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
                // flip_ker_row = kernel.size() - 1 - kernel_row;      // row index of flipped kernel

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
} } } // namespace boost::gil::detail

#endif