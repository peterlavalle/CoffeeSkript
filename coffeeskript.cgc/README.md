
# CoffeeSkript

This is a (non-working) attempt to embed CoffeeScript (via DukTape and TinFlue) for behaviour scripting.

- `duktape/`
  - a/my configuration of duktape plus a C++ "pimp-like" object
- `miniz/`
  - the last version of `tinfl.c` which I could find with a public-domain header
- `palbase/`
  - some macros to make things like `TODO()` simpler
- `small-tin-check/`
  - a small test of tinflue
- `coffeescript/`
  - a workspace to generrate a compressed copy of the coffeescript compiler and some C/++ code to interact with coffee-scripts
- `coffeescript-test-shared/`
  - implmentation of catch2.cpp tin.c and the base `script.coffee`
- `coffeescript-test-regress/`
  - old (unused) test to determine which version(s) of coffeescript work with DukTape
- `coffeescript-test-1/`
  - the primary test/s of coffeescript
- `coffeescript-test-2/`
  - replaces `script.coffee` and tries to "re-tin" the data


