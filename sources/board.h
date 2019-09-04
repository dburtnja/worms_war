#pragma once

#include <unordered_map>
#include <unordered_set>
#include <thread>

#include "worm.h"

enum WormType {
  Lazy, Hunter, User
};

struct AdditionalWormInfo {
    int size;
};

struct Coordinates {
    int x;
    int y;
};

class Board {
 protected:
  std::vector<std::vector<int>> board_;
  std::unordered_map<int, std::thread> worms_;
  std::unordered_map<int, WormType> wormTypes_;
  std::unordered_map<int, AdditionalWormInfo> wormsInfo_;
  std::unordered_set<int> killed_;
  int nextId_;
  bool killAll_;
  mutable std::mutex mutex_;
  Coordinates mouseCoordinates_;

  int getNextId() { return nextId_++; };
  template <typename T>
  void emplace_worm(int id, int x, int y);

 public:
  Board(int width, int height);
  virtual ~Board();
  virtual void addWorm(WormType type, int x, int y);
  virtual int getWidth() const { return board_[0].size(); }
  virtual int getHeight() const { return board_.size(); }
  virtual bool isWormAt(int x, int y) const;
  virtual int at(int x, int y) const;
  virtual void update(int id, int oldX, int oldY, int newX, int newY);
  virtual WormType getWormType(int id) { return wormTypes_[id]; }
  virtual int getWormSize(int id) const;
  virtual bool checkKill(int id) const;
  virtual void killAll();
  virtual void clearDead();

  virtual void setMousePosition(int x, int y);
  virtual const Coordinates &getMousePosition() const;
};

