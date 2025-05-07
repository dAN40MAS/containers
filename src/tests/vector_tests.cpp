#include <gtest/gtest.h>

#include "../s21_containers.h"

namespace s21 {

class VectorTest : public ::testing::Test {
 protected:
  Vector<int> vec;

  void SetUp() override { vec = Vector<int>(); }
};

TEST_F(VectorTest, DefaultConstructor) {
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.capacity(), 0);
}

TEST_F(VectorTest, ConstructorWithSize) {
  Vector<int> v(5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.capacity(), 5);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(v.at(i), 0);
  }
}

TEST_F(VectorTest, InitializerListConstructor) {
  Vector<int> v = {1, 2, 3};
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.capacity(), 3);
  EXPECT_EQ(v.at(0), 1);
  EXPECT_EQ(v.at(1), 2);
  EXPECT_EQ(v.at(2), 3);
}

TEST_F(VectorTest, PushBack) {
  vec.push_back(1);
  vec.push_back(2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(1), 2);
}

TEST_F(VectorTest, PopBack) {
  vec.push_back(1);
  vec.push_back(2);
  vec.pop_back();
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.at(0), 1);
}

TEST_F(VectorTest, Reserve) {
  vec.reserve(10);
  EXPECT_GE(vec.capacity(), 10);
}

TEST_F(VectorTest, Clear) {
  vec.push_back(1);
  vec.push_back(2);
  vec.clear();
  EXPECT_EQ(vec.size(), 0);
}

TEST_F(VectorTest, CopyConstructor) {
  vec.push_back(1);
  vec.push_back(2);
  Vector<int> copy(vec);
  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.at(0), 1);
  EXPECT_EQ(copy.at(1), 2);
}

TEST_F(VectorTest, MoveConstructor) {
  vec.push_back(1);
  vec.push_back(2);
  Vector<int> moved(std::move(vec));
  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(moved.at(0), 1);
  EXPECT_EQ(moved.at(1), 2);
  EXPECT_TRUE(vec.empty());
}

TEST_F(VectorTest, CopyAssignment) {
  vec.push_back(1);
  vec.push_back(2);
  Vector<int> copy;
  copy = vec;
  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.at(0), 1);
  EXPECT_EQ(copy.at(1), 2);
}

TEST_F(VectorTest, MoveAssignment) {
  vec.push_back(1);
  vec.push_back(2);
  Vector<int> moved;
  moved = std::move(vec);
  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(moved.at(0), 1);
  EXPECT_EQ(moved.at(1), 2);
  EXPECT_TRUE(vec.empty());
}

TEST_F(VectorTest, Insert) {
  vec.push_back(1);
  vec.push_back(3);
  vec.insert(vec.begin() + 1, 2);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(1), 2);
  EXPECT_EQ(vec.at(2), 3);
}

TEST_F(VectorTest, Erase) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.erase(vec.begin() + 1);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(1), 2);
}

TEST_F(VectorTest, ShrinkToFit) {
  vec.reserve(10);
  vec.push_back(1);
  vec.shrink_to_fit();
  EXPECT_EQ(vec.capacity(), 1);
}

TEST_F(VectorTest, Front) {
  vec.push_back(1);
  EXPECT_EQ(vec.front(), 1);
}

TEST_F(VectorTest, Back) {
  vec.push_back(1);
  vec.push_back(2);
  EXPECT_EQ(vec.back(), 2);
}

TEST_F(VectorTest, Empty) {
  EXPECT_TRUE(vec.empty());
  vec.push_back(1);
  EXPECT_FALSE(vec.empty());
}

TEST_F(VectorTest, Data) {
  vec.push_back(1);
  EXPECT_EQ(*vec.data(), 1);
}

}  // namespace s21
