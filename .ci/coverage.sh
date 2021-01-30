#! /bin/bash
set -e
echo I was here 1
lcov --directory bin.v2 --capture --no-external --directory $(pwd) --output-file coverage.info > /dev/null 2>&1
echo 2
lcov --extract coverage.info $(pwd)'/boost/gil/*' --output-file coverage.info > /dev/null
echo 3
lcov --list coverage.info
echo 4
cd libs/gil
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"