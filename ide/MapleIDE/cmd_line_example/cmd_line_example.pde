#include "cmd_line_example.h"

static cl_app *app;

static void send_data(const char *data) {
  SerialUSB.println(data);
}

static char *receive_data(char *buf, size_t buf_size) {
  char r = 0;
  size_t i = 0;
  while (!SerialUSB.available());  
  while (i < buf_size - 1) {
     if (SerialUSB.available()) {
       r = SerialUSB.read();
       if (r == ';') {
         break;
       }
       buf[i] = r;
       i++;
       buf[i] = '\0';
     }
  }

  return buf;
}

void setup() {
  app = cl_math_app_get_instance();
  cl_app_set_send_data(app, send_data);
  cl_app_set_receive_data(app, receive_data);  
  while (!SerialUSB.available()) {
    SerialUSB.println("Input any key to begin."); 
    delay(500);
  }
  while (SerialUSB.available()) SerialUSB.read();
}

void loop() {
  cl_app_run(app);
  delay(500);
}

