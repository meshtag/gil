//
// Copyright 2019 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/gil/detail/math.hpp>
#include <boost/gil/image_processing/numeric.hpp>

#include <boost/core/lightweight_test.hpp>

#include <algorithm>

#include "test_utility_output_stream.hpp"

namespace gil = boost::gil;

std::vector<float> dx_sobel_6_15 {
    1.0000e+00,  2.0000e+00, -5.0000e+00, -1.2000e+01,  9.0000e+00,  3.0000e+01,
   -5.0000e+00, -4.0000e+01, -5.0000e+00,  3.0000e+01,  9.0000e+00, -1.2000e+01,
   -5.0000e+00,  2.0000e+00,  1.0000e+00,
    1.4000e+01,  2.8000e+01, -7.0000e+01, -1.6800e+02,  1.2600e+02,  4.2000e+02,
   -7.0000e+01, -5.6000e+02, -7.0000e+01,  4.2000e+02,  1.2600e+02, -1.6800e+02,
   -7.0000e+01,  2.8000e+01,  1.4000e+01,
    9.1000e+01,  1.8200e+02, -4.5500e+02, -1.0920e+03,  8.1900e+02,  2.7300e+03,
   -4.5500e+02, -3.6400e+03, -4.5500e+02,  2.7300e+03,  8.1900e+02, -1.0920e+03,
   -4.5500e+02,  1.8200e+02,  9.1000e+01,
    3.6400e+02,  7.2800e+02, -1.8200e+03, -4.3680e+03,  3.2760e+03,  1.0920e+04,
   -1.8200e+03, -1.4560e+04, -1.8200e+03,  1.0920e+04,  3.2760e+03, -4.3680e+03,
   -1.8200e+03,  7.2800e+02,  3.6400e+02,
    1.0010e+03,  2.0020e+03, -5.0050e+03, -1.2012e+04,  9.0090e+03,  3.0030e+04,
   -5.0050e+03, -4.0040e+04, -5.0050e+03,  3.0030e+04,  9.0090e+03, -1.2012e+04,
   -5.0050e+03,  2.0020e+03,  1.0010e+03,
    2.0020e+03,  4.0040e+03, -1.0010e+04, -2.4024e+04,  1.8018e+04,  6.0060e+04,
   -1.0010e+04, -8.0080e+04, -1.0010e+04,  6.0060e+04,  1.8018e+04, -2.4024e+04,
   -1.0010e+04,  4.0040e+03,  2.0020e+03,
    3.0030e+03,  6.0060e+03, -1.5015e+04, -3.6036e+04,  2.7027e+04,  9.0090e+04,
   -1.5015e+04, -1.2012e+05, -1.5015e+04,  9.0090e+04,  2.7027e+04, -3.6036e+04,
   -1.5015e+04,  6.0060e+03,  3.0030e+03,
    3.4320e+03,  6.8640e+03, -1.7160e+04, -4.1184e+04,  3.0888e+04,  1.0296e+05,
   -1.7160e+04, -1.3728e+05, -1.7160e+04,  1.0296e+05,  3.0888e+04, -4.1184e+04,
   -1.7160e+04,  6.8640e+03,  3.4320e+03,
    3.0030e+03,  6.0060e+03, -1.5015e+04, -3.6036e+04,  2.7027e+04,  9.0090e+04,
   -1.5015e+04, -1.2012e+05, -1.5015e+04,  9.0090e+04,  2.7027e+04, -3.6036e+04,
   -1.5015e+04,  6.0060e+03,  3.0030e+03,
    2.0020e+03,  4.0040e+03, -1.0010e+04, -2.4024e+04,  1.8018e+04,  6.0060e+04,
   -1.0010e+04, -8.0080e+04, -1.0010e+04,  6.0060e+04,  1.8018e+04, -2.4024e+04,
   -1.0010e+04,  4.0040e+03,  2.0020e+03,
    1.0010e+03,  2.0020e+03, -5.0050e+03, -1.2012e+04,  9.0090e+03,  3.0030e+04,
   -5.0050e+03, -4.0040e+04, -5.0050e+03,  3.0030e+04,  9.0090e+03, -1.2012e+04,
   -5.0050e+03,  2.0020e+03,  1.0010e+03,
    3.6400e+02,  7.2800e+02, -1.8200e+03, -4.3680e+03,  3.2760e+03,  1.0920e+04,
   -1.8200e+03, -1.4560e+04, -1.8200e+03,  1.0920e+04,  3.2760e+03, -4.3680e+03,
   -1.8200e+03,  7.2800e+02,  3.6400e+02,
    9.1000e+01,  1.8200e+02, -4.5500e+02, -1.0920e+03,  8.1900e+02,  2.7300e+03,
   -4.5500e+02, -3.6400e+03, -4.5500e+02,  2.7300e+03,  8.1900e+02, -1.0920e+03,
   -4.5500e+02,  1.8200e+02,  9.1000e+01,
    1.4000e+01,  2.8000e+01, -7.0000e+01, -1.6800e+02,  1.2600e+02,  4.2000e+02,
   -7.0000e+01, -5.6000e+02, -7.0000e+01,  4.2000e+02,  1.2600e+02, -1.6800e+02,
   -7.0000e+01,  2.8000e+01,  1.4000e+01,
    1.0000e+00,  2.0000e+00, -5.0000e+00, -1.2000e+01,  9.0000e+00,  3.0000e+01,
   -5.0000e+00, -4.0000e+01, -5.0000e+00,  3.0000e+01,  9.0000e+00, -1.2000e+01,
   -5.0000e+00,  2.0000e+00,  1.0000e+00
};

