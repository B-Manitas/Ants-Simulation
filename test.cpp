#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int main(int argc, const char **argv)
{
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  return 0;
}
