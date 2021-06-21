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
    RandomImageGray8Fixture() 
    { }

    void setUp(const celero::TestFixture::ExperimentValue& experimentValue) final
    {
        gil::read_image("resize_256_gray.png", img_256, gil::png_tag{});
        gil::read_image("resize_512_gray.png", img_512, gil::png_tag{});
        gil::read_image("resize_1024_gray.png", img_1024, gil::png_tag{});
        gil::read_image("resize_2048_gray.png", img_2048, gil::png_tag{});

        this -> img_out_256.recreate(this -> img_256.dimensions());
        this -> img_out_512.recreate(this -> img_512.dimensions());
        this -> img_out_1024.recreate(this -> img_1024.dimensions());
        this -> img_out_2048.recreate(this -> img_2048.dimensions());

        this -> img_out1_256.recreate(this -> img_256.dimensions());
        this -> img_out1_512.recreate(this -> img_512.dimensions());
        this -> img_out1_1024.recreate(this -> img_1024.dimensions());
        this -> img_out1_2048.recreate(this -> img_2048.dimensions());

        // Loads an image
        src = opencv::imread( opencv::samples::findFile( imageName ), opencv::IMREAD_COLOR );
        anchor = Point( -1, -1 );
        delta = 0;
        ddepth = -1;
        // Loop - Will filter the image with different kernel sizes each 0.5 seconds
        int ind = 0;
        // Update kernel size for a normalized box filter
        kernel_size = 3 ;
        kernel1 = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    }

    gil::gray8_image_t img_32, img_256, img_512, img_1024, img_2048;
    gil::gray8_image_t img_out_32, img_out_256, img_out_512, img_out_1024, img_out_2048;
    gil::gray8_image_t img_out1_32, img_out1_256, img_out1_512, img_out1_1024, img_out1_2048;
    std::vector<float> v{std::vector<float>(9, 1.0f / 9.0f)};
    gil::detail::kernel_2d<float> kernel{gil::detail::kernel_2d<float>(v.begin(), v.size(), 3, 3)};

    opencv::Mat src, dst;
    opencv::Mat kernel1;
    opencv::Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    const char* window_name = "filter2D Demo";
    const char* imageName = "resize_512_gray.png";
};

#ifdef NDEBUG
constexpr int samples_num = 30; // Celero default minimum
constexpr int iterations = 100;
#else
constexpr int samples_num = 1;
constexpr int iterations = 1;
#endif

BASELINE_F(Correlate2D, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    // gil::detail::correlate_2d(gil::const_view(this -> img_256), this -> kernel, 
    //     gil::view(this -> img_out_256));
    gil::detail::correlate_2d(gil::const_view(this -> img_512), this -> kernel, 
        gil::view(this -> img_out_512));
    // gil::detail::correlate_2d(gil::const_view(this -> img_1024), this -> kernel, 
    //     gil::view(this -> img_out_1024));
    // gil::detail::correlate_2d(gil::const_view(this -> img_2048), this -> kernel, 
    //     gil::view(this -> img_out_2048));
}

// BENCHMARK_F(Correlate2D, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
// {
//     // gil::detail::image_correlate(gil::const_view(this -> img_256), this -> kernel, 
//     //     gil::view(this -> img_out1_256));
//     gil::detail::image_correlate(gil::const_view(this -> img_512), this -> kernel, 
//         gil::view(this -> img_out1_512));
//     // gil::detail::image_correlate(gil::const_view(this -> img_1024), this -> kernel, 
//     //     gil::view(this -> img_out1_1024));
//     // gil::detail::image_correlate(gil::const_view(this -> img_2048), this -> kernel, 
//     //     gil::view(this -> img_out1_2048));
// }

BENCHMARK_F(Correlate2D, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src, dst, ddepth , kernel1, anchor, delta, BORDER_DEFAULT );
}

CELERO_MAIN
