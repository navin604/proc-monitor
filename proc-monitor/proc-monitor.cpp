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
	std::cout << "Start function\n";
	std::vector<std::string> pid_vec;
	get_pid(pid_vec);

}



