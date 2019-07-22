#include <riscv/mmio_plugin.h>
#include <cstdio>

struct test_mmio_plugin
{
  test_mmio_plugin(const std::string& args)
  {
    printf("ALLOC -- ARGS=%s\n", args.c_str());
  }

  ~test_mmio_plugin()
  {
    printf("DEALLOC -- SELF=%p\n", this);
  }

  bool load(reg_t addr, size_t len, uint8_t* bytes)
  {
    printf("LOAD -- SELF=%p ADDR=0x%lx LEN=%lu BYTES=%p\n", this, addr, len, (void*)bytes);
    return true;
  }

  bool store(reg_t addr, size_t len, const uint8_t* bytes)
  {
    printf("STORE -- SELF=%p ADDR=0x%lx LEN=%lu BYTES=%p\n", this, addr, len, (const void*)bytes);
    return true;
  }
};

static mmio_plugin_registration_t<test_mmio_plugin> test_mmio_plugin_registration("test_mmio_plugin");
