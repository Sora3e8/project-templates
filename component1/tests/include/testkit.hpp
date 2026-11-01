#ifndef TESTKIT_HANDLERS
#define TESTKIT_HANDLERS

int help(int argc, char* argv[]);
int class1test1(int argc, char* argv[]);
int class1method1(int argc, char* argv[]);

struct Handler
{
    const char* arg;
    int (*func)(int argc, char* argv[]);
    const char* descr;
};

const Handler handlers[] = {
  { "--help", &help, "Prints this table." },
  { "--c01t01", &class1test1, "Class1 test1 constructor1" },
  { "--c01t02", &class1method1, "Class1 test2 method1 " },
};
int handler(int argc, char* argv[]);

#endif // !TESTKIT_HANDLERS
