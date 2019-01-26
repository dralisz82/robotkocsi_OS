// receive buffer size is one character bigger than max. acceptable command length to leave space for null terminator
#define BT_RB_SIZE 33

typedef char simplestr[BT_RB_SIZE];

void gotChar();
void cmdHandlerMain(void);
void handleCommand();
void execCommand(char *cmd, int argc, simplestr *args);
