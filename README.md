# fd_sdk_vendor
ROS 2 vendor package for Force Dimension SDK.

***Tested with a Humble ROS distribution only (Ubuntu 22.04 LTS)***

![Force Dimension SDK](https://img.shields.io/badge/SDK_version-3.16.1-blue)
[![CI (humble)](https://github.com/ICube-Robotics/fd_sdk_vendor/actions/workflows/ci.yml/badge.svg)](https://github.com/ICube-Robotics/fd_sdk_vendor/actions/workflows/ci.yml)
[![Build tests (iron)](../../actions/workflows/ci-iron.yaml/badge.svg?branch=main)](../../actions/workflows/ci-iron.yaml?query=branch:main)
[![Build tests (rolling)](../../actions/workflows/ci-rolling.yaml/badge.svg?branch=main)](../../actions/workflows/ci-rolling.yaml?query=branch:main)

## Usage example


> `package.xml` :
```xml
...
<depend>fd_sdk_vendor</depend>
...
```

> `CMakeLists.txt` :
```cmake
...
find_package(fd_sdk_vendor)
...
ament_target_dependencies(<the_target> <type> fd_sdk_vendor)
...
```
> `xxx.cpp` :
```cpp
#include <fd_sdk_vendor/dhd.hpp>

/*
-----------------------------
List of valid include paths:
-----------------------------
  * Haptic SDK
#include <fd_sdk_vendor/dhd.hpp>

  * Robotic SDK
#include <fd_sdk_vendor/drd.hpp>

  * Legacy includes (provided for compatibility with older code)
#include "dhdc.h"
#include "drdc.h"
*/
...

int major, minor, release, revision;
dhdGetSDKVersion (&major, &minor, &release, &revision);
printf("Force Dimension SDK, version %i.%i.%i", major, minor, release);

...
```
