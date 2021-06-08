#include "processor.h"
#include <linux_parser.h>
#include <string>
#include <vector>

using std::string;
using std::vector;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_info = LinuxParser::CpuUtilization();

  long user = stol(cpu_info[0]);
  long nice = stol(cpu_info[1]);
  long system = stol(cpu_info[2]);
  long idle = stol(cpu_info[3]);
  long iowait = stol(cpu_info[4]);
  long irq = stol(cpu_info[5]);
  long soft_irq = stol(cpu_info[6]);
  long steal = stol(cpu_info[7]);

  long idle_all = idle + iowait;
  long non_idle = user + nice + system + irq + soft_irq + steal;

  long p_total = p_idle + p_non_idle;
  long total = idle_all + non_idle;

  long totald = total - p_total;
  long idled = idle_all - p_idle;
  p_idle = idle_all;
  p_non_idle = non_idle;
  return (float)(totald - idled) / totald;
}