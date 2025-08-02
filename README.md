# exa-cpp

low-latency c++20 client for exa's search api; http/1.1 transport via boost.beast.

## features

- modern c++20 implementation
- http/1.1 transport via boost.beast
- asynchronous i/o with boost.asio
- low-latency design for high-performance applications

## build requirements

- c++20 compatible compiler
- cmake 3.27+
- conan 2.x package manager

## building

```bash
conan install . --build=missing --output-folder=build
cmake --preset conan-release
cmake --build --preset conan-release
```

## License

MIT License - see LICENSE file for details.
