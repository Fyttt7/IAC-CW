// #include <stdio.h>

// int f(void);

// void test(double a)
// {
//     printf("Test function produced value: %f\n", (float)a + 3.2f);
// }

// int main()
// {
//     printf("Hello from RISC-V\n");

//     test(5.5);
//     printf("Example function returned: %d\n", f());

//     return !(f() == 5);
// }
#include <stdio.h>

int f(void);

int main() {
  int result = f();
  printf("Example function returned: %d\n", result);

  if (result == 0) {
    printf("Test Passed!\n");
    return 0;
  } else {
    printf("Test Failed! Expected 0, got %d\n", result);
    return 1;
  }
}
