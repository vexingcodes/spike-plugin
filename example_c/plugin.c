#include <riscv/mmio_plugin.h>
#include <stdio.h>

void* test_mmio_plugin_alloc(const char* args)
{
    printf("ALLOC -- ARGS=%s\n", args);
    return (void*)0x0123456789abcdef;
}

bool test_mmio_plugin_load(void* self, reg_t addr, size_t len, uint8_t* bytes)
{
    printf("LOAD -- SELF=%p ADDR=0x%lx LEN=%lu BYTES=%p\n", self, addr, len, (void*)bytes);
    return true;
}

bool test_mmio_plugin_store(void* self, reg_t addr, size_t len, const uint8_t* bytes)
{
    printf("STORE -- SELF=%p ADDR=0x%lx LEN=%lu BYTES=%p\n", self, addr, len, (const void*)bytes);
    return true;
}

void test_mmio_plugin_dealloc(void* self)
{
    printf("DEALLOC -- SELF=%p\n", self);
}

__attribute__((constructor)) static void on_load()
{
  static mmio_plugin_t test_mmio_plugin = {
      test_mmio_plugin_alloc,
      test_mmio_plugin_load,
      test_mmio_plugin_store,
      test_mmio_plugin_dealloc
  };

  register_mmio_plugin("test_mmio_plugin", &test_mmio_plugin);
}
