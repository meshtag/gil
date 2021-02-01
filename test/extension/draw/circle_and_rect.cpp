#include <boost/core/lightweight_test.hpp>
#include<boost/gil/extension/draw/circle_and_rect.hpp>
#include <boost/gil/extension/io/png.hpp>

namespace gil = boost::gil;

// This function helps us fill pixels of a view given as 2nd argument with
// elements of the vector given as 1st argument.
void pixel_fill(std::vector<std::vector<int>> &original_binary_vector,
                boost::gil::gray8_image_t &original_img) {
  for (std::ptrdiff_t view_row = 0; view_row < view(original_img).height();
       ++view_row) {
    for (std::ptrdiff_t view_col = 0; view_col < view(original_img).width();
         ++view_col) {
      view(original_img)(view_col, view_row) =
          gil::gray8_pixel_t(original_binary_vector[view_row][view_col]);
    }
  }
}

int main()
{
    std::vector<std::vector<int>> original_image_vector{
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> exp_rectangle_vector{
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}};
}