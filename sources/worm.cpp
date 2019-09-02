#include "worm.h"
#include "board.h"

#include <queue>
#include <unordered_set>
#include <vector>

#include <iostream>
#include <mutex>

using std::pair;
using std::make_pair;
using std::queue;
using std::unordered_set;
using std::hash;
using std::vector;

void Worm::move() {
  int oldX = x_, oldY = y_;
  int board_width = board_->getWidth();
  int board_height = board_->getHeight();

  switch (currDir_) {
      case RIGHT:
          x_ = (x_ + 1) % board_width;
          break;
      case UP:
          y_ = (y_ + 1) % board_height;
          break;
      case LEFT:
          x_ > 0 ? --x_ : board_width - 1;
          break;
      case DOWN:
          y_ > 0 ? --y_ : board_height - 1;
          break;
  }
  board_->update(id_,oldX,oldY, x_, y_);
}

void LazyWorm::run() {
  while (alive) {
    if (board_->checkKill(id_)) {
      alive = false;
      break;
    }
    // Pick waiting time between 0.1s and 1s.
    int delay = rand() % 901;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + delay));

    // Pick the direction - with probability 0.75 stay with current direction,
    // with probability 0.125 turn right and with 0.125 turn left.
    int dice = rand() % 8;
    if (dice == 0) { // turn right
      currDir_++;
      currDir_ %= 4;
    } else if (dice == 1) { // turn left
      (int)currDir_--;
      if (currDir_ < 0) currDir_ += 4;
    }
    this->move();
  }
}

void HunterWorm::run() {
  while (alive) {
    if (board_->checkKill(id_)) {
      alive = false;
      break;
    }

    // Pick waiting time between 0.1s and 1s.
    int delay = rand() % 901;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + delay));

    pair<int,int> target, myPos = {x_,y_};
    if (goalId_ != -1) {
      target = findClosest(goalId_, goalPos_);
      if (target.first < 0)
        target = findClosest(-1, myPos);
    } else {
      target = findClosest(-1, myPos);
    }

    if (target.first < 0) {
      if (goalPos_ == myPos) {
        target = make_pair(rand() % board_->getWidth(),
                           rand() % board_->getHeight());
      }
    }
    goalPos_ = target;
    int newId = board_->at(goalPos_.first, goalPos_.second);
    goalId_ = newId > 0 ? newId : -1;

    vector<int> dirs;
    int dx = goalPos_.first - myPos.first,
        dy = goalPos_.second - myPos.second;
    if (dx > 0) dirs.push_back(RIGHT);
    if (dx < 0) dirs.push_back(LEFT);
    if (dy > 0) dirs.push_back(UP);
    if (dy < 0) dirs.push_back(DOWN);

    if(!dirs.empty())
      currDir_ = dirs[rand() % dirs.size()];
    this->move();
  }
  std::cout << "Id " << id_ << " has died\n";
}

struct pair_hash
{
  template <class T1, class T2>
  size_t operator() (const pair<T1, T2> &pair) const
  {
    return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
  }
};

pair<int,int> HunterWorm::findClosest(int id, pair<int,int> start) {
  queue<pair<int,int>> q;
  unordered_set<pair<int,int>,pair_hash> visited;
  int dx[] = {-1, 0, 1, 0},
      dy[] = { 0,-1, 0, 1};

  visited.insert(start);
  q.push(start);
  while (!q.empty()) {
    pair<int,int> v = q.front();
    q.pop();
    for (int i = 0; i < 4; ++i) {
      int newX = v.first + dx[i], newY = v.second + dy[i];
      pair<int,int> newV = make_pair(newX,newY);
      if (newX < 0 || newX >= board_->getWidth()) continue;
      if (newY < 0 || newY >= board_->getHeight()) continue;
      if (visited.find(newV) != visited.end()) continue;

      if (id < 0) {
        if (board_->at(newX,newY) > 0) return newV;
      } else {
        if (board_->at(newX,newY) == id) return newV;
      }

      visited.insert(newV);
      q.push(newV);
    }
  }

  return make_pair(-1,-1);
}
