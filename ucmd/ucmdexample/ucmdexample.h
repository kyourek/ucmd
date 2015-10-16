#ifndef UCMDEXAMPLE_H
#define UCMDEXAMPLE_H

#include "ucmd.h"

uc_EXPORTED void    uc_example(ucCmdLine_TransmitFunc *transmit, ucCmdLineApp_ReceiveFunc *receive);
uc_EXPORTED void    uc_hello_world(ucCmdLine_TransmitFunc *transmit, ucCmdLineApp_ReceiveFunc *receive);

#endif
