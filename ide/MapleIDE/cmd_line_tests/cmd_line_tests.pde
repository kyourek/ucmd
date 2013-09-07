#include "cmd_line_tests.h"

cl_tests *tests_instance;

void tests_print(const char *str) {
  SerialUSB.print(str);
}

void setup() {
  tests_instance = cl_tests_get_instance();
  cl_tests_set_print(tests_instance, tests_print);  
}

void loop() {
  cl_tests_main(tests_instance);
  delay(1000);
}

