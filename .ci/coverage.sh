#! /bin/bash
set -e
echo "I was here in coverage.sh check error"
cd ../boost-root
pwd
ls

./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/core
./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/legacy
./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/dynamic_image
./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/numeric
./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/toolbox
./b2 -j 2 $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/io//simple

lcov --directory bin.v2 --capture --no-external --directory $(pwd) --output-file coverage.info > /dev/null 2>&1
echo "I reached even here"
lcov --extract coverage.info $(pwd)'/boost/gil/*' --output-file coverage.info > /dev/null
echo "I reached even even here"
lcov --list coverage.info
cd libs/gil
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"

#  echo "I was even here"
#         cd boost-root
#         echo "I was here"
#         export BOOST_ROOT=$(pwd)
#         ./bootstrap.sh
#         ls