std::vector<float> dx_sobel_7_15 {
   -1.00000e+00,  0.00000e+00,  7.00000e+00,  0.00000e+00, -2.10000e+01,
    0.00000e+00,  3.50000e+01,  0.00000e+00, -3.50000e+01,  0.00000e+00,
    2.10000e+01,  0.00000e+00, -7.00000e+00,  0.00000e+00,  1.00000e+00,
   -1.40000e+01,  0.00000e+00,  9.80000e+01,  0.00000e+00, -2.94000e+02,
    0.00000e+00,  4.90000e+02,  0.00000e+00, -4.90000e+02,  0.00000e+00,
    2.94000e+02,  0.00000e+00, -9.80000e+01,  0.00000e+00,  1.40000e+01,
   -9.10000e+01,  0.00000e+00,  6.37000e+02,  0.00000e+00, -1.91100e+03,
    0.00000e+00,  3.18500e+03,  0.00000e+00, -3.18500e+03,  0.00000e+00,
    1.91100e+03,  0.00000e+00, -6.37000e+02,  0.00000e+00,  9.10000e+01,
   -3.64000e+02,  0.00000e+00,  2.54800e+03,  0.00000e+00, -7.64400e+03,
    0.00000e+00,  1.27400e+04,  0.00000e+00, -1.27400e+04,  0.00000e+00,
    7.64400e+03,  0.00000e+00, -2.54800e+03,  0.00000e+00,  3.64000e+02,
   -1.00100e+03,  0.00000e+00,  7.00700e+03,  0.00000e+00, -2.10210e+04,
    0.00000e+00,  3.50350e+04,  0.00000e+00, -3.50350e+04,  0.00000e+00,
    2.10210e+04,  0.00000e+00, -7.00700e+03,  0.00000e+00,  1.00100e+03,
   -2.00200e+03,  0.00000e+00,  1.40140e+04,  0.00000e+00, -4.20420e+04,
    0.00000e+00,  7.00700e+04,  0.00000e+00, -7.00700e+04,  0.00000e+00,
    4.20420e+04,  0.00000e+00, -1.40140e+04,  0.00000e+00,  2.00200e+03,
   -3.00300e+03,  0.00000e+00,  2.10210e+04,  0.00000e+00, -6.30630e+04,
    0.00000e+00,  1.05105e+05,  0.00000e+00, -1.05105e+05,  0.00000e+00,
    6.30630e+04,  0.00000e+00, -2.10210e+04,  0.00000e+00,  3.00300e+03,
   -3.43200e+03,  0.00000e+00,  2.40240e+04,  0.00000e+00, -7.20720e+04,
    0.00000e+00,  1.20120e+05,  0.00000e+00, -1.20120e+05,  0.00000e+00,
    7.20720e+04,  0.00000e+00, -2.40240e+04,  0.00000e+00,  3.43200e+03,
   -3.00300e+03,  0.00000e+00,  2.10210e+04,  0.00000e+00, -6.30630e+04,
    0.00000e+00,  1.05105e+05,  0.00000e+00, -1.05105e+05,  0.00000e+00,
    6.30630e+04,  0.00000e+00, -2.10210e+04,  0.00000e+00,  3.00300e+03,
   -2.00200e+03,  0.00000e+00,  1.40140e+04,  0.00000e+00, -4.20420e+04,
    0.00000e+00,  7.00700e+04,  0.00000e+00, -7.00700e+04,  0.00000e+00,
    4.20420e+04,  0.00000e+00, -1.40140e+04,  0.00000e+00,  2.00200e+03,
   -1.00100e+03,  0.00000e+00,  7.00700e+03,  0.00000e+00, -2.10210e+04,
    0.00000e+00,  3.50350e+04,  0.00000e+00, -3.50350e+04,  0.00000e+00,
    2.10210e+04,  0.00000e+00, -7.00700e+03,  0.00000e+00,  1.00100e+03,
   -3.64000e+02,  0.00000e+00,  2.54800e+03,  0.00000e+00, -7.64400e+03,
    0.00000e+00,  1.27400e+04,  0.00000e+00, -1.27400e+04,  0.00000e+00,
    7.64400e+03,  0.00000e+00, -2.54800e+03,  0.00000e+00,  3.64000e+02,
   -9.10000e+01,  0.00000e+00,  6.37000e+02,  0.00000e+00, -1.91100e+03,
    0.00000e+00,  3.18500e+03,  0.00000e+00, -3.18500e+03,  0.00000e+00,
    1.91100e+03,  0.00000e+00, -6.37000e+02,  0.00000e+00,  9.10000e+01,
   -1.40000e+01,  0.00000e+00,  9.80000e+01,  0.00000e+00, -2.94000e+02,
    0.00000e+00,  4.90000e+02,  0.00000e+00, -4.90000e+02,  0.00000e+00,
    2.94000e+02,  0.00000e+00, -9.80000e+01,  0.00000e+00,  1.40000e+01,
   -1.00000e+00,  0.00000e+00,  7.00000e+00,  0.00000e+00, -2.10000e+01,
    0.00000e+00,  3.50000e+01,  0.00000e+00, -3.50000e+01,  0.00000e+00,
    2.10000e+01,  0.00000e+00, -7.00000e+00,  0.00000e+00,  1.00000e+00
};

