// TODO: Read and return the system memory utilization

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::stol;
vector<string> CpuUtilization() {
  vector<string> output;
  std::ifstream file("/proc/stat");
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
  file.close();
  return output;
}


float CpuUtilizationP() {
  vector<string> cpu_previnfo =CpuUtilization();  // Reading the information of the cpu and
                                      // labeling it as previous mesurement

  long prev_user = stol(cpu_previnfo[0]);
  long prev_nice = stol(cpu_previnfo[1]);
  long prev_system = stol(cpu_previnfo[2]);
  long prev_idle = stol(cpu_previnfo[3]);
  long prev_iowait = stol(cpu_previnfo[4]);
  long prev_irq = stol(cpu_previnfo[5]);
  long prev_soft_irq = stol(cpu_previnfo[6]);
  long prev_steal = stol(cpu_previnfo[7]);

  // Calcultation of the times as stackoverflow question
  float prev_idle_all = prev_idle + prev_iowait;
  float prev_non_idle = prev_user + prev_nice + prev_system + prev_soft_irq +
                       prev_irq + prev_steal;
  float prev_total = prev_idle_all + prev_non_idle;

  vector<string> cpu_info = CpuUtilization();

  long user = stol(cpu_info[0]);
  long nice = stol(cpu_info[1]);
  long system = stol(cpu_info[2]);
  long idle = stol(cpu_info[3]);
  long iowait = stol(cpu_info[4]);
  long irq = stol(cpu_info[5]);
  long soft_irq = stol(cpu_info[6]);
  long steal = stol(cpu_info[7]);

  float idle_all = idle + iowait;
  float non_idle = user + nice + system + irq + soft_irq + steal;
  float total = idle_all + non_idle;

  float  totald =  total - prev_total;
  float idled = idle_all - prev_idle_all;

 std::cout<<"\n||||||||||| CPU: "<< user << " prev user_: "<< prev_user; 
   return (totald - idled)/totald;

 
}
int main() {
  std::ifstream filestream("/proc/meminfo");
  string line;
  string key;
  string value;
  string unit;
  float free_memory, total_memory, buffers;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "MemTotal:") {
          if (linestream >> value) {
            std::cout << "MemTotal: " << value << "\n";
            total_memory = stof(value);
          }
        }
        if (key == "MemAvailable:") {
          if (linestream >> value) {
            std::cout << "Memfree: " << value << "\n";
            free_memory = stof(value);
          }
        }
        if (key == "Buffers:") {
          if (linestream >> value) {
            std::cout << "Buffers: " << value << "\n";

            buffers = stof(value);
          }
        }
      }
    }
  }

  // std::cout<<((free_memory / (total_memory - buffers)));

  std::ifstream file("/proc/uptime");
  string uptime, idletime, linet;
  if (file.is_open()) {
    std::getline(file, linet);
    std::istringstream linestream(linet);
    linestream >> uptime >> idletime;
  }

  // std::cout<<stol(uptime)<<"\n"<<idletime;
  vector<string> cpu = CpuUtilization();
  std::cout << cpu.size() << "\n"
            << cpu[0] << " : " << cpu[1] << " : " <<cpu[2] << " : " << cpu[3]
            << " : " <<cpu[4] << " : "<< cpu[5] << " : "<< cpu[6] << " : "<< cpu[7]
            << " : " <<cpu[8] << " : "<< cpu[9] << " : ";
  float cpu_ = CpuUtilizationP();
}
