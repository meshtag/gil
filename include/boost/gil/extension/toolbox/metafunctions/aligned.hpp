// Copyright Tom Brinkman 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef _aligned_hpp_
#define _aligned_hpp_

#include <boost/gil.hpp>

namespace boost  {  namespace gil  {

/// \brief Aligns the image view passed through struct constructor in a specific direction
/// inside the image view passed through the overloaded '()' operator. The direction of 
/// alignment is specified by constructor parameters.
template <typename view_t>
struct aligned
{
    // Following enum variables will be used for specifying alignment of view passed through
    // constructor with respect to the view passed through overloaded '()' operator.
    enum
    {
        left = (0x1 << 0),
        center = (0x1 << 1),
        right = (0x1 << 2),
        top = (0x1 << 3),
        middle = (0x1 << 5),
        bottom = (0x1 << 7),
    };

    view_t& v2;
    int align;
    aligned(view_t v2, int align = center | middle)
        : v2(v2)
        , align(align)
    {
    }

    void operator()(view_t& view)
    {
        using namespace boost::gil;

        std::size_t w = v2.width(), h = v2.height();

        // For ensuring that view passed through '()' operator has dimensions greater than or
        // equal to the dimensions of view passed through constructor.
        if(h > view.height() || w > view.width())
        {
            throw std::length_error("Image view passed through overloaded '()' operator must have"
            " dimensions greater than or equal to the dimensions of image view passed through"
            " struct constructor");
        }

        std::ptrdiff_t x = 0;
        if(align & center)
            x = (view.width() - w) / 2;
        else if(align & right)
            x = view.width() - w;

        std::ptrdiff_t y = 0;
        if(align & middle)
            y = (view.height() - h) / 2;
        else if(align & bottom)
            y = view.height() - h;

        view_t v3 = subimage_view(view, x, y, w, h);
        copy_pixels(v2, v3);
    }
};
}  }
#endif
