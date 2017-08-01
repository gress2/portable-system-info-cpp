#include <iostream>
#include <cstdlib>
#include <cstdio>
#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
#include <windows.h>
#include <intrin.h>
#endif

struct sysinfo {
  std::string os;
  std::string cpu;
  std::string ram;
};

sysinfo get_sysinfo() {
  sysinfo sys;
  FILE* out;
  char buffer[1024];
  #if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
    sys.os = "Windows";
    int CPUInfo[4] = {-1};
    unsigned nExIds, i = 0;
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i=0x80000000; i<=nExIds; ++i) {
      __cpuid(CPUInfo, i);
      if (i == 0x80000002) {
        memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
      }
      else if  (i == 0x80000003) {
        memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
      }
      else if  (i == 0x80000004) {
        memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
      }
    }
    sys.cpu = CPUBrandString;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    sys.ram = std::to_string(statex.ullTotalPhys / (1024 * 1024 * 1024)) + " GB";
  #elif defined(__linux__)
    sys.os = "Linux";
    out = popen("cat /proc/cpuinfo | grep 'model name' | uniq | sed 's/model name[[:blank:]]*:[[:blank:]]*//' | awk 1 ORS=' '", "r");
    sys.cpu = fgets(buffer, sizeof(buffer), out);
    out = popen("cat /proc/meminfo | grep 'MemTotal' | sed 's/MemTotal:[[:blank:]]*//' | awk '{$1=$1/1024^2; print $1,\"GB\";}' | awk 1 ORS=' '", "r");
    sys.ram = fgets(buffer, sizeof(buffer), out);
  #elif defined(__APPLE__)
    sys.os = "Apple";
    out = popen("sysctl -n machdep.cpu.brand_string | awk 1 ORS=' '", "r");
    sys.cpu = fgets(buffer, sizeof(buffer), out);
    out = popen("system_profiler SPHardwareDataType | grep Memory | sed 's/[[:blank:]]*Memory: //' | awk 1 ORS=' '", "r");
    sys.ram = fgets(buffer, sizeof(buffer), out);
  #elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    sys.os = "BSD";
    out = popen("dmesg | grep 'CPU:' | sed 's/CPU: //' | awk 1 ORS=' '", "r");
    sys.cpu = fgets(buffer, sizeof(buffer), out);
    out = popen("dmesg | grep 'real memory' | sed 's/.*(\\([0-9]*\\).*)/\\1/' | awk '{$1=$1/1024; print $1,\"GB\";}' | awk 1 ORS=' '", "r");
    sys.ram = fgets(buffer, sizeof(buffer), out);
  #endif
    return sys;
}
