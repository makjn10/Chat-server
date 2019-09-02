//files to be included
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <string.h>
#include <time.h>

#include "server-getfile.h"
#include "server-sendfile.h"
#include "server-message.h"
