#pragma once

#include <vector>
#include <thread>
#include <mutex>

constexpr int LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3;

class Board;

class Worm {
 protected:
  int id_;
  int x_, y_; // Coordinates
  int currDir_;
  bool alive;
  Board* board_; // No ownership, should be shared_ptr instead.

 public:
  Worm(int x, int y, Board* b)
    : x_{x}, y_{y}, currDir_{rand()%4}, alive{true}, board_{b} {}
  virtual ~Worm() { }
  virtual void move();
  void kill() { alive = false; }
  virtual void run() = 0;
  virtual void operator()(int id) { id_ = id; run(); }
  int getId() const { return id_; }
};

class LazyWorm : public Worm {
 public:
  LazyWorm(int x, int y, Board* b) : Worm(x,y,b) { }
  void run();
};

class HunterWorm : public Worm {
 private:
  std::pair<int,int> goalPos_ = {0,0};
  int goalId_ = -1;

  std::pair<int,int> findClosest(int id, std::pair<int,int> start);
 public:
  HunterWorm(int x, int y, Board* b) : Worm(x,y,b) { }
  void run();
};
