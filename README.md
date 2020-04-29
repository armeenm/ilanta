# Requirements
C++20, spdlog, libtorch, gpiod (with C++ bindings)

# Building

```
cmake -H. -Bbuilds/Release
cmake --build builds/Release
```

By default, this will produce a shared lib.
