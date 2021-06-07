#include "format.h"

#include <cmath>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hours, minutes, seconds_remain;
  string output;
  minutes = (seconds / 60) % 60;
  hours = seconds / 3600;
  seconds_remain = seconds % 60;
  if (seconds_remain < 10) {
    output = std::to_string(hours) + ":" + std::to_string(minutes) + ":0" +
             std::to_string(seconds_remain);
  }else if(seconds_remain >= 10){
    output = std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
             std::to_string(seconds_remain);

  }else{
    output = std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
             std::to_string(seconds_remain);

  }
    
     return  output;
}