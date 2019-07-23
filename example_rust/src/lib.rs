#[repr(C)]
struct CMmioPlugin {
    alloc: extern fn(*const libc::c_char) -> *mut libc::c_void,
    load: extern fn(*mut libc::c_void, u64, libc::size_t, *mut libc::c_uchar) -> bool,
    store: extern fn(*mut libc::c_void, u64, libc::size_t, *const libc::c_uchar) -> bool,
    dealloc: extern fn(*mut libc::c_void)
}

extern "C" {
    fn register_mmio_plugin(name_cstr: *const libc::c_char, mmio_plugin: *const CMmioPlugin);
}

extern "C" fn alloc(args: *const libc::c_char) -> *mut libc::c_void {
  unsafe {
      println!("ALLOC -- ARGS={}", std::ffi::CStr::from_ptr(args).to_string_lossy().into_owned());
  }

  return 0x123456789abcdef as *mut libc::c_void;
}

extern "C" fn load(c_self: *mut libc::c_void, addr: u64, len: libc::size_t, bytes: *mut libc::c_uchar) -> bool {
  println!("LOAD -- SELF={:p} ADDR={:#x} LEN={} BYTES={:p}", c_self, addr, len, bytes);
  return true
}

extern "C" fn store(c_self: *mut libc::c_void, addr: u64, len: libc::size_t, bytes: *const libc::c_uchar) -> bool {
  println!("STORE -- SELF={:p} ADDR={:#x} LEN={} BYTES={:p}", c_self, addr, len, bytes);
  return true
}

extern "C" fn dealloc(c_self: *mut libc::c_void) {
  println!("DEALLOC -- SELF={:p}", c_self);
}

#[ctor::ctor]
fn on_load() {
    let test_mmio_plugin = CMmioPlugin {
        alloc: alloc,
        load: load,
        store: store,
        dealloc: dealloc
    };

    unsafe {
        register_mmio_plugin(std::ffi::CString::new("test_mmio_plugin").unwrap().as_ptr(), &test_mmio_plugin);
    }
}
