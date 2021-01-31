#! /bin/bash
set -e
lcov --directory bin.v2 --capture --no-external --directory $(pwd)  --output-file coverage.info > /dev/null 2>&1
echo $(pwd)'/gil/*'
lcov --extract coverage.info $(pwd)'/gil/*' --output-file coverage.info > /dev/null
lcov --list coverage.info
cd /home/runner/work/gil/gil
bash <(curl -s https://codecov.io/bash) -f ../../coverage.info || echo "Codecov did not collect coverage reports"