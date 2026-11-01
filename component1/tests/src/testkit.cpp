#include "testkit.hpp"
#include "MyClass1.hpp"
#include <cstddef>
#include <cstring>
#include <stdio.h>

int class1test1(int argc, char* argv[])
{
  MyClass1 test;
  return 0;
}

int class1method1(int argc, char* argv[])
{
  MyClass1 test_instance;
  test_instance.doSomething();

  return 0;
}

int help(int argc, char* argv[])
{
  printf("Usage: %s [OPTION] ...\n", EXECUTABLE_NAME);
  printf("%s created for [%s:%s]\n\nAvailable options:\n", EXECUTABLE_NAME, MULTILIB_NAME, COMPONENT_NAME);

  for (size_t i = 0; i < (sizeof(handlers) / sizeof(Handler)); i++)
  {
    printf("  %s\n\t%s\n", handlers[i].arg, handlers[i].descr);
  }

  return 0;
}

int handler(int argc, char* argv[])
{
  size_t size = sizeof(handlers) / sizeof(Handler);
  for (size_t i = 0; i < size; i++)
  {
    if (strcmp(argv[1], handlers[i].arg) == 0)
    {
      return handlers[i].func(argc, argv);
    }
  }

  printf("Unknown option: %s\n\n", argv[1]);
  help(argc, argv);

  return 1;
};

int main(int argc, char* argv[])
{
  if ((argc - 1) < 1)
  {
    printf("Invaid usage\n");
    help(argc, argv);
    return 1;
  }

  return handler(argc, argv);
}
