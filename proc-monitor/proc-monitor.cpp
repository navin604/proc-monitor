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



void get_info(std::string& pid)
{
    
    std::string ppid = get_parent_id(pid);
    std::string cmd = get_command(pid);
   
    if (pid == "2065") {
        std::cout << pid << "\n";
        std::cout << ppid << "\n";
        std::cout << cmd << "\n";
    }
    
    


}


std::string get_num(std::string& entry)
{
    // Split path using delimiter '/'
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

    // Return PID
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
    std::cout << "| Parent ID  |   Process ID   |   Command Line   |\n";
    std::cout << "-------------------------------------------------\n";
    for (auto& pid : pid_vec)
    {
        get_info(pid);
    }
    

}



