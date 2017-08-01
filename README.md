# portable-system-info-cpp
Get OS type, CPU, and amount of RAM on Windows, Mac, Linux, BSD

This was sort of thrown together quickly and there may be some bugs. It was *briefly* tested on target operating systems. Scripting can likely be improved upon / made more concise.

Hosting so others needing a solution to this problem won't need to write their own implementation like I did.

#### Example use:
```c++
#include <iostream>

#include "sysinfo.hpp"

int main() {
  sysinfo s_i = get_sysinfo();
  std::cout << s_i.os << std::endl;
  std::cout << s_i.cpu << std::endl;
  std::cout << s_i.ram << std::endl;
}

```
