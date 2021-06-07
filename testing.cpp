// TODO: Read and return the system memory utilization

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

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

  //std::cout<<((free_memory / (total_memory - buffers)));

  std::ifstream file("/proc/uptime");
  string uptime, idletime, linet;
  if (file.is_open()) {
    std::getline(file, linet);
    std::istringstream linestream(linet);
    linestream >> uptime >> idletime;
  }

  std::cout<<stol(uptime)<<"\n"<<idletime;
}
