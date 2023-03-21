#include "Core.h"
#include <filesystem>
namespace fs = std::filesystem;


void get_pid(std::vector<std::string> &pid_vec)
{
	std::cout << "In get_pid\n";
	std::string path = "/proc";
    for (const auto& entry : fs::directory_iterator(path))
    {
        std::string entry_string = entry.path().string();
        std::string pid = get_num(entry_string);
        if (is_digit(pid))
        {
            pid_vec.push_back(pid);
            
        }
    }

}


 std::string get_parent_id(std::string& pid)
{
    const auto path = "/proc/" + pid + "/status";
    std::string ppid;
    std::ifstream file(path);
       
    int count = 0;
    while (std::getline(file, ppid)) 
    {
        ++count;
         if (count == 7) 
         {
            break;
         }
    }

    std::istringstream iss(ppid);
    std::string ppid_val;
    iss >> ppid_val >> ppid_val;
    return ppid_val;

}


 std::string get_command(std::string& pid)
{
     const auto cmdline_path = "/proc/" + pid + "/cmdline";
     std::string cmd;
     std::ifstream file(cmdline_path);
     std::getline(file, cmd);
     return cmd;
}


 void get_fd_files(std::string& pid)
 {  
     int good = 0;
     int er = 0;
     std::string fd_path = "/proc/" + pid + "/fd";
     try {
         for (const auto& entry : fs::directory_iterator(fd_path))
         {
             std::string fd = entry.path().string();
             analyze_fd(fd, pid);             
         }
     }
     catch (const std::system_error& error)
     {
         if (error.code() == std::errc::permission_denied) {
             std::cerr << "Error: Permission denied. Cannot access directory /fd>." << std::endl;
             std::cerr << "Try running the program with sudo to monitor all processes." << std::endl;
             
         }
         else {
             std::cerr << "Error: " << error.what() << std::endl;
             std::cerr << "Try running the program with sudo to monitor all processes." << std::endl;
             
         }
     }     
 }
 
 void get_inode(std::string& inode, std::string symlink)
 {

     size_t inode_pos = symlink.rfind("socket:[");
     if (inode_pos == std::string::npos) {
         return;
     }
     inode_pos += sizeof("socket:[") - 1;
     size_t inode_end_pos = symlink.find(']', inode_pos);
     if (inode_end_pos == std::string::npos) {
         return;
     }
     inode = symlink.substr(inode_pos, inode_end_pos - inode_pos);
     




 }

 void get_port(std::string symlink, std::string& pid)
 {
     std::string inode;
     get_inode(inode, symlink);



     std::string command = "lsof -w -n -i | grep -w " + inode;
     std::string port = "";
     FILE* file = popen(command.c_str(), "r");
     if (file) {
         char buffer[256];
         while (!feof(file)) {
             if (fgets(buffer, 256, file) != NULL) {
                 char* token = std::strtok(buffer, " ");
                 int i = 0;
                 while (token != NULL) {
                     if (i == 8) {
                         std::string str(token);
                         port = str.substr(str.find(":") + 1, str.find("(LISTEN)") - str.find(":") - 1);
                         break;
                     }
                     token = std::strtok(NULL, " ");
                     i++;
                 }
                 if (!port.empty()) {
                     break;
                 }
             }
         }
         pclose(file);
     }
     if (!port.empty()) {
         std::cout << symlink << " using port: " << port << std::endl;
     }
     else {
         std::cout << symlink << " is not using a port" << port << std::endl;
     }     
  }


 void analyze_fd(std::string& fd_path, std::string& pid)
 {
     
     char fd_path_buf[4096];
     ssize_t len = readlink(fd_path.c_str(), fd_path_buf, sizeof(fd_path_buf) - 1);
     if (len != -1) {
         fd_path_buf[len] = '\0';
         struct stat sb;
        if (stat(fd_path.c_str(), &sb) == -1) {
             std::cerr << "Failed to stat file descriptor " << fd_path << '\n';
             return;
             
        }
        std::string str(fd_path_buf);
        switch (sb.st_mode & S_IFMT) 
        {
            case S_IFBLK:  
                printf("Block device: %s\n", fd_path_buf);
                break;
            case S_IFCHR:     
                printf("Character device: %s\n", fd_path_buf);
                break;
            case S_IFDIR:  
                printf("Directory: %s\n", fd_path_buf);
                break;
            case S_IFIFO:  
                printf("FIFO/pipe: %s\n", fd_path_buf);
                break;
            case S_IFLNK:  
                printf("Symlink: %s\n", fd_path_buf);
                break;
            case S_IFREG:  
                printf("Regular file: %s\n", fd_path_buf);
                break;
            case S_IFSOCK: 
                get_port(str,pid);
                break;
            default:       
                printf("Non-categorized file: %s\n", fd_path_buf);
                break;
        }

     }
     else {
         perror("Failed to analyze symbolic link of fd");
         return;
     }
 }


 void get_info(std::string& pid)
{
    
     std::string ppid = get_parent_id(pid);
     std::string cmd = get_command(pid);
     std::cout  << "PID: " << pid << " PARENT ID: " << ppid << " Command: " << cmd << "\n";
     std::cout << "FILES OPEN BY PROCESS BELOW!\n";
     std::cout << "-------------------------------------------------\n";
     get_fd_files(pid);
     std::cout << "-------------------------------------------------\n";
     std::cout << "-------------------------------------------------\n";
      

}


std::string get_num(std::string& entry)
{
    
    std::vector<std::string> items;
    size_t pos = 0;
    std::string delimiter = "/";
    while ((pos = entry.find(delimiter)) != std::string::npos)
    {
        std::string token = entry.substr(0, pos);
        items.push_back(token);
        entry.erase(0, pos + delimiter.length());
    }
    items.push_back(entry);

    
    return items.back();
}


bool is_digit(std::string &pid)
{
    std::size_t found = pid.find_first_not_of("0123456789");
    if (found != std::string::npos)
    {
        return false;
    }
    return true;
}


void start() 
{
	std::vector<std::string> pid_vec;
	get_pid(pid_vec);
    std::cout << "|------------------START-------------------------|\n";
    for (auto& pid : pid_vec)
    {
        
        get_info(pid);
    }
    

}