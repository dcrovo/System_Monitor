#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::stol;
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
   cpu_utilization = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return LinuxParser::CpuUtilization(pid_);
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime() -
         (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
}

/*void Process::CpuUtilization(int pid) {
  vector<string> cpu_info = LinuxParser::CpuUtilization(pid);
  long uptime = LinuxParser::UpTime();
  long utime = stol(cpu_info[13]);
  long stime = stol(cpu_info[14]);
  long cutime = stol(cpu_info[15]);
  long cstime = stol(cpu_info[16]);
  long starttime = stol(cpu_info[21]);
  long freq = sysconf(_SC_CLK_TCK);
  long total = utime + stime + cutime + cstime;
  long seconds = uptime - (starttime / freq);

  // Calculation acording to stackoverflow question
  // provided by Udacity

  if (seconds != 0) {
    cpu_utilization = float(100 * ((total / freq) / seconds));
  }

  else {
    cpu_utilization = 0.0;
  }
}*/
// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a ) const {
  return cpu_utilization>a.cpu_utilization;
}
