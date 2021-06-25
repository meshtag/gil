#include <celero/Celero.h>
#include <vector>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>
#include <boost/gil/extension/numeric/convolve_novel.hpp>
#include <boost/gil/extension/io/png.hpp>

#include "/usr/include/opencv4/opencv2/imgproc.hpp"
#include "/usr/include/opencv4/opencv2/imgcodecs.hpp"
#include "/usr/include/opencv4/opencv2/highgui.hpp"
namespace opencv = cv;
using namespace cv;

#include <boost/gil/extension/io/jpeg.hpp>
using namespace boost::gil;
using namespace std;

namespace gil = boost::gil;

class RandomImageGray8Fixture : public celero::TestFixture
{
public:

    void setUp(const celero::TestFixture::ExperimentValue& experimentValue) final
    {
        gil::read_image("resize_32_gray.png", img_32, gil::png_tag{});
        gil::read_image("resize_256_gray.png", img_256, gil::png_tag{});
        gil::read_image("resize_512_gray.png", img_512, gil::png_tag{});
        gil::read_image("resize_1024_gray.png", img_1024, gil::png_tag{});
        gil::read_image("resize_2048_gray.png", img_2048, gil::png_tag{});

        this -> img_out_32.recreate(this -> img_32.dimensions());
        this -> img_out_256.recreate(this -> img_256.dimensions());
        this -> img_out_512.recreate(this -> img_512.dimensions());
        this -> img_out_1024.recreate(this -> img_1024.dimensions());
        this -> img_out_2048.recreate(this -> img_2048.dimensions());

        this -> img_out1_32.recreate(this -> img_32.dimensions());
        this -> img_out1_256.recreate(this -> img_256.dimensions());
        this -> img_out1_512.recreate(this -> img_512.dimensions());
        this -> img_out1_1024.recreate(this -> img_1024.dimensions());
        this -> img_out1_2048.recreate(this -> img_2048.dimensions());

        // Loads an image
        src_32 = opencv::imread(opencv::samples::findFile( imageName_32), opencv::IMREAD_COLOR);
        src_256 = opencv::imread(opencv::samples::findFile( imageName_256), opencv::IMREAD_COLOR);
        src_512 = opencv::imread(opencv::samples::findFile( imageName_512), opencv::IMREAD_COLOR);
        src_1024 = opencv::imread(opencv::samples::findFile( imageName_1024), opencv::IMREAD_COLOR);
        src_2048 = opencv::imread(opencv::samples::findFile( imageName_2048), opencv::IMREAD_COLOR);
        anchor = Point( -1, -1 );
        delta = 0;
        ddepth = -1;
        // Loop - Will filter the image with different kernel sizes each 0.5 seconds
        int ind = 0;
        // Update kernel size for a normalized box filter
        kernel_size = 3 ;
        kernel1 = Mat::ones( kernel_size, kernel_size, CV_32F );
    }

    gil::gray8_image_t img_32, img_256, img_512, img_1024, img_2048;
    gil::gray8_image_t img_out_32, img_out_256, img_out_512, img_out_1024, img_out_2048;
    gil::gray8_image_t img_out1_32, img_out1_256, img_out1_512, img_out1_1024, img_out1_2048;
    std::vector<float> v = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    gil::detail::kernel_2d<float> kernel{gil::detail::kernel_2d<float>(v.begin(), v.size(), 1, 1)};

    opencv::Mat src_32, src_256, src_512, src_1024, src_2048, dst;
    opencv::Mat kernel1;
    opencv::Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    const char* window_name = "filter2D Demo";
    const char* imageName_32 = "resize_32_gray.png";
    const char* imageName_256 = "resize_256_gray.png";
    const char* imageName_512 = "resize_512_gray.png";
    const char* imageName_1024 = "resize_1024_gray.png";
    const char* imageName_2048 = "resize_2048_gray.png";
};

#ifdef NDEBUG
constexpr int samples_num = 30; // Celero default minimum
constexpr int iterations = 100;
// #define IMG_32
#define IMG_256
// #define IMG_512
// #define IMG_1024
// #define IMG_2048
#else
constexpr int samples_num = 1;
constexpr int iterations = 1;
#define IMG_32
#endif



#ifdef IMG_32
BASELINE_F(Correlate2D_32, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_32), this -> kernel, 
        gil::view(this -> img_out_32));
}

BENCHMARK_F(Correlate2D_32, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_32), this -> kernel, 
        gil::view(this -> img_out1_32));
}

BENCHMARK_F(Correlate2D_32, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_32, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_256
BASELINE_F(Correlate2D_256, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_256), this -> kernel, 
        gil::view(this -> img_out_256));
}

BENCHMARK_F(Correlate2D_256, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_256), this -> kernel, 
        gil::view(this -> img_out1_256));
}

BENCHMARK_F(Correlate2D_256, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_256, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifndef IMG_512
// BENCHMARK_F(Correlate2D_512, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
// {
//     gil::detail::correlate_2d(gil::const_view(this -> img_512), this -> kernel, 
//         gil::view(this -> img_out_512));
// }

// BENCHMARK_F(Correlate2D_512, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
// {
//     gil::detail::image_correlate(gil::const_view(this -> img_512), this -> kernel, 
//         gil::view(this -> img_out1_512));
// }

// BENCHMARK_F(Correlate2D_512, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
// {
//     opencv::filter2D(src_512, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT);
// }
#endif

#ifdef IMG_1024
BASELINE_F(Correlate2D_1024, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_1024), this -> kernel, 
        gil::view(this -> img_out_1024));
}

BENCHMARK_F(Correlate2D_1024, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_1024), this -> v, 
        gil::view(this -> img_out1_1024));
}

BENCHMARK_F(Correlate2D_1024, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_1024, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_2048
BASELINE_F(Correlate2D_2048, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_2048), this -> kernel, 
        gil::view(this -> img_out_2048));
}

BENCHMARK_F(Correlate2D_2048, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_2048), this -> kernel, 
        gil::view(this -> img_out1_2048));
}

BENCHMARK_F(Correlate2D_2048, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_2048, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT);
}
#endif

CELERO_MAIN
