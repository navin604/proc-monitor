#ifndef PROC_H
#define PROC_H

#include "Core.h"

void get_pid(std::vector<std::string>& pid_vec);

void get_info(std::string& pid);

std::string get_parent_id(std::string& pid);

std::string get_command(std::string& pid);

void get_fd_files(std::string& pid);

void get_port(std::string symlink, std::string& pid);

std::string get_num(std::string& entry);

bool is_digit(std::string& pid);

void analyze_fd(std::string& fd, std::string& pid);

void start();

void get_inode(std::string& inode, std::string symlink);

#endif
