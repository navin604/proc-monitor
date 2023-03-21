#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "proc-monitor.h"
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// Included to ensure <arpa/inet.h> is compatible on multible systems
#include <sys/types.h>
#include <regex>
#include <array>
#include <cstring>