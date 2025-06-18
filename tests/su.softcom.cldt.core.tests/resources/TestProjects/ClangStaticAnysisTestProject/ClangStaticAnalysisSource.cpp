#include <iostream>
#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
#endif
#include <stdio.h>

void f(float array[3]) {
    size_t n = sizeof(array); // EXPECT: sizeof() is likely to be misused
    n = n / sizeof(array[0]);

    for (size_t i = 0; i != n; i++) { // EXPECT: Unused variable
        array[i] = 1.0f;
    }

    array = 0;
    array[0] = 5; // EXPECT: Access to null pointer is possible
}

int* f2() {
    int a = 10;

    return &a; // EXPECT: Variable is accessible outside its scope
}

#if defined(__linux__) || defined(__APPLE__)
void *testfn(void *p) // EXPECT: Unused variable
{
    int n;

    printf("test"); // EXPECT: Printing in thread
    scanf("%d", &n); // EXPECT: Scanning in thread
    return (void *)5;
}

pthread_mutex_t mutex;
int gvar = 0;

int f()
{
    pthread_mutex_lock(&mutex);
    gvar = 5; // EXPECT: Locking is inconsistent
    pthread_mutex_unlock(&mutex);
}

int g()
{
    gvar = 6;
}


#endif


int main(int argc, char** argv) // EXPECT: Unused variable
{
    // Roadmap #1
    //
    // Impracticable condition analysis testing
    int a = 0;
    if (a) /// EXPECT: Condition is always false
    {
        a = 5;
    }


    // Roadmap #2
    //
    // "sizeof" analysis testing
    int x = 5;
    int obj[2] = { 1, 2 };
    int* ptr = obj;
    int* obj2[2] = { 0, 0 };

    x = sizeof(x);

    x = sizeof(obj);

    x = sizeof(ptr); // EXPECT: sizeof() is likely to be misused | Unused assignment result

    x = sizeof(unsigned int*); // EXPECT: Unused assignment result | Unused assignment result

    x = sizeof(obj) / sizeof(*obj); // EXPECT: Unused assignment result

    x = sizeof(obj2) / sizeof(*obj2); // EXPECT: Unused assignment result

    float array[3] = { 0.1, 0.2, 0.3 };
    f(array);

    int y;
    y = sizeof(unsigned int) * 8 - 24;
    y = 5;  // EXPECT: Unused assignment result


    // Roadmap #3
    //
    // printf/scanf analysis testing
    printf("%d %x\n", 5.0f, (char*)"512"); // EXPECT: Argument type does not match the format string | Argument type does not match the format string

    printf("%d\n", 5.0f, (char*)"512"); // EXPECT: Argument type does not match the format string | Format argument is redundant

    printf("%f %s %d\n", 5.0f, (char*)"512"); // EXPECT: Format argument is not present

    printf("%.*d %llf\n", 1, 2, 5.0); // EXPECT: Format parameter is invalid | Format argument is redundant

    printf("%d %x\n", 5.0f, (char*)"512"); // EXPECT: Argument type does not match the format string | Argument type does not match the format string


    // Roadmap #4
    //
    // Error analysis testing "dereference of null pointers"
    int* b;
    int  b_good;
    if (argv[0])
    {
        b = 0;
    }
    else
    {
        b = 0;
    }
    *b = 5; // EXPECT: Access to null pointer is possible
    b = &b_good;
    *b = 5;
    b = NULL;
    *b = 5; // EXPECT: Access to null pointer is possible


    // Roadmap #5
    //
    // Error analysis testing "The absence of an incrementing variable in the loop"
    int i1, j1 = 1, size1 = 10;
    for (i1 = 0; i1 < size1; ++j1) { // EXPECT: Missing loop variable
        printf("val = %d", i1);
    }

    for (i1 = 0; i1 < size1; ++i1) {
        /* correct variable is used */
        printf("val = %d", i1);
    }


    // Roadmap #6
    //
    // Error analysis testing "Incorrect arithmetic operations"
    int a1 = 5;
    int b1;
    b1 = a1 << (sizeof(a1) * 8); // EXPECT: Incorrect integer shift | Possible integer overflow
    printf("%d", b1);

    int a2 = 5;
    int b2;
    b2 = a2 << -1; // EXPECT: Incorrect integer shift | Possible integer overflow
    printf("%d", b2);

    char a3 = 5; // EXPECT: Integer is implicitly truncated
    a3 = a3 & 0xFFFF; // EXPECT: Integer is implicitly truncated | Invalid bitmask

    int a4 = 500;
    char b4; // EXPECT: Unused variable
    b4 = a4; // EXPECT: Integer is implicitly truncated

    char a5 = 0xF0; // EXPECT: Integer is implicitly truncated
    a5 += 0xF0; // EXPECT: Integer is implicitly truncated


    // Roadmap #7
    //
    // Error analysis testing "Going outside the array"
    int a6[10];
    a6[10] = 100; // EXPECT: The index is out of range

    a6[5] = 6;


    // Roadmap #8
    //
    // Error analysis testing "Possible division by zero"
    int a7 = 0;
    int a8 = 10 / a7; // EXPECT: Possible division by zero
    a8 = 10 / (a7 + 1 * 5 + 2 - 7);


    // Roadmap #9
    //
    // Error analysis testing "Memory leak"
    int* a9 = (int*)malloc(1); // EXPECT: Memory leak is possible | Unused variable


    // Roadmap #10
    //
    // Error analysis testing "Returning a pointer to a local variable"
    f2();


    // Roadmap #11
    //
    // Error analysis testing "Initialization of fields in class constructors"
    class Test1 {
    public:
        int a;
        int b;
        int c;

        Test1() { // EXPECT: Uninitialized class member
            a = 5;
        }
    };

    class Test2 {
    public:
        int a;
        int b;
        int c;

        Test2() {
            a = 5;
            b = 5;
            c = 5;
        }
    };

    class Test3 {
    public:
        int a;
        int b;
        int *c;

        Test3() {
            a = 5;
            b = 5;
            c = nullptr;
        }
    };

    class Test4 {
    public:
        int a;
        int b;
        Test2 c;

        Test4() {
            /* c is initialized automatically */
            a = 5;
            b = 5;
        }
    };

    class Test5 {
    public:
        int a;
        int b;
        Test2 c;

        Test5() {
            a = 5;
            b = 5;
        }

        Test5(const Test4 &rhs) // EXPECT: Uninitialized class member
        {
            b = 5;
        }
    };

    FILE *f = fopen("/test", 0); // EXPECT: Null argument
    char buf[50];
    fread(buf, 0, 1, f); // EXPECT: Reading 0 symbols is pointless
    fwrite(buf, 0, 1, f); // EXPECT: Writing 0 symbols is pointless
    fclose(f);
    fclose(f); // EXPECT: File is closed twice
    fclose(f); // EXPECT: File is closed twice

#if defined(__linux__) || defined(__APPLE__)
    pthread_t handle;

    pthread_create(&handle,
                   NULL,
                   testfn,
                   NULL);
#endif
    return 0;
}