std::vector<float> dx_sobel_11_23 {
   -1.0000000e+00  0.0000000e+00  1.1000000e+01  0.0000000e+00
   -5.5000000e+01  0.0000000e+00  1.6500000e+02  0.0000000e+00
   -3.3000000e+02  0.0000000e+00  4.6200000e+02  0.0000000e+00
   -4.6200000e+02  0.0000000e+00  3.3000000e+02  0.0000000e+00
   -1.6500000e+02  0.0000000e+00  5.5000000e+01  0.0000000e+00
   -1.1000000e+01  0.0000000e+00  1.0000000e+00]
   -2.2000000e+01  0.0000000e+00  2.4200000e+02  0.0000000e+00
   -1.2100000e+03  0.0000000e+00  3.6300000e+03  0.0000000e+00
   -7.2600000e+03  0.0000000e+00  1.0164000e+04  0.0000000e+00
   -1.0164000e+04  0.0000000e+00  7.2600000e+03  0.0000000e+00
   -3.6300000e+03  0.0000000e+00  1.2100000e+03  0.0000000e+00
   -2.4200000e+02  0.0000000e+00  2.2000000e+01]
   -2.3100000e+02  0.0000000e+00  2.5410000e+03  0.0000000e+00
   -1.2705000e+04  0.0000000e+00  3.8115000e+04  0.0000000e+00
   -7.6230000e+04  0.0000000e+00  1.0672200e+05  0.0000000e+00
   -1.0672200e+05  0.0000000e+00  7.6230000e+04  0.0000000e+00
   -3.8115000e+04  0.0000000e+00  1.2705000e+04  0.0000000e+00
   -2.5410000e+03  0.0000000e+00  2.3100000e+02]
   -1.5400000e+03  0.0000000e+00  1.6940000e+04  0.0000000e+00
   -8.4700000e+04  0.0000000e+00  2.5410000e+05  0.0000000e+00
   -5.0820000e+05  0.0000000e+00  7.1148000e+05  0.0000000e+00
   -7.1148000e+05  0.0000000e+00  5.0820000e+05  0.0000000e+00
   -2.5410000e+05  0.0000000e+00  8.4700000e+04  0.0000000e+00
   -1.6940000e+04  0.0000000e+00  1.5400000e+03]
   -7.3150000e+03  0.0000000e+00  8.0465000e+04  0.0000000e+00
   -4.0232500e+05  0.0000000e+00  1.2069750e+06  0.0000000e+00
   -2.4139500e+06  0.0000000e+00  3.3795300e+06  0.0000000e+00
   -3.3795300e+06  0.0000000e+00  2.4139500e+06  0.0000000e+00
   -1.2069750e+06  0.0000000e+00  4.0232500e+05  0.0000000e+00
   -8.0465000e+04  0.0000000e+00  7.3150000e+03]
   -2.6334000e+04  0.0000000e+00  2.8967400e+05  0.0000000e+00
   -1.4483700e+06  0.0000000e+00  4.3451100e+06  0.0000000e+00
   -8.6902200e+06  0.0000000e+00  1.2166308e+07  0.0000000e+00
   -1.2166308e+07  0.0000000e+00  8.6902200e+06  0.0000000e+00
   -4.3451100e+06  0.0000000e+00  1.4483700e+06  0.0000000e+00
   -2.8967400e+05  0.0000000e+00  2.6334000e+04]
   -7.4613000e+04  0.0000000e+00  8.2074300e+05  0.0000000e+00
   -4.1037150e+06  0.0000000e+00  1.2311145e+07  0.0000000e+00
   -2.4622290e+07  0.0000000e+00  3.4471208e+07  0.0000000e+00
   -3.4471208e+07  0.0000000e+00  2.4622290e+07  0.0000000e+00
   -1.2311145e+07  0.0000000e+00  4.1037150e+06  0.0000000e+00
   -8.2074300e+05  0.0000000e+00  7.4613000e+04]
   -1.7054400e+05  0.0000000e+00  1.8759840e+06  0.0000000e+00
   -9.3799200e+06  0.0000000e+00  2.8139760e+07  0.0000000e+00
   -5.6279520e+07  0.0000000e+00  7.8791328e+07  0.0000000e+00
   -7.8791328e+07  0.0000000e+00  5.6279520e+07  0.0000000e+00
   -2.8139760e+07  0.0000000e+00  9.3799200e+06  0.0000000e+00
   -1.8759840e+06  0.0000000e+00  1.7054400e+05]
   -3.1977000e+05  0.0000000e+00  3.5174700e+06  0.0000000e+00
   -1.7587350e+07  0.0000000e+00  5.2762048e+07  0.0000000e+00
   -1.0552410e+08  0.0000000e+00  1.4773374e+08  0.0000000e+00
   -1.4773374e+08  0.0000000e+00  1.0552410e+08  0.0000000e+00
   -5.2762048e+07  0.0000000e+00  1.7587350e+07  0.0000000e+00
   -3.5174700e+06  0.0000000e+00  3.1977000e+05]
   -4.9742000e+05  0.0000000e+00  5.4716200e+06  0.0000000e+00
   -2.7358100e+07  0.0000000e+00  8.2074304e+07  0.0000000e+00
   -1.6414861e+08  0.0000000e+00  2.2980803e+08  0.0000000e+00
   -2.2980803e+08  0.0000000e+00  1.6414861e+08  0.0000000e+00
   -8.2074304e+07  0.0000000e+00  2.7358100e+07  0.0000000e+00
   -5.4716200e+06  0.0000000e+00  4.9742000e+05]
   -6.4664600e+05  0.0000000e+00  7.1131060e+06  0.0000000e+00
   -3.5565528e+07  0.0000000e+00  1.0669659e+08  0.0000000e+00
   -2.1339318e+08  0.0000000e+00  2.9875046e+08  0.0000000e+00
   -2.9875046e+08  0.0000000e+00  2.1339318e+08  0.0000000e+00
   -1.0669659e+08  0.0000000e+00  3.5565528e+07  0.0000000e+00
   -7.1131060e+06  0.0000000e+00  6.4664600e+05]
   -7.0543200e+05  0.0000000e+00  7.7597520e+06  0.0000000e+00
   -3.8798760e+07  0.0000000e+00  1.1639628e+08  0.0000000e+00
   -2.3279256e+08  0.0000000e+00  3.2590957e+08  0.0000000e+00
   -3.2590957e+08  0.0000000e+00  2.3279256e+08  0.0000000e+00
   -1.1639628e+08  0.0000000e+00  3.8798760e+07  0.0000000e+00
   -7.7597520e+06  0.0000000e+00  7.0543200e+05]
   -6.4664600e+05  0.0000000e+00  7.1131060e+06  0.0000000e+00
   -3.5565528e+07  0.0000000e+00  1.0669659e+08  0.0000000e+00
   -2.1339318e+08  0.0000000e+00  2.9875046e+08  0.0000000e+00
   -2.9875046e+08  0.0000000e+00  2.1339318e+08  0.0000000e+00
   -1.0669659e+08  0.0000000e+00  3.5565528e+07  0.0000000e+00
   -7.1131060e+06  0.0000000e+00  6.4664600e+05]
   -4.9742000e+05  0.0000000e+00  5.4716200e+06  0.0000000e+00
   -2.7358100e+07  0.0000000e+00  8.2074304e+07  0.0000000e+00
   -1.6414861e+08  0.0000000e+00  2.2980803e+08  0.0000000e+00
   -2.2980803e+08  0.0000000e+00  1.6414861e+08  0.0000000e+00
   -8.2074304e+07  0.0000000e+00  2.7358100e+07  0.0000000e+00
   -5.4716200e+06  0.0000000e+00  4.9742000e+05]
   -3.1977000e+05  0.0000000e+00  3.5174700e+06  0.0000000e+00
   -1.7587350e+07  0.0000000e+00  5.2762048e+07  0.0000000e+00
   -1.0552410e+08  0.0000000e+00  1.4773374e+08  0.0000000e+00
   -1.4773374e+08  0.0000000e+00  1.0552410e+08  0.0000000e+00
   -5.2762048e+07  0.0000000e+00  1.7587350e+07  0.0000000e+00
   -3.5174700e+06  0.0000000e+00  3.1977000e+05]
   -1.7054400e+05  0.0000000e+00  1.8759840e+06  0.0000000e+00
   -9.3799200e+06  0.0000000e+00  2.8139760e+07  0.0000000e+00
   -5.6279520e+07  0.0000000e+00  7.8791328e+07  0.0000000e+00
   -7.8791328e+07  0.0000000e+00  5.6279520e+07  0.0000000e+00
   -2.8139760e+07  0.0000000e+00  9.3799200e+06  0.0000000e+00
   -1.8759840e+06  0.0000000e+00  1.7054400e+05]
   -7.4613000e+04  0.0000000e+00  8.2074300e+05  0.0000000e+00
   -4.1037150e+06  0.0000000e+00  1.2311145e+07  0.0000000e+00
   -2.4622290e+07  0.0000000e+00  3.4471208e+07  0.0000000e+00
   -3.4471208e+07  0.0000000e+00  2.4622290e+07  0.0000000e+00
   -1.2311145e+07  0.0000000e+00  4.1037150e+06  0.0000000e+00
   -8.2074300e+05  0.0000000e+00  7.4613000e+04]
   -2.6334000e+04  0.0000000e+00  2.8967400e+05  0.0000000e+00
   -1.4483700e+06  0.0000000e+00  4.3451100e+06  0.0000000e+00
   -8.6902200e+06  0.0000000e+00  1.2166308e+07  0.0000000e+00
   -1.2166308e+07  0.0000000e+00  8.6902200e+06  0.0000000e+00
   -4.3451100e+06  0.0000000e+00  1.4483700e+06  0.0000000e+00
   -2.8967400e+05  0.0000000e+00  2.6334000e+04]
   -7.3150000e+03  0.0000000e+00  8.0465000e+04  0.0000000e+00
   -4.0232500e+05  0.0000000e+00  1.2069750e+06  0.0000000e+00
   -2.4139500e+06  0.0000000e+00  3.3795300e+06  0.0000000e+00
   -3.3795300e+06  0.0000000e+00  2.4139500e+06  0.0000000e+00
   -1.2069750e+06  0.0000000e+00  4.0232500e+05  0.0000000e+00
   -8.0465000e+04  0.0000000e+00  7.3150000e+03]
   -1.5400000e+03  0.0000000e+00  1.6940000e+04  0.0000000e+00
   -8.4700000e+04  0.0000000e+00  2.5410000e+05  0.0000000e+00
   -5.0820000e+05  0.0000000e+00  7.1148000e+05  0.0000000e+00
   -7.1148000e+05  0.0000000e+00  5.0820000e+05  0.0000000e+00
   -2.5410000e+05  0.0000000e+00  8.4700000e+04  0.0000000e+00
   -1.6940000e+04  0.0000000e+00  1.5400000e+03]
   -2.3100000e+02  0.0000000e+00  2.5410000e+03  0.0000000e+00
   -1.2705000e+04  0.0000000e+00  3.8115000e+04  0.0000000e+00
   -7.6230000e+04  0.0000000e+00  1.0672200e+05  0.0000000e+00
   -1.0672200e+05  0.0000000e+00  7.6230000e+04  0.0000000e+00
   -3.8115000e+04  0.0000000e+00  1.2705000e+04  0.0000000e+00
   -2.5410000e+03  0.0000000e+00  2.3100000e+02]
   -2.2000000e+01  0.0000000e+00  2.4200000e+02  0.0000000e+00
   -1.2100000e+03  0.0000000e+00  3.6300000e+03  0.0000000e+00
   -7.2600000e+03  0.0000000e+00  1.0164000e+04  0.0000000e+00
   -1.0164000e+04  0.0000000e+00  7.2600000e+03  0.0000000e+00
   -3.6300000e+03  0.0000000e+00  1.2100000e+03  0.0000000e+00
   -2.4200000e+02  0.0000000e+00  2.2000000e+01]
   -1.0000000e+00  0.0000000e+00  1.1000000e+01  0.0000000e+00
   -5.5000000e+01  0.0000000e+00  1.6500000e+02  0.0000000e+00
   -3.3000000e+02  0.0000000e+00  4.6200000e+02  0.0000000e+00
   -4.6200000e+02  0.0000000e+00  3.3000000e+02  0.0000000e+00
   -1.6500000e+02  0.0000000e+00  5.5000000e+01  0.0000000e+00
   -1.1000000e+01  0.0000000e+00  1.0000000e+00]]
};

