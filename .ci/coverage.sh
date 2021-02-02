#! /bin/bash
#set -e
cd boost-root
ls
pwd
cd ..
ls
pwd
cd boost-root
cd ..
ls
pwd
cd boost-root
cd ..
ls
pwd
cd boost-root
lcov --directory bin.v2 --capture --no-external --directory $(pwd) --output-file coverage.info > /dev/null 2>&1
lcov --extract coverage.info $(pwd)'/boost/gil/*' --output-file coverage.info > /dev/null
lcov --list coverage.info
cd libs/gil
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"
