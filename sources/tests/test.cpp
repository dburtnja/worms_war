#include <gtest/gtest.h>
#include "../board.h"
#include <set>

class MockBoard : public Board {
 public:
  int recentId_;
  MockBoard(int w, int h) : Board(w,h) { }
  virtual void addWorm(WormType type, int x, int y) override {
    board_[x][y] = nextId_;
    recentId_ = nextId_;
    nextId_++;
  }
};

class MockWorm : public Worm {
 public:
  MockWorm(int x, int y, Board* b) : Worm(x,y,b) {
    currDir_ = LEFT;
  }
  void setId(int id) { id_ = id; }
  int getId() const { return id_; }
  void run() { }
  std::pair<int,int> getPos() const { return {x_,y_}; }
};

class MockBoardNoUpdate : public Board {
 public:
  MockBoardNoUpdate(int w, int h) : Board(w, h) { }
  virtual void update(int id, int oldX, int oldY, int newX, int newY) override { }
};

TEST(wormTest, testWormMove) {
  MockBoard b(50, 50);
  MockWorm w(5, 4, &b);
  b.addWorm(Lazy, 5, 4);
  w.setId(b.recentId_);
  w.move();
  EXPECT_FALSE(b.isWormAt(5,4));
  EXPECT_TRUE(b.isWormAt(4,4));
  EXPECT_EQ(b.at(4,4),w.getId());
  w.move();
  EXPECT_FALSE(b.isWormAt(4,4));
  EXPECT_TRUE(b.isWormAt(3,4));
  EXPECT_EQ(b.at(3,4),w.getId());
}

TEST(boardTest, testInitialAddWorms) {
  int w = 60, h = 50;
  MockBoardNoUpdate b(w,h);
  std::set<std::pair<int,int>> worms = { {3,4}, {5,6}, {7,8} };
  for (auto& worm : worms)
    b.addWorm(Lazy,worm.first,worm.second);

  for (int j = 0; j < w; ++j)
    for (int i = 0; i < h; ++i)
      if (worms.find({i,j}) == worms.end())
        EXPECT_FALSE(b.isWormAt(i,j));
      else
        EXPECT_TRUE(b.isWormAt(i,j));
  b.killAll();
}

TEST(boardTest, testBoardSizePropagation) {
  int w = 23, h = 187;
  Board b(w, h);
  EXPECT_EQ(b.getWidth(), w);
  EXPECT_EQ(b.getHeight(), h);
}

TEST(boardTest, testInitEmptyBoard) {
  int w = 25, h = 30;
  Board b(w, h);
  for (int j = 0; j < w; ++j) {
    for (int i = 0; i < h; ++i) {
      EXPECT_FALSE(b.isWormAt(i,j));
      EXPECT_EQ(b.at(i,j), 0);
    }
  }
}

TEST(boardTest, testIncorrectSize) {
  ASSERT_THROW(Board b(0,0), std::exception);
  ASSERT_THROW(Board b(15,-2), std::exception);
  ASSERT_THROW(Board b(-1,0), std::exception);
}

TEST(boardTest, testWholeRun) {
  int w = 50, h = 50;
  Board b(w, h);
  for (int i = 0; i < 20; ++i)
    b.addWorm(Lazy, i, 13);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  b.killAll();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
