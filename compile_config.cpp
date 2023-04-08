#include <iostream>

typedef uint8_t u8;

#include "Driver.h"

int main() {
  std::cout << uint8_t(sizeof(ttconfig_fw_ver)-2);
  for (int i = 0; i < sizeof(ttconfig_fw_ver)-2; ++i) std::cout << ttconfig_fw_ver[2+i];
  for (int i = 0; i < sizeof(cyttsp4_param_regs); ++i) std::cout << cyttsp4_param_regs[i];
}
