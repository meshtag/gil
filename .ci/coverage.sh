#! /bin/bash
set -e
echo "I was here in coverage.sh check error"
# cd ../boost-root

# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/core
# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/legacy
# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/extension/dynamic_image
# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/extension/numeric
# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/extension/toolbox
# ./b2 -j3 coverage=on toolset=gcc variant=debug cxxstd=11 libs/gil/test/extension/io//simple    

# echo "ls bin.v2 1"
# ls -a bin.v2
./b2 -j3 coverage=on libs/gil/test toolset=gcc cxxstd=11 variant=debug,release

echo "ls bin.v2"
ls -a bin.v2/libs/.
lcov --directory bin.v2 --capture --no-external --directory $(pwd) --output-file coverage.info > /dev/null 2>&1
echo "I reached even here"
lcov --extract coverage.info $(pwd)'/boost/gil/*' --output-file coverage.info > /dev/null
echo "I reached even even here"
lcov --list coverage.info
echo "I reached even even even here"
cd libs/gil
echo "I reached even even even even here"
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"
