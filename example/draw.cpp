#include <boost/gil/extension/io/png.hpp>
#include<boost/gil/extension/draw/rect.hpp>

int main()
{
    using namespace boost::gil;
    rgb8_image_t img;
    read_image("draw.png", img, png_tag{});
    rgb8_image_t img_rect = img,img_circle = img;
    //draw_rect(Input_image_view,x_co-ordinate_of_top_left_corner,y_co_ordinate_of_top_left_corner,
    //x_co-odinate_of_bottom_right_corner,y_co-ordinate_of_bottom_right_corner,
    //desired_colour_of_border_in_r,g,b_format,thickness of border)
    draw_rect(view(img_rect),275,300,600,600,0,0,105,30);
    write_view("out-draw_add_rectangle.png", view(img_rect), png_tag{});
}