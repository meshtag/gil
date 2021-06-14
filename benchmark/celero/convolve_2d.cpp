#include <celero/Celero.h>
#include <vector>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>
#include <boost/gil/extension/io/png.hpp>

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
        gil::read_image("resize_512_gray.png", img, gil::png_tag{});
        this -> img_out.recreate((this -> img).dimensions());
    }
    gil::gray8_image_t img;
    gil::gray8_image_t img_out;
    std::vector<float> v{std::vector<float>(9, 1.0f / 9.0f)};
    gil::detail::kernel_2d<float> kernel{gil::detail::kernel_2d<float>(v.begin(), v.size(), 1, 1)};
};

#ifdef NDEBUG
constexpr int samples = 30; // Celero default minimum
constexpr int iterations = 100;
#else
constexpr int samples = 1;
constexpr int iterations = 1;
#endif

BASELINE_F(Convolve2D, Original_version, RandomImageGray8Fixture, samples, iterations)
{
    detail::convolve_2d(view(this -> img), this -> kernel, view(this -> img_out));
}

CELERO_MAIN
