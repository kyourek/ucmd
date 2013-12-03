#include "Arduino.h"
#include "ucmd_tests.h"
 
uc_tests *tests_instance;

void tests_print(const char *str) {
  Serial.print(str);
}

void setup() {                
  Serial.begin(9600);
  while (!Serial);
  tests_instance = uc_tests_get_instance();
  uc_tests_set_print(tests_instance, tests_print);
}

void loop() {
  uc_tests_main(tests_instance);
  delay(1000);
}