std::vector<float> dy_sobel_2_15 {
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00,
    1.00000e+01,  1.40000e+02,  9.10000e+02,  3.64000e+03,  1.00100e+04,
    2.00200e+04,  3.00300e+04,  3.43200e+04,  3.00300e+04,  2.00200e+04,
    1.00100e+04,  3.64000e+03,  9.10000e+02,  1.40000e+02,  1.00000e+01,
    4.30000e+01,  6.02000e+02,  3.91300e+03,  1.56520e+04,  4.30430e+04,
    8.60860e+04,  1.29129e+05,  1.47576e+05,  1.29129e+05,  8.60860e+04,
    4.30430e+04,  1.56520e+04,  3.91300e+03,  6.02000e+02,  4.30000e+01,
    1.00000e+02,  1.40000e+03,  9.10000e+03,  3.64000e+04,  1.00100e+05,
    2.00200e+05,  3.00300e+05,  3.43200e+05,  3.00300e+05,  2.00200e+05,
    1.00100e+05,  3.64000e+04,  9.10000e+03,  1.40000e+03,  1.00000e+02,
    1.21000e+02,  1.69400e+03,  1.10110e+04,  4.40440e+04,  1.21121e+05,
    2.42242e+05,  3.63363e+05,  4.15272e+05,  3.63363e+05,  2.42242e+05,
    1.21121e+05,  4.40440e+04,  1.10110e+04,  1.69400e+03,  1.21000e+02,
    2.20000e+01,  3.08000e+02,  2.00200e+03,  8.00800e+03,  2.20220e+04,
    4.40440e+04,  6.60660e+04,  7.55040e+04,  6.60660e+04,  4.40440e+04,
    2.20220e+04,  8.00800e+03,  2.00200e+03,  3.08000e+02,  2.20000e+01,
   -1.65000e+02, -2.31000e+03, -1.50150e+04, -6.00600e+04, -1.65165e+05,
   -3.30330e+05, -4.95495e+05, -5.66280e+05, -4.95495e+05, -3.30330e+05,
   -1.65165e+05, -6.00600e+04, -1.50150e+04, -2.31000e+03, -1.65000e+02,
   -2.64000e+02, -3.69600e+03, -2.40240e+04, -9.60960e+04, -2.64264e+05,
   -5.28528e+05, -7.92792e+05, -9.06048e+05, -7.92792e+05, -5.28528e+05,
   -2.64264e+05, -9.60960e+04, -2.40240e+04, -3.69600e+03, -2.64000e+02,
   -1.65000e+02, -2.31000e+03, -1.50150e+04, -6.00600e+04, -1.65165e+05,
   -3.30330e+05, -4.95495e+05, -5.66280e+05, -4.95495e+05, -3.30330e+05,
   -1.65165e+05, -6.00600e+04, -1.50150e+04, -2.31000e+03, -1.65000e+02,
    2.20000e+01,  3.08000e+02,  2.00200e+03,  8.00800e+03,  2.20220e+04,
    4.40440e+04,  6.60660e+04,  7.55040e+04,  6.60660e+04,  4.40440e+04,
    2.20220e+04,  8.00800e+03,  2.00200e+03,  3.08000e+02,  2.20000e+01,
    1.21000e+02,  1.69400e+03,  1.10110e+04,  4.40440e+04,  1.21121e+05,
    2.42242e+05,  3.63363e+05,  4.15272e+05,  3.63363e+05,  2.42242e+05,
    1.21121e+05,  4.40440e+04,  1.10110e+04,  1.69400e+03,  1.21000e+02,
    1.00000e+02,  1.40000e+03,  9.10000e+03,  3.64000e+04,  1.00100e+05,
    2.00200e+05,  3.00300e+05,  3.43200e+05,  3.00300e+05,  2.00200e+05,
    1.00100e+05,  3.64000e+04,  9.10000e+03,  1.40000e+03,  1.00000e+02,
    4.30000e+01,  6.02000e+02,  3.91300e+03,  1.56520e+04,  4.30430e+04,
    8.60860e+04,  1.29129e+05,  1.47576e+05,  1.29129e+05,  8.60860e+04,
    4.30430e+04,  1.56520e+04,  3.91300e+03,  6.02000e+02,  4.30000e+01,
    1.00000e+01,  1.40000e+02,  9.10000e+02,  3.64000e+03,  1.00100e+04,
    2.00200e+04,  3.00300e+04,  3.43200e+04,  3.00300e+04,  2.00200e+04,
    1.00100e+04,  3.64000e+03,  9.10000e+02,  1.40000e+02,  1.00000e+01,
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00,
};

