#include "Arduino.h"
#include "ucmd_tests.h"
 
uc_test *test;

void test_print(const char *str, void *state) {
  Serial.print(str);
}

void setup() {                
  Serial.begin(9600);
  while (!Serial);
  test = uc_tests_get_test();
  uc_test_set_print_func(test, test_print);
}

void loop() {
  uc_test_run(test);
  delay(1000);
}

