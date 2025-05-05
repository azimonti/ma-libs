#ifndef __clang_analyzer__
/************************/
/*     main.cpp         */
/*    Version 1.0       */
/*     2022/07/07       */
/************************/

// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "doctest.h"
#include "global_static.h"
bool GLOBAL_VERBOSE_FLAG = false;

class dt_removed
{
    std::vector<const char*> vec;

  public:
    dt_removed(const char** argv_in)
    {
        for (; *argv_in; ++argv_in)
            if (strncmp(*argv_in, "--dt-", strlen("--dt-")) != 0) vec.push_back(*argv_in);
        vec.push_back(NULL);
    }

    int argc() { return static_cast<int>(vec.size()) - 1; }

    const char** argv() { return &vec[0]; } // Note: non-const char **:
};

int main(int argc, const char** argv)
{
    (void)argc;
    doctest::Context context;

    context.setOption("order-by", "file");
    context.setOption("no-intro", true);

    context.applyCommandLine(argc, argv);

    dt_removed args(argv);
    while (*++argv)
    {
        if ((strncmp(*argv, "-b", strlen("-b")) == 0) || (strncmp(*argv, "--verbose", strlen("--verbose")) == 0))
            GLOBAL_VERBOSE_FLAG = true;
    }
    int res = context.run(); // run

    return res;
}
#endif
