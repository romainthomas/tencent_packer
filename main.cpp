#include <iostream>
#include <dlfcn.h>
#include <link.h>
#include <LIEF/ELF.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h" //support for stdout logging
#include <zlib.h>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace LIEF::ELF;

struct tencent_header_t {
  uint32_t imagebase;
  uint32_t size;
  uint16_t segments_offset;
  uint16_t nb_segments;
  uint32_t unk1;
  uint32_t dynstr_offset;
  uint32_t dynsym_offset;
  uint32_t dt_init;
  uint32_t dt_init_array;
  uint32_t unk6;
  uint32_t unk7;
  uint16_t unk8;
  uint16_t nb_init_array;
  uint16_t nb_dt_needed;
  uint16_t unk10;
  uint32_t dt_needed_offset;
  uint32_t nb_symbols;
};

struct packed_segment_t {
  uint32_t virtual_address;
  uint32_t virtual_size;
  uint32_t file_offset;
  uint32_t file_size;
  uint32_t flags;
  uint32_t encrypted_size;
};


std::string buffer_string(const std::vector<uint8_t>& buff, size_t max) {
  const size_t max_size = std::min<size_t>(buff.size(), max);
  std::ostringstream oss;
  for (size_t i = 0; i < max_size; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(buff[i]) << " ";
  }
  return oss.str();
}


int main (int argc, char** argv) {
  auto console = spdlog::stdout_color_mt("console");

  if (argc != 2) {
    console->error("Usage: {} <libshell>", argv[0]);
    return EXIT_FAILURE;
  }

  std::unique_ptr<Binary> libshell = Parser::parse(argv[1]);
  if (not libshell) {
    console->error("Unable to parse: '{}'", argv[1]);
    return EXIT_FAILURE;
  }

  Binary::overlay_t overlay = libshell->overlay();
  auto theader = reinterpret_cast<const tencent_header_t*>(overlay.data());

  console->info("Overlay size: 0x{:06x}", overlay.size());
  console->info("== Tencent Header ==");

  console->info("{:<20s}: 0x{:06x}", "Image Base",         theader->imagebase);
  console->info("{:<20s}: 0x{:06x}", "Size",               theader->size);
  console->info("{:<20s}: 0x{:06x}", "Segments Offset",    theader->segments_offset);
  console->info("{:<20s}: {:d}",     "Number of Segments", theader->nb_segments);

  console->info("{:<30s}: 0x{:06x}",     "UNKNOWN[1]", theader->unk1);
  console->info("{:<30s}: 0x{:06x}",     ".dynstr offset", theader->dynstr_offset);
  console->info("{:<30s}: 0x{:06x}",     ".dynsym offset", theader->dynsym_offset);
  console->info("{:<30s}: 0x{:06x}",     "DT_INIT",       theader->dt_init);
  console->info("{:<30s}: 0x{:06x}",     "DT_INIT_ARRAY", theader->dt_init_array);
  console->info("{:<30s}: 0x{:06x}",     "UNKNOWN[6]", theader->unk6);
  console->info("{:<30s}: 0x{:06x}",     "UNKNOWN[7]", theader->unk7);
  console->info("{:<30s}: 0x{:06x}",     "UNKNOWN[8]", theader->unk8);
  console->info("{:<30s}: {:d}",         "Number of ELF ctor", theader->nb_init_array);
  console->info("{:<30s}: {:d}",         "Number of Dependencies", theader->nb_dt_needed);
  console->info("{:<30s}: 0x{:06x}",     "UNKNOWN[10]", theader->unk10);
  console->info("{:<30s}: 0x{:06x}",     "DT_NEEDED", theader->dt_needed_offset);
  console->info("{:<30s}: {:d}",         "Number of Symbols", theader->nb_symbols);


  console->info("== Packed Segments ({}) ==", theader->nb_segments);
  for (size_t i = 0; i < theader->nb_segments; ++i) {
    auto sheader = reinterpret_cast<const packed_segment_t*>(overlay.data() + theader->segments_offset + i * sizeof(packed_segment_t));
    console->info("Segment #{}:", i);
    console->info("    {:<20s}: 0x{:06x}", "Virtual Address", sheader->virtual_address);
    console->info("    {:<20s}: 0x{:06x}", "Virtual Size",    sheader->virtual_size);
    console->info("    {:<20s}: 0x{:06x}", "File Offset",     sheader->file_offset);
    console->info("    {:<20s}: 0x{:06x}", "File Size",       sheader->file_size);
    console->info("    {:<20s}: 0x{:06x}", "Flags",           sheader->flags);
    console->info("    {:<20s}: 0x{:06x}", "Encrypted Size",  sheader->encrypted_size);
  }

  return EXIT_SUCCESS;
}