std::vector<float> dy_sobel_3_15 {
   -1.00000e+00, -1.40000e+01, -9.10000e+01, -3.64000e+02, -1.00100e+03,
   -2.00200e+03, -3.00300e+03, -3.43200e+03, -3.00300e+03, -2.00200e+03,
   -1.00100e+03, -3.64000e+02, -9.10000e+01, -1.40000e+01, -1.00000e+00,
   -8.00000e+00, -1.12000e+02, -7.28000e+02, -2.91200e+03, -8.00800e+03,
   -1.60160e+04, -2.40240e+04, -2.74560e+04, -2.40240e+04, -1.60160e+04,
   -8.00800e+03, -2.91200e+03, -7.28000e+02, -1.12000e+02, -8.00000e+00,
   -2.50000e+01, -3.50000e+02, -2.27500e+03, -9.10000e+03, -2.50250e+04,
   -5.00500e+04, -7.50750e+04, -8.58000e+04, -7.50750e+04, -5.00500e+04,
   -2.50250e+04, -9.10000e+03, -2.27500e+03, -3.50000e+02, -2.50000e+01,
   -3.20000e+01, -4.48000e+02, -2.91200e+03, -1.16480e+04, -3.20320e+04,
   -6.40640e+04, -9.60960e+04, -1.09824e+05, -9.60960e+04, -6.40640e+04,
   -3.20320e+04, -1.16480e+04, -2.91200e+03, -4.48000e+02, -3.20000e+01,
    1.10000e+01,  1.54000e+02,  1.00100e+03,  4.00400e+03,  1.10110e+04,
    2.20220e+04,  3.30330e+04,  3.77520e+04,  3.30330e+04,  2.20220e+04,
    1.10110e+04,  4.00400e+03,  1.00100e+03,  1.54000e+02,  1.10000e+01,
    8.80000e+01,  1.23200e+03,  8.00800e+03,  3.20320e+04,  8.80880e+04,
    1.76176e+05,  2.64264e+05,  3.02016e+05,  2.64264e+05,  1.76176e+05,
    8.80880e+04,  3.20320e+04,  8.00800e+03,  1.23200e+03,  8.80000e+01,
    9.90000e+01,  1.38600e+03,  9.00900e+03,  3.60360e+04,  9.90990e+04,
    1.98198e+05,  2.97297e+05,  3.39768e+05,  2.97297e+05,  1.98198e+05,
    9.90990e+04,  3.60360e+04,  9.00900e+03,  1.38600e+03,  9.90000e+01,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
   -9.90000e+01, -1.38600e+03, -9.00900e+03, -3.60360e+04, -9.90990e+04,
   -1.98198e+05, -2.97297e+05, -3.39768e+05, -2.97297e+05, -1.98198e+05,
   -9.90990e+04, -3.60360e+04, -9.00900e+03, -1.38600e+03, -9.90000e+01,
   -8.80000e+01, -1.23200e+03, -8.00800e+03, -3.20320e+04, -8.80880e+04,
   -1.76176e+05, -2.64264e+05, -3.02016e+05, -2.64264e+05, -1.76176e+05,
   -8.80880e+04, -3.20320e+04, -8.00800e+03, -1.23200e+03, -8.80000e+01,
   -1.10000e+01, -1.54000e+02, -1.00100e+03, -4.00400e+03, -1.10110e+04,
   -2.20220e+04, -3.30330e+04, -3.77520e+04, -3.30330e+04, -2.20220e+04,
   -1.10110e+04, -4.00400e+03, -1.00100e+03, -1.54000e+02, -1.10000e+01,
    3.20000e+01,  4.48000e+02,  2.91200e+03,  1.16480e+04,  3.20320e+04,
    6.40640e+04,  9.60960e+04,  1.09824e+05,  9.60960e+04,  6.40640e+04,
    3.20320e+04,  1.16480e+04,  2.91200e+03,  4.48000e+02,  3.20000e+01,
    2.50000e+01,  3.50000e+02,  2.27500e+03,  9.10000e+03,  2.50250e+04,
    5.00500e+04,  7.50750e+04,  8.58000e+04,  7.50750e+04,  5.00500e+04,
    2.50250e+04,  9.10000e+03,  2.27500e+03,  3.50000e+02,  2.50000e+01,
    8.00000e+00,  1.12000e+02,  7.28000e+02,  2.91200e+03,  8.00800e+03,
    1.60160e+04,  2.40240e+04,  2.74560e+04,  2.40240e+04,  1.60160e+04,
    8.00800e+03,  2.91200e+03,  7.28000e+02,  1.12000e+02,  8.00000e+00,
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00
};

