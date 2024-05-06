# beamlib

A barely working graphics library built for an introdoctory computer graphics course.

### Quick start

Project 1 is located in `examples/gumdam/`, and can be built with the following commands:

```bash
git clone --recursive https://github.com/BWsix/beamlib
cd beamlib
cmake -DASSIMP_WARNINGS_AS_ERRORS=OFF -S . -B build
cmake --build build -j

# for project 1, execute `build/examples/gumdam/gumdam` from the root directory
```

