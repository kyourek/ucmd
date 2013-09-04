#include "cl_tests.h"
#include <avr/pgmspace.h>
 
int led = 13;
cl_tests *tests_instance;

void default_print(const char *str) {
  Serial.print(str);
}

void setup() {                
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
  tests_instance = cl_tests_get_instance();
  cl_tests_set_print(tests_instance, default_print);
}

int loop_count = 0;

void loop() {
  loop_count++;
  Serial.println(loop_count, DEC);
  
  cl_tests_main(tests_instance);

  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}

