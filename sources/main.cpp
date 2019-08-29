#include <iostream>
#include "graphics.h"
#include "worm.h"

#include "board.h"

int main(int argc, char** argv) {
  std::cout << "Worms app\n";
  return Graphics::createAndRun(argc, argv, 20, 20, true);
}
