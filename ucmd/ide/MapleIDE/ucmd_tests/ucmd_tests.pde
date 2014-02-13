#include "ucmd_tests.h"

uc_tests *tests_instance;

void tests_print(const char *str) {
  SerialUSB.print(str);
}

void setup() {
  tests_instance = uc_tests_get_instance();
  uc_tests_set_print(tests_instance, tests_print);  
}

void loop() {
  uc_tests_main(tests_instance);
  delay(1000);
}
