#include "linux_parser.h"

#include <dirent.h>
#include <process.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE DC: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string line;
  string key;
  string value;
  float total_memory, free_memory, buffers;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "MemTotal:") {
          if (linestream >> value) {
            total_memory = stof(value);
          }
        }
        if (key == "MemAvailable:") {
          if (linestream >> value) {
            free_memory = stof(value);
          }
        }
        if (key == "Buffers:") {
          if (linestream >> value) {
            buffers = stof(value);
          }
        }
      }
    }
  }
  return 1.0 - (free_memory / (total_memory - buffers));
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream file(kProcDirectory + kUptimeFilename);
  string uptime, idletime, line;
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);
    string up, idle;
    linestream >> uptime >> idle;
  }

  return stol(uptime);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE : Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> output;
  std::ifstream file(kProcDirectory + kStatFilename);
  string line, value;
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        output.push_back(value);
      }
    }
  }
  return output;
}

// DONE - TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream file(kProcDirectory + kStatFilename);
  string line, key, value;
  int total_processes;
  if (file.is_open()) {
    while (getline(file, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          if (linestream >> value) {
            total_processes = stoi(value);
          }
        }
      }
    }
  }
  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream file(kProcDirectory + kStatFilename);
  string line, key, value;
  int procs_running;
  if (file.is_open()) {
    while (getline(file, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> value;
          procs_running = stoi(value);
        }
      }
    }
  }

  return procs_running;
}

// DONE: Read and return CPU utilization

float LinuxParser::CpuUtilization(int  pid) {


  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  string line, value;
  vector<string> cpu_info;
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 22; i++) {
      linestream >> value;
      cpu_info.push_back(value);
    }
  }
  float uptime = LinuxParser::UpTime();
  float utime = stol(cpu_info[13]);
  float stime = stol(cpu_info[14]);
  float cutime = stol(cpu_info[15]);
  float cstime = stol(cpu_info[16]);
  float starttime = UpTime(pid);
  float freq = sysconf(_SC_CLK_TCK);
  float total = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / freq);
  float cpu_utilization;
  // Calculation acording to stackoverflow question
  // provided by Udacity

    cpu_utilization = ((total / freq) / seconds);
 
  return cpu_utilization;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (file.is_open()) {
    getline(file, line);
  }
  return line;
}

// DONE - DC: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, value;
  long valueMb;

  vector<string> values;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::istringstream linestream(line);

      while (linestream >> key) {
        if (key == "VmSize:") {
          linestream >> value;
          valueMb = stol(value) / 1000;
        }
      }
    }
  }
  return to_string(valueMb);
}

// DONE-DC: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  string key, line, uid;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> uid;
        }
      }
    }
  }
  return uid;
}

// DONE-DC: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string x, key, line, user;
  std::ifstream file(kPasswordPath);

  if (file.is_open()) {
    while (getline(file, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> user >> x >> key) {
        if (key == uid) {
          return user;
        }
      }
    }
  }

  return user;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  string line, value;
  vector<string> process_inf;
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 22; i++) {
      linestream >> value;
      process_inf.push_back(value);
    }
  }
  long uptime = stol(process_inf[21]);
  return uptime;
}