std::vector<float> dy_sobel_4_15 {
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00,
    6.00000e+00,  8.40000e+01,  5.46000e+02,  2.18400e+03,  6.00600e+03,
    1.20120e+04,  1.80180e+04,  2.05920e+04,  1.80180e+04,  1.20120e+04,
    6.00600e+03,  2.18400e+03,  5.46000e+02,  8.40000e+01,  6.00000e+00,
    1.10000e+01,  1.54000e+02,  1.00100e+03,  4.00400e+03,  1.10110e+04,
    2.20220e+04,  3.30330e+04,  3.77520e+04,  3.30330e+04,  2.20220e+04,
    1.10110e+04,  4.00400e+03,  1.00100e+03,  1.54000e+02,  1.10000e+01,
   -4.00000e+00, -5.60000e+01, -3.64000e+02, -1.45600e+03, -4.00400e+03,
   -8.00800e+03, -1.20120e+04, -1.37280e+04, -1.20120e+04, -8.00800e+03,
   -4.00400e+03, -1.45600e+03, -3.64000e+02, -5.60000e+01, -4.00000e+00,
   -3.90000e+01, -5.46000e+02, -3.54900e+03, -1.41960e+04, -3.90390e+04,
   -7.80780e+04, -1.17117e+05, -1.33848e+05, -1.17117e+05, -7.80780e+04,
   -3.90390e+04, -1.41960e+04, -3.54900e+03, -5.46000e+02, -3.90000e+01,
   -3.80000e+01, -5.32000e+02, -3.45800e+03, -1.38320e+04, -3.80380e+04,
   -7.60760e+04, -1.14114e+05, -1.30416e+05, -1.14114e+05, -7.60760e+04,
   -3.80380e+04, -1.38320e+04, -3.45800e+03, -5.32000e+02, -3.80000e+01,
    2.70000e+01,  3.78000e+02,  2.45700e+03,  9.82800e+03,  2.70270e+04,
    5.40540e+04,  8.10810e+04,  9.26640e+04,  8.10810e+04,  5.40540e+04,
    2.70270e+04,  9.82800e+03,  2.45700e+03,  3.78000e+02,  2.70000e+01,
    7.20000e+01,  1.00800e+03,  6.55200e+03,  2.62080e+04,  7.20720e+04,
    1.44144e+05,  2.16216e+05,  2.47104e+05,  2.16216e+05,  1.44144e+05,
    7.20720e+04,  2.62080e+04,  6.55200e+03,  1.00800e+03,  7.20000e+01,
    2.70000e+01,  3.78000e+02,  2.45700e+03,  9.82800e+03,  2.70270e+04,
    5.40540e+04,  8.10810e+04,  9.26640e+04,  8.10810e+04,  5.40540e+04,
    2.70270e+04,  9.82800e+03,  2.45700e+03,  3.78000e+02,  2.70000e+01,
   -3.80000e+01, -5.32000e+02, -3.45800e+03, -1.38320e+04, -3.80380e+04,
   -7.60760e+04, -1.14114e+05, -1.30416e+05, -1.14114e+05, -7.60760e+04,
   -3.80380e+04, -1.38320e+04, -3.45800e+03, -5.32000e+02, -3.80000e+01,
   -3.90000e+01, -5.46000e+02, -3.54900e+03, -1.41960e+04, -3.90390e+04,
   -7.80780e+04, -1.17117e+05, -1.33848e+05, -1.17117e+05, -7.80780e+04,
   -3.90390e+04, -1.41960e+04, -3.54900e+03, -5.46000e+02, -3.90000e+01,
   -4.00000e+00, -5.60000e+01, -3.64000e+02, -1.45600e+03, -4.00400e+03,
   -8.00800e+03, -1.20120e+04, -1.37280e+04, -1.20120e+04, -8.00800e+03,
   -4.00400e+03, -1.45600e+03, -3.64000e+02, -5.60000e+01, -4.00000e+00,
    1.10000e+01,  1.54000e+02,  1.00100e+03,  4.00400e+03,  1.10110e+04,
    2.20220e+04,  3.30330e+04,  3.77520e+04,  3.30330e+04,  2.20220e+04,
    1.10110e+04,  4.00400e+03,  1.00100e+03,  1.54000e+02,  1.10000e+01,
    6.00000e+00,  8.40000e+01,  5.46000e+02,  2.18400e+03,  6.00600e+03,
    1.20120e+04,  1.80180e+04,  2.05920e+04,  1.80180e+04,  1.20120e+04,
    6.00600e+03,  2.18400e+03,  5.46000e+02,  8.40000e+01,  6.00000e+00,
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00
};

