//
// Created by Shihao Jing on 11/4/17.
//

#include "tree.h"
#include <iostream>

int main() {
  tree c(100);
  int data[] = {1, 2, 5};
  bool results[3];
  c.insert(data, results, 3);
  for (int i = 0; i < c.getSize(); ++i) {
    std::cout << c.getElement(i) << std::endl;
  }

  c.remove(data, results, 2);
  for (int i = 0; i < c.getSize(); ++i) {
    std::cout << c.getElement(i) << std::endl;
  }

}