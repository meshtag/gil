#include <celero/Celero.h>
#include <vector>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>
#include <boost/gil/extension/numeric/convolve_novel.hpp>
#include <boost/gil/extension/io/png.hpp>

#include "/usr/include/opencv4/opencv2/imgproc.hpp"
#include "/usr/include/opencv4/opencv2/imgcodecs.hpp"
namespace opencv = cv;
using namespace cv;

using namespace boost::gil;
using namespace std;

#include <iostream>

namespace gil = boost::gil;

class RandomImageGray8Fixture : public celero::TestFixture
{
public:

    std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const final
    {
        int const initial_power = static_cast<int>(std::ceil(std::log(64) / std::log(2)));

        std::vector<celero::TestFixture::ExperimentValue> problem_space;
        problem_space.resize(6);
        for (int i = 0; i < 6; i++)
        {
            problem_space[i] = { int64_t(pow(2, initial_power + i)) };
        }
        return problem_space;
    }

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
        kernel_size = 3 ;
        kernel1_3x3 = Mat::ones(kernel_size, kernel_size, CV_8U);
        kernel1_7x7 = Mat::ones(7, 7, CV_8U);
    }

    gil::gray8_image_t img_32, img_256, img_512, img_1024, img_2048;
    gil::gray8_image_t img_out_32, img_out_256, img_out_512, img_out_1024, img_out_2048;
    gil::gray8_image_t img_out1_32, img_out1_256, img_out1_512, img_out1_1024, img_out1_2048;
    std::vector<float> v = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<float> k_7x7 = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    };
    gil::detail::kernel_2d<float> kernel_3x3{gil::detail::kernel_2d<float>(v.begin(), v.size(), 
        1, 1)};
    gil::detail::kernel_2d<float> kernel_7x7{gil::detail::kernel_2d<float>(k_7x7.begin(), 
        k_7x7.size(), 3, 3)};

    opencv::Mat src_32, src_256, src_512, src_1024, src_2048, dst;
    opencv::Mat kernel1_3x3, kernel1_7x7;
    opencv::Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    const char* imageName_32 = "resize_32_gray.png";
    const char* imageName_256 = "resize_256_gray.png";
    const char* imageName_512 = "resize_512_gray.png";
    const char* imageName_1024 = "resize_1024_gray.png";
    const char* imageName_2048 = "resize_2048_gray.png";
};

#ifdef NDEBUG
constexpr int samples_num = 30; // Celero default minimum
constexpr int iterations = 100;
#define IMG_32_3x3
#define IMG_256_3x3
#define IMG_512_3x3
#define IMG_1024_3x3
#define IMG_2048_3x3
#define IMG_32_7x7
#define IMG_256_7x7
#define IMG_512_7x7
#define IMG_1024_7x7
#define IMG_2048_7x7
#else
constexpr int samples_num = 1;
constexpr int iterations = 1;
#define IMG_32_3x3
#define IMG_32_7x7
#endif

#ifdef IMG_32_3x3
BASELINE_F(Correlate2D_32_3x3, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_32), this -> kernel_3x3, 
        gil::view(this -> img_out_32));
}

BENCHMARK_F(Correlate2D_32_3x3, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_32), this -> v, 
        gil::view(this -> img_out1_32));
}

BENCHMARK_F(Correlate2D_32_3x3, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_32, dst, ddepth , kernel1_3x3, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_32_7x7
BASELINE_F(Correlate2D_32_7x7, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_32), this -> kernel_7x7, 
        gil::view(this -> img_out_32));
}

BENCHMARK_F(Correlate2D_32_7x7, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_32), this -> k_7x7, 
        gil::view(this -> img_out1_32));
}

BENCHMARK_F(Correlate2D_32_7x7, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_32, dst, ddepth , kernel1_7x7, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_256_3x3
BASELINE_F(Correlate2D_256_3x3, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_256), this -> kernel_3x3, 
        gil::view(this -> img_out_256));
}

BENCHMARK_F(Correlate2D_256_3x3, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_256), this -> v, 
        gil::view(this -> img_out1_256));
}

BENCHMARK_F(Correlate2D_256_3x3, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_256, dst, ddepth , kernel1_3x3, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_256_7x7
BASELINE_F(Correlate2D_256_7x7, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_256), this -> kernel_7x7, 
        gil::view(this -> img_out_256));
}

BENCHMARK_F(Correlate2D_256_7x7, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_256), this -> k_7x7, 
        gil::view(this -> img_out1_256));
}

BENCHMARK_F(Correlate2D_256_7x7, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_256, dst, ddepth , kernel1_7x7, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_512_3x3
BASELINE_F(Correlate2D_512_3x3, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_512), this -> kernel_3x3, 
        gil::view(this -> img_out_512));
}

BENCHMARK_F(Correlate2D_512_3x3, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_512), this -> v, 
        gil::view(this -> img_out1_512));
}

BENCHMARK_F(Correlate2D_512_3x3, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_512, dst, ddepth , kernel1_3x3, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_512_7x7
BASELINE_F(Correlate2D_512_7x7, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_512), this -> kernel_7x7, 
        gil::view(this -> img_out_512));
}

BENCHMARK_F(Correlate2D_512_7x7, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_512), this -> k_7x7, 
        gil::view(this -> img_out1_512));
}

BENCHMARK_F(Correlate2D_512_7x7, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_512, dst, ddepth , kernel1_7x7, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_1024_3x3
BASELINE_F(Correlate2D_1024_3x3, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_1024), this -> kernel_3x3, 
        gil::view(this -> img_out_1024));
}

BENCHMARK_F(Correlate2D_1024_3x3, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_1024), this -> v, 
        gil::view(this -> img_out1_1024));
}

BENCHMARK_F(Correlate2D_1024_3x3, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_1024, dst, ddepth , kernel1_3x3, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_1024_7x7
BASELINE_F(Correlate2D_1024_7x7, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_1024), this -> kernel_7x7, 
        gil::view(this -> img_out_1024));
}

BENCHMARK_F(Correlate2D_1024_7x7, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_1024), this -> k_7x7, 
        gil::view(this -> img_out1_1024));
}

BENCHMARK_F(Correlate2D_1024_7x7, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_1024, dst, ddepth , kernel1_7x7, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_2048_3x3
BASELINE_F(Correlate2D_2048_3x3, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_2048), this -> kernel_3x3, 
        gil::view(this -> img_out_2048));
}

BENCHMARK_F(Correlate2D_2048_3x3, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_2048), this -> v, 
        gil::view(this -> img_out1_2048));
}

BENCHMARK_F(Correlate2D_2048_3x3, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_2048, dst, ddepth , kernel1_3x3, anchor, delta, BORDER_DEFAULT);
}
#endif

#ifdef IMG_2048_7x7
BASELINE_F(Correlate2D_2048_7x7, Gil_version, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::correlate_2d(gil::const_view(this -> img_2048), this -> kernel_7x7, 
        gil::view(this -> img_out_2048));
}

BENCHMARK_F(Correlate2D_2048_7x7, Second_modif, RandomImageGray8Fixture, samples_num, iterations)
{
    gil::detail::image_correlate(gil::const_view(this -> img_2048), this -> k_7x7, 
        gil::view(this -> img_out1_2048));
}

BENCHMARK_F(Correlate2D_2048_7x7, Opencv_version, RandomImageGray8Fixture, samples_num, iterations)
{
    opencv::filter2D(src_2048, dst, ddepth , kernel1_7x7, anchor, delta, BORDER_DEFAULT);
}
#endif

CELERO_MAIN