std::vector<float> dy_sobel_5_15 {
   -1.00000e+00, -1.40000e+01, -9.10000e+01, -3.64000e+02, -1.00100e+03,
   -2.00200e+03, -3.00300e+03, -3.43200e+03, -3.00300e+03, -2.00200e+03,
   -1.00100e+03, -3.64000e+02, -9.10000e+01, -1.40000e+01, -1.00000e+00,
   -4.00000e+00, -5.60000e+01, -3.64000e+02, -1.45600e+03, -4.00400e+03,
   -8.00800e+03, -1.20120e+04, -1.37280e+04, -1.20120e+04, -8.00800e+03,
   -4.00400e+03, -1.45600e+03, -3.64000e+02, -5.60000e+01, -4.00000e+00,
   -1.00000e+00, -1.40000e+01, -9.10000e+01, -3.64000e+02, -1.00100e+03,
   -2.00200e+03, -3.00300e+03, -3.43200e+03, -3.00300e+03, -2.00200e+03,
   -1.00100e+03, -3.64000e+02, -9.10000e+01, -1.40000e+01, -1.00000e+00,
    1.60000e+01,  2.24000e+02,  1.45600e+03,  5.82400e+03,  1.60160e+04,
    3.20320e+04,  4.80480e+04,  5.49120e+04,  4.80480e+04,  3.20320e+04,
    1.60160e+04,  5.82400e+03,  1.45600e+03,  2.24000e+02,  1.60000e+01,
    1.90000e+01,  2.66000e+02,  1.72900e+03,  6.91600e+03,  1.90190e+04,
    3.80380e+04,  5.70570e+04,  6.52080e+04,  5.70570e+04,  3.80380e+04,
    1.90190e+04,  6.91600e+03,  1.72900e+03,  2.66000e+02,  1.90000e+01,
   -2.00000e+01, -2.80000e+02, -1.82000e+03, -7.28000e+03, -2.00200e+04,
   -4.00400e+04, -6.00600e+04, -6.86400e+04, -6.00600e+04, -4.00400e+04,
   -2.00200e+04, -7.28000e+03, -1.82000e+03, -2.80000e+02, -2.00000e+01,
   -4.50000e+01, -6.30000e+02, -4.09500e+03, -1.63800e+04, -4.50450e+04,
   -9.00900e+04, -1.35135e+05, -1.54440e+05, -1.35135e+05, -9.00900e+04,
   -4.50450e+04, -1.63800e+04, -4.09500e+03, -6.30000e+02, -4.50000e+01,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
    0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,  0.00000e+00,
    4.50000e+01,  6.30000e+02,  4.09500e+03,  1.63800e+04,  4.50450e+04,
    9.00900e+04,  1.35135e+05,  1.54440e+05,  1.35135e+05,  9.00900e+04,
    4.50450e+04,  1.63800e+04,  4.09500e+03,  6.30000e+02,  4.50000e+01,
    2.00000e+01,  2.80000e+02,  1.82000e+03,  7.28000e+03,  2.00200e+04,
    4.00400e+04,  6.00600e+04,  6.86400e+04,  6.00600e+04,  4.00400e+04,
    2.00200e+04,  7.28000e+03,  1.82000e+03,  2.80000e+02,  2.00000e+01,
   -1.90000e+01, -2.66000e+02, -1.72900e+03, -6.91600e+03, -1.90190e+04,
   -3.80380e+04, -5.70570e+04, -6.52080e+04, -5.70570e+04, -3.80380e+04,
   -1.90190e+04, -6.91600e+03, -1.72900e+03, -2.66000e+02, -1.90000e+01,
   -1.60000e+01, -2.24000e+02, -1.45600e+03, -5.82400e+03, -1.60160e+04,
   -3.20320e+04, -4.80480e+04, -5.49120e+04, -4.80480e+04, -3.20320e+04,
   -1.60160e+04, -5.82400e+03, -1.45600e+03, -2.24000e+02, -1.60000e+01,
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00,
    4.00000e+00,  5.60000e+01,  3.64000e+02,  1.45600e+03,  4.00400e+03,
    8.00800e+03,  1.20120e+04,  1.37280e+04,  1.20120e+04,  8.00800e+03,
    4.00400e+03,  1.45600e+03,  3.64000e+02,  5.60000e+01,  4.00000e+00,
    1.00000e+00,  1.40000e+01,  9.10000e+01,  3.64000e+02,  1.00100e+03,
    2.00200e+03,  3.00300e+03,  3.43200e+03,  3.00300e+03,  2.00200e+03,
    1.00100e+03,  3.64000e+02,  9.10000e+01,  1.40000e+01,  1.00000e+00
};

