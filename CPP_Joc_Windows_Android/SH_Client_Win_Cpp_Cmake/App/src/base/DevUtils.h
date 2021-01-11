#pragma once

// __FILE__ contains the path also and is not recommended to be used in release.

// __FILE_NAME__ is clang speciffic but visual studio doesn't see it so this "fake define" (will not actually be definned when compiling)) is required.
/// Tis define will not occur but visual studio editor sees it as active.
#ifndef __FILE_NAME__
#define __FILE_NAME__ nullptr
#endif // !__FILE_NAME__

#define LOC __FILE_NAME__,__LINE__
