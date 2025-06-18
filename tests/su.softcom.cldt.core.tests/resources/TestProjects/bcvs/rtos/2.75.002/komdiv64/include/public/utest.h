#ifndef _UTEST_H_
#define _UTEST_H_

#include <stdio.h>


#define UT_ASSERT_EX(expr,fmt,...) \
    do { \
    if (!(expr)) { printf("ASSERT: file \"%s\", line %d, function \"%s\": ",\
            __FILE__, __LINE__, __FUNCTION__); \
            printf(fmt, __VA_ARGS__); \
            printf("\n"); \
            return 1; } \
    } while(0)

#define UT_ASSERT(expr,msg) UT_ASSERT_EX(expr, "%s", msg)

#define UT_TEST(test) \
    do { \
        printf("TEST: \"%s\" %s\n", #test, (_t_ ## test() ? "failed" : "OK")); \
    } while(0)

#define UT_TESTDEF(name) \
    static int _t_ ## name ()


#endif /*_UTEST_H_*/
