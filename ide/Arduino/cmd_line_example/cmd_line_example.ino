#include "cl_cmd_line.h"
#include "cl_cmd_line_app.h"
#include "cl_cmd_line_opt.h"
#include "cl_math_app.h"
 
int led = 13;
int loop_count = 0;
cl_cmd_line_app *cmd_line_app;
cl_cmd_line_opt *cmd_line_opt;

char *default_receive(char *buf, int buf_size) {
  int i = 0;
  char r = 0;
  while (!Serial.available()) {
    continue; 
  }
  
  while (i < buf_size - 1) {
     if (Serial.available()) {
       r = Serial.read();
       buf[i] = r;
       i++;
       buf[i] = '\0';
       if (r == '\n') {
         break;
       }
     }
  }

  return buf;
}

void default_transmit(const char *response) {
  Serial.println(response); 
}

void setup() {                
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  while (!Serial) {
    ;
  }  
  cmd_line_opt = cl_math_app_create_cmd_opt();
  cmd_line_app = cl_cmd_line_app_get_instance();
  cl_cmd_line_app_set_receive(cmd_line_app, default_receive);
  cl_cmd_line_set_transmit(cl_cmd_line_app_get_cmd(cmd_line_app), default_transmit);
}

void loop() {
  cl_cmd_line_app_run(cmd_line_app, cmd_line_opt);
}

