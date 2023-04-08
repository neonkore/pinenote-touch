#include <boost/crc.hpp>
#include <fstream>
#include <iostream>

typedef uint8_t u8;

#include "Driver.h"

int main(int argc, char** argv) {
  if (argc != 2) {
       std::cerr << "Usage: compile_config <out_file>\n";
       return 1;
  }
  std::ofstream fp;
  fp.open(argv[1], std::ios::out | std::ios::binary);
  fp.put(sizeof(ttconfig_fw_ver) - 2);
  fp.write(reinterpret_cast<char*>(ttconfig_fw_ver + 2), sizeof(ttconfig_fw_ver) - 2);
  std::size_t const cyttsp4_param_regs_length = sizeof(cyttsp4_param_regs) - 2;
  fp.write(reinterpret_cast<const char*>(cyttsp4_param_regs), cyttsp4_param_regs_length);
  boost::crc_ccitt_false_t crc;
  crc = std::for_each(cyttsp4_param_regs, cyttsp4_param_regs + cyttsp4_param_regs_length, crc);
  uint16_t crcval = crc();
  fp.write(reinterpret_cast<char*>(&crcval), 2);
}
