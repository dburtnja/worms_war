#include "board.h"


Board::Board(int width, int height) : nextId_{1}, killAll_{false} {
  if (width <= 0 or height <= 0)
      throw std::invalid_argument("Invalid board size. Should be grater than 0.");
  board_.resize(height);
  for (int i = 0; i < (int)board_.size(); ++i)
    board_[i].resize(width);
}

void Board::addWorm(WormType type, int x, int y) {
    int id = getNextId();

    board_[x][y] = id;
    wormTypes_[id] = type;
    switch (type) {
        case Lazy:
            emplace_worm<LazyWorm>(id, x, y);
            wormsInfo_[id] = {2};
            break;
        case Hunter:
            emplace_worm<HunterWorm>(id, x, y);
            wormsInfo_[id] = {3};
            break;
        case User:
            emplace_worm<UserWorm>(id, x, y);
            wormsInfo_[id] = {2};
    }
}

void Board::update(int id, int oldX, int oldY, int newX, int newY) {
  std::unique_lock<std::mutex> l(mutex_);

  if (checkKill(id))
      return;
  int survivorId = id;
  int targetId = board_[newX][newY];
  if (targetId != 0 && targetId != id) {
      int &size = wormsInfo_[id].size;
      int &targetSize = wormsInfo_[targetId].size;

      if (size >= targetSize) {
        size += targetSize;
        killed_.insert(targetId);
      } else {
        targetSize += size;
        killed_.insert(id);
        survivorId = targetId;
      }
  }
  board_[oldX][oldY] = 0;
  board_[newX][newY] = survivorId;
}

bool Board::checkKill(int id) const {
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

Board::~Board()  {
    killAll();
    for (auto &worm_thread : worms_)
        worm_thread.second.join();
}

int Board::at(int x, int y) const {
    if (x < 0 or y < 0 or x >= getHeight() or y >= getWidth())
        return -1;
    return board_[x][y];
}

template<typename T>
void Board::emplace_worm(int id, int x, int y) {
    worms_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(T(x, y, this), id)
    );
}

bool Board::isWormAt(int x, int y) const {
    return board_[x][y] != 0;
}

int Board::getWormSize(int id) const {
    try {
        return wormsInfo_.at(id).size;
    } catch (std::out_of_range &e) {
        throw std::out_of_range("Id '" + std::to_string(id) + "' don't exist.");
    }
}

void Board::setMousePosition(int x, int y) {
    mouseCoordinates_ = {x, y};
}

const Coordinates &Board::getMousePosition() const {
    return mouseCoordinates_;
}
