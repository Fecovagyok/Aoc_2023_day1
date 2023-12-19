#include "readin.h"

#include <iostream>
#include <fstream>

int AoCReader::read(){
  std::ios_base::sync_with_stdio(false);
  std::ifstream input{file_name};
  if (input.fail()) {
    std::cerr << "No input file bro" << std::endl;
    return -1;
  }

  while (true) {
    std::getline(input, buf);
    if (input.eof()) {
      break;
    }
    if (input.fail()) {
      std::cerr << "Fail before eof? How did we get here?" << std::endl;
      return -2;
    }
    callback(buf);    
  }
  return 0;
}
