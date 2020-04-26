# Requirements
C++20, fmtlib, spdlog, tensorflow, gpiod (with C++ bindings)

# Building

```
cmake -H. -Bbuilds/Release
cmake --build builds/Release
```

By default, this will produce a shared lib.
