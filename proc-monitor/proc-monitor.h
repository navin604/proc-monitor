#ifndef PROC_H
#define PROC_H

#include "Core.h"

void get_pid(std::vector<std::string>& pid_vec);
void get_info(std::vector<std::string>& pid_vec);

std::string get_parent_id(std::string& pid);

std::string get_command(std::string& pid);



std::string get_num(std::string &entry);
bool is_digit(std::string& pid);
void start();


#endif
