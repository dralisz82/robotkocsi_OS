#include "mbed.h"
#include "rtos.h"
#include <ctype.h>
#include "commandhandler.h"

char bt_receive_buffer[BT_RB_SIZE];
unsigned int bt_rb_pos = 0;

Mail<simplestr, 16> cmdMailBox;
Thread cmdHandlerThread;

extern RawSerial BT;
extern DigitalOut cmdExecLED;
/**
 * UART Interrupt handler
 */
void gotChar() {
    simplestr *mail;
    char c = BT.getc(); // read incoming character

    if(c != '\n')
        bt_receive_buffer[bt_rb_pos++] = c; // receive characters till EOL
    else
        bt_receive_buffer[bt_rb_pos++] = '\0';  // replace EOL with null terminator

    if(c == '\n' || bt_rb_pos == BT_RB_SIZE) {    // if EOL or buffer is full...
        if(bt_rb_pos == BT_RB_SIZE)
            bt_receive_buffer[BT_RB_SIZE - 1] = '\0'; // Better would be some error handling here
        cmdExecLED = 0; // turn on command execution LED
        mail = cmdMailBox.alloc(2); // 2 ms timeout
        if(mail != NULL) {
            strcpy(*mail, bt_receive_buffer);
            cmdMailBox.put(mail); // sending command to handleCommand() via mailbox
        }
        bt_rb_pos = 0;
    }
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

void handleCommand() {
    
    char *trimmedCommand;
    const char separator[2] = " ";
    char *token;
    
    char cmd[BT_RB_SIZE];
    char args[4][BT_RB_SIZE] = {0};
    int argIdx = 0;

    osEvent evt = cmdMailBox.get(); // receiving command from UART Interrupt handler
    if (evt.status != osEventMail)
      return;
    
    simplestr *mail = (simplestr*)evt.value.p;
    trimmedCommand = trimwhitespace(*mail);
    printf("cmd: >%s<\n", trimmedCommand);
    
    /* get the first token */
    token = strtok(trimmedCommand, separator); // TODO: replace with strtok_r()
    
    if(token == NULL)
      return;
    
    strcpy(cmd, token);
    
    /* walk through arguments */
    while( (token = strtok(NULL, separator)) != NULL ) {
      strcpy(args[argIdx++], token);
    }

    cmdMailBox.free(mail);
    execCommand(cmd, argIdx, args);
    cmdExecLED = 1; // turn off command execution LED
}

void cmdHandlerMain(void) {
    printf("Command handler started\n");
    while(1) {
        handleCommand();
//        Thread::wait(2000); // For testing purposes only
    }
}
