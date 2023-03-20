#ifndef PROC_H
#define PROC_H

#include "Core.h"

void get_pid(std::vector<std::string> &pid_vec);
std::string get_num(std::string &entry);
bool is_digit(std::string& pid);
void start();


#endif
