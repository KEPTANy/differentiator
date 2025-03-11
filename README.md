# Differentiator

Differentiate and evaluate expressions.

# How to build/test/delete this library

```bash
# Build static library and executable. # Creates differentiator.a
# and ./differentiator in the same directory as this file.
make

# Build project and run test suite. Creates test/test_runner binary.
make test

# Rebuild project. Same as make fclean && make.
make re

# Delete temporary files (object files and other junk).
make clean

# Deletes build files, differentiator.a, ./differentiator
# and ./test/test_runner.
make fclean
```

