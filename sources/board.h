#pragma once

#include <unordered_map>
#include <unordered_set>
#include <thread>

#include "worm.h"

enum WormType {
  Lazy, Hunter
};

class Board {
 protected:
  std::vector<std::vector<int>> board_;
  std::unordered_map<int, std::thread> worms_;
  std::unordered_map<int, WormType> wormTypes_;
  std::unordered_set<int> killed_;
  int nextId_;
  bool killAll_;

  int getNextId() { return nextId_++; };

 public:
  Board(int width, int height);
  virtual ~Board();
  virtual void addWorm(WormType type, int x, int y);
  virtual int getWidth() const { return board_[0].size(); }
  virtual int getHeight() const { return board_.size(); }
  virtual bool isWormAt(int x, int y) const { return board_[x][y] != 0; }
  virtual int at(int x, int y) const;
  virtual void update(int id, int oldX, int oldY, int newX, int newY);
  virtual WormType getWormType(int id) { return wormTypes_[id]; }
  virtual bool checkKill(int id);
  virtual void killAll();
  virtual void clearDead();
};
