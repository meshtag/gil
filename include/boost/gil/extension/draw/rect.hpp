#ifndef BOOST_GIL_EXTENSION_DRAW_CIRCLE_AND_RECT_HPP
#define BOOST_GIL_EXTENSION_DRAW_CIRCLE_AND_RECT_HPP
#include <math.h>

// (tl0,tl1) -> top left corner co-ordinates
// (br0,br1) -> bottom right corner co-ordinates
template <typename SrcView>
void draw_rect_impl(SrcView const& src_view, const int tlc0, const int tlc1, const int brc0, const int brc1,const int colour, const int thickness)
{
    //Following four loops draw edges of the rectangle.
    for(std::ptrdiff_t view_col = tlc0,view_row = tlc1;view_col <= brc0;++view_col)
        for(int i = 0;i < thickness;++i)
            src_view(view_col,view_row - i) = colour;
    for(std::ptrdiff_t view_row = tlc1,view_col = brc0;view_row <= brc1;++view_row)
        for(int i = 0;i < thickness;++i)
            src_view(view_col + i,view_row) = colour;
    for(std::ptrdiff_t view_row = tlc1,view_col = tlc0;view_row <= brc1;++view_row)
        for(int i = 0;i < thickness;++i)
            src_view(view_col - i,view_row) = colour;
    for(std::ptrdiff_t view_col = tlc0,view_row = brc1;view_col <= brc0;++view_col)
        for(int i = 0;i < thickness;++i)
            src_view(view_col,view_row + i) = colour;
    std::ptrdiff_t view_col = brc0,view_row = brc1;
    // Following loop completes the bottom right part of the rectangle.
    for(int j = 0;j<thickness;++j)
    {
        for(int i = 0;i < thickness;++i)
            src_view(view_col,view_row + i) = colour;
        ++view_col;
    }
    view_col = tlc0,view_row = tlc1;
    // Following loop completes the top left part of the rectangle.
    for(int j = 0;j<thickness;++j)
    {
        for(int i = 0;i < thickness;++i)
            src_view(view_col,view_row - i) = colour;
        --view_col;
    }
    view_col = brc0,view_row = tlc1;
    // Following loop completes the top right part of the rectangle.
    for(int j = 0;j<thickness;++j)
    {
        for(int i = 0;i < thickness;++i)
            src_view(view_col + i,view_row) = colour;
        --view_row;
    }
    view_col = tlc0,view_row = brc1;
    // Following loop completes the bottom left part of the rectangle.
    for(int j = 0;j<thickness;++j)
    {
        for(int i = 0;i < thickness;++i)
            src_view(view_col - i,view_row) = colour;
        ++view_row;
    }
}

template <typename SrcView>
void draw_rect(SrcView const& src_view, const int tlc0, const int tlc1, const int brc0, const int brc1, const int r,const int g,const int b,const int thickness)
{
    int colours[] = {r,g,b};
    for (std::size_t i = 0; i < src_view.num_channels(); i++)
    {
            draw_rect_impl(
            nth_channel_view(src_view, i),
            tlc0,tlc1,brc0,brc1,colours[i],thickness
        );
    }
}
#endif //BOOST_GIL_EXTENSION_DRAW_CIRCLE_AND_RECT_HPP 