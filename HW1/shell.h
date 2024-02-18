#ifndef _SHELL_
#define _SHELL_

typedef int bool;  //status setting

enum
{
   RUN_BAKC_PID = 1,
   COMMAND_SIZE = 20,
   INPUT_SIZE = 125,
};

void myShell();
void reset_all();
void read_command();
void pipeline_create();

#endif
