#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

SOURCE_BRANCH="master"
TARGET_BRANCH="gh-pages"

if [ -z "$GH_TOKEN" ]; then
    echo "Error: GH_TOKEN is undefined"
    exit 1
fi

# Save some useful information
REPO=`git config remote.origin.url`
SHA=`git rev-parse --verify HEAD`

# doc happens to contain the "html" tree that we want to push
# into the gh-pages branch. So we step into that directory, create a new repo,
# set the remote appropriately, then commit and push.
cd doc
git init
git config user.name "GA"
git config user.email "ga"

# Make sure 'GH_TOKEN' is set (as 'secure' variable) in .travis.yml
git remote add upstream "https://$GH_TOKEN@github.com/boostorg/gil.git"
git fetch upstream
git reset upstream/gh-pages

# Prepare version.
if [ "$BRANCH_NAME" = develop -o "$BRANCH_NAME" = doc ]; then
    mkdir -p develop/doc/
    cp ../index.html develop/
    cp ../doc/index.html develop/doc/
    cp -a html develop/doc/
    git add -A develop
else
    git add html index.html
fi
# Commit the new version.
git commit -m "Deploy to GitHub Pages: ${SHA}"

# Now that we're all set up, we can push.
git push -q upstream HEAD:gh-pages