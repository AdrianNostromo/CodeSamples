#include "IFileHandlerAsync.h"
#include <base/MM.h>

using namespace base;

base::AsyncFHEvent* IFileHandlerAsync::EVENT_IO_failed = new base::AsyncFHEvent("EVENT_read_failed");

base::AsyncFHEvent* IFileHandlerAsync::EVENT_read_string_success = new base::AsyncFHEvent("EVENT_read_string_success");
base::AsyncFHEvent* IFileHandlerAsync::EVENT_read_binary_success = new base::AsyncFHEvent("EVENT_read_binary_success");