void test_dx_sobel_kernel()
{
    auto const kernel = gil::generate_sobel_kernel({1, 0});
    BOOST_TEST_ALL_EQ(kernel.begin(), kernel.end(), gil::detail::dx_sobel.begin(),
        gil::detail::dx_sobel.end());

    auto const kernel_x_6_15 = gil::generate_sobel_kernel({6, 0}, 15);
    BOOST_TEST_ALL_EQ(kernel_x_6_15.begin(), kernel_x_6_15.end(),
        dx_sobel_6_15.begin(), dx_sobel_6_15.end());

    auto const kernel_x_7_15 = gil::generate_sobel_kernel({7, 0});
    BOOST_TEST_ALL_EQ(kernel_x_7_15.begin(), kernel_x_7_15.end(),
        dx_sobel_7_15.begin(), dx_sobel_7_15.end());
}

void test_dx_scharr_kernel()
{
    auto const kernel = gil::generate_dx_scharr(1);
    BOOST_TEST_ALL_EQ(kernel.begin(), kernel.end(), gil::detail::dx_scharr.begin(),
        gil::detail::dx_scharr.end());
}

void test_dy_sobel_kernel()
{
    auto const kernel = gil::generate_sobel_kernel({0, 1});
    BOOST_TEST_ALL_EQ(kernel.begin(), kernel.end(), gil::detail::dy_sobel.begin(),
        gil::detail::dy_sobel.end());

    auto const kernel_y_2_5 = gil::generate_sobel_kernel({0, 2});
    BOOST_TEST_ALL_EQ(kernel_y_2_5.begin(), kernel_y_2_5.end(),
        gil::detail::dy_sobel_2_5.begin(), gil::detail::dy_sobel_2_5.end());

    auto const kernel_y_2_15 = gil::generate_sobel_kernel({0, 2}, 15);
    BOOST_TEST_ALL_EQ(kernel_y_2_15.begin(), kernel_y_2_15.end(),
        dy_sobel_2_15.begin(), dy_sobel_2_15.end());

    auto const kernel_y_3_15 = gil::generate_sobel_kernel({0, 3}, 15);
    BOOST_TEST_ALL_EQ(kernel_y_3_15.begin(), kernel_y_3_15.end(),
        dy_sobel_3_15.begin(), dy_sobel_3_15.end());

    auto const kernel_y_4_15 = gil::generate_sobel_kernel({0, 4}, 15);
    BOOST_TEST_ALL_EQ(kernel_y_4_15.begin(), kernel_y_4_15.end(),
        dy_sobel_4_15.begin(), dy_sobel_4_15.end());

    auto const kernel_y_5_15 = gil::generate_sobel_kernel({0, 5}, 15);
    BOOST_TEST_ALL_EQ(kernel_y_5_15.begin(), kernel_y_5_15.end(),
        dy_sobel_5_15.begin(), dy_sobel_5_15.end());
}

void test_dy_scharr_kernel()
{
    auto const kernel = gil::generate_dy_scharr(1);
    BOOST_TEST_ALL_EQ(kernel.begin(), kernel.end(), gil::detail::dy_scharr.begin(),
        gil::detail::dy_scharr.end());
}

int main()
{
    test_dx_sobel_kernel();
    test_dx_scharr_kernel();
    test_dy_sobel_kernel();
    test_dy_scharr_kernel();

    // auto ans = gil::generate_sobel_kernel({2, 6}, 17);
    
    // for (int i = 0; i < 17; ++i)
    // {
    //     for (int j = 0; j < 17; ++j)
    //     {
    //         std::cout << ans[17 * i + j] << " ";
    //     }
    //     std::cout << "\n";
    // }
    
    return boost::report_errors();
}
