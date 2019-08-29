#include "board.h"


Board::Board(int width, int height) : nextId_{1}, killAll_{false} {
  board_.resize(height);
  for (int i = 0; i < (int)board_.size(); ++i)
    board_[i].resize(width);
}

void Board::addWorm(WormType type, int x, int y) {
  // TODO: Implement the body of the function. It should add a worm
  // into the board_ and start a new thread which will invoke
  // call operator - operator()(int) - defined in Worm class.
  // Note: there are a few class members (like wormTypes_) that need to be
  // updated as well to have full information about the worm that is being
  // added. They are usually based on worm's id which should be uniquely given
  // here.
}

void Board::update(int id, int oldX, int oldY, int newX, int newY) {
  int targetId = board_[newX][newY];
  if (targetId != 0 && targetId != id)
    killed_.insert(targetId);
  board_[oldX][oldY] = 0;
  board_[newX][newY] = id;
}

bool Board::checkKill(int id) {
  return killAll_ || killed_.find(id) != killed_.end();
}

void Board::killAll() {
  killAll_ = true;
}

void Board::clearDead() {
  for (int i = 0; i < (int)board_.size(); ++i)
    for (int j = 0; j < (int)board_[0].size(); ++j)
      if (killed_.find(board_[i][j]) != killed_.end())
        board_[i][j] = 0;
}
