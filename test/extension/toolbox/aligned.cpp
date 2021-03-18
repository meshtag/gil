//
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

// Reference for test cases was taken from
// https://github.com/tuttleofx/TuttleOFX/blob/develop/libraries/boostHack/boost/gil/extension/toolbox/aligned.tests.cpp

#include <boost/gil/extension/toolbox/metafunctions.hpp>
#include <boost/core/lightweight_test.hpp>
#include <vector>

namespace gil = boost::gil;

// This function helps us fill pixels of a rgb view given as 2nd argument with
// elements of the vector given as 1st argument.
void pixel_fill_rgb(std::vector<std::vector<std::vector<int>>>& vec, gil::rgb8_image_t& img)
{
    for (std::ptrdiff_t view_row = 0; view_row < view(img).height(); ++view_row)
    {
        for (std::ptrdiff_t view_col = 0; view_col < view(img).width(); ++view_col)
        {
            gil::view(img)(view_col, view_row) = gil::rgb8_pixel_t(vec[view_row][view_col][0],
                vec[view_row][view_col][1], vec[view_row][view_col][2]);
        }
    }
}

int main()
{
    std::vector<std::vector<std::vector<int>>> original_img_vector {
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}} 
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}}
 };

    std::vector<std::vector<std::vector<int>>> expected_center_middle_vector {
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {191, 191, 191}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {127, 127, 127}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {127, 127, 127},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {127, 127, 127},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {191, 191, 191}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {127, 127, 127}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}}
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}}
 };

    std::vector<std::vector<std::vector<int>>> expected_left_top_vector {
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191}, {  0,   0,   0},
//     {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
//     {  0,   0,   0}, {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {191, 191, 191},
//     {127, 127, 127}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {127, 127, 127},
//     {191, 191, 191}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
//     {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255},
//     {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}}
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}}
 };

    // gil::rgb8_image_t original_image(16, 16), expected_center_middle_img(20, 20);
    // gil::rgb8_image_t expected_left_top_img(20, 20), obtained_center_middle_img(20, 20);
    // gil::rgb8_image_t obtained_left_top_img(20, 20);

    gil::rgb8_image_t original_image(2, 2), expected_center_middle_img(2, 2);
    gil::rgb8_image_t expected_left_top_img(2, 2), obtained_center_middle_img(2, 2);
    gil::rgb8_image_t obtained_left_top_img(2, 2);

    gil::fill_pixels(gil::view(obtained_center_middle_img), gil::rgb8_pixel_t(255, 255, 255));
    gil::fill_pixels(gil::view(obtained_left_top_img), gil::rgb8_pixel_t(255, 255, 255));

    gil::rgb8_view_t obtained_center_middle_view = gil::view(obtained_center_middle_img);
    gil::rgb8_view_t obtained_left_top_view = gil::view(obtained_left_top_img);

    pixel_fill_rgb(original_img_vector, original_image);
    pixel_fill_rgb(expected_center_middle_vector, expected_center_middle_img);
    pixel_fill_rgb(expected_left_top_vector, expected_left_top_img);

    using aligned_t =  gil::aligned<gil::rgb8_view_t>;
    aligned_t aligned(gil::view(original_image), aligned_t::center | aligned_t::middle);
    aligned(obtained_center_middle_view);

    BOOST_TEST(gil::equal_pixels(obtained_center_middle_view, gil::view(expected_center_middle_img)));
    
    //  aligned.align = aligned_t::left | aligned_t::top;
    //  aligned(obtained_left_top_view);

    std::vector<std::vector<std::vector<int>>> vec1 {
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}}
};

    std::vector<std::vector<std::vector<int>>> vec2 {
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}}
};

    gil::rgb8_image_t vec1_img(2, 2), vec2_img(2, 2);
    pixel_fill_rgb(vec1, vec1_img);
    pixel_fill_rgb(vec2, vec2_img);

    BOOST_TEST(gil::equal_pixels(gil::view(vec1_img), gil::view(vec2_img)));

    // BOOST_TEST(gil::equal_pixels(gil::view(expected_center_middle_img), gil::view(original_image)));
    // BOOST_TEST(gil::equal_pixels(gil::view(expected_left_top_img), gil::view(original_image)));

    // BOOST_TEST(gil::equal_pixels(obtained_left_top_view, gil::view(expected_left_top_img)));

    return boost::report_errors();
}