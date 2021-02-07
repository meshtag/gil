#! /bin/bash
echo "I was here in coverage.sh"
set -e
lcov --directory bin.v2 --capture --no-external --directory $(pwd) --output-file coverage.info > /dev/null 2>&1
lcov --extract coverage.info $(pwd)'/boost/gil/*' --output-file coverage.info > /dev/null
lcov --list coverage.info
cd libs/gil
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"

#  echo "I was even here"
#         cd boost-root
#         echo "I was here"
#         export BOOST_ROOT=$(pwd)
#         ./bootstrap.sh
#         ls