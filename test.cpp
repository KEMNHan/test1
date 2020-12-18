#include "gtest/gtest.h"

#if GTEST_OS_WINDOWS
# include <windows.h>
# include <stdlib.h>
#endif

namespace {

// A test that's expected to fail.
TEST(Foo, Bar) {
  EXPECT_EQ(2, 3);
}

#if GTEST_HAS_SEH && !GTEST_OS_WINDOWS_MOBILE
// On Windows Mobile global exception handlers are not supported.
LONG WINAPI ExitWithExceptionCode(
    struct _EXCEPTION_POINTERS* exception_pointers) {
  exit(exception_pointers->ExceptionRecord->ExceptionCode);
}
#endif

}  // namespace

int main(int argc, char **argv) {
#if GTEST_OS_WINDOWS
  // Suppresses display of the Windows error dialog upon encountering
  // a general protection fault (segment violation).
  SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);

# if GTEST_HAS_SEH && !GTEST_OS_WINDOWS_MOBILE

  // The default unhandled exception filter does not always exit
  // with the exception code as exit code - for example it exits with
  // 0 for EXCEPTION_ACCESS_VIOLATION and 1 for EXCEPTION_BREAKPOINT
  // if the application is compiled in debug mode. Thus we use our own
  // filter which always exits with the exception code for unhandled
  // exceptions.
  SetUnhandledExceptionFilter(ExitWithExceptionCode);

# endif
#endif  // GTEST_OS_WINDOWS
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

