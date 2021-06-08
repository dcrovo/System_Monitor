#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long p_idle = 0;
 long p_non_idle = 0;
 long idle;
 long non_idle;
};

#endif