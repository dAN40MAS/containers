#include <gtest/gtest.h>

#include "../s21_containers.h"

namespace s21 {
class StackTest : public ::testing::Test {
 protected:
  Stack<int> stack;
};

TEST_F(StackTest, DefaultConstructor) {
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
}

TEST_F(StackTest, Push) {
  stack.push(1);
  stack.push(2);
  EXPECT_EQ(stack.size(), 2);
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.top(), 2);
}

TEST_F(StackTest, Pop) {
  stack.push(1);
  stack.push(2);
  stack.pop();
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.top(), 1);
}

TEST_F(StackTest, Top) {
  stack.push(1);
  EXPECT_EQ(stack.top(), 1);
  stack.push(2);
  EXPECT_EQ(stack.top(), 2);
}

TEST_F(StackTest, PopEmptyStack) {
  EXPECT_THROW(stack.pop(), std::out_of_range);
}

TEST_F(StackTest, TopEmptyStack) {
  EXPECT_THROW(stack.top(), std::out_of_range);
}

TEST_F(StackTest, Clear) {
  stack.push(1);
  stack.push(2);
  stack.clear();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
}

TEST_F(StackTest, CopyConstructor) {
  stack.push(1);
  stack.push(2);
  Stack<int> copy(stack);
  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.top(), 2);
}

TEST_F(StackTest, MoveConstructor) {
  stack.push(1);
  stack.push(2);
  Stack<int> moved(std::move(stack));
  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(moved.top(), 2);
  EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, CopyAssignment) {
  stack.push(1);
  stack.push(2);
  Stack<int> copy;
  copy = stack;
  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.top(), 2);
}

TEST_F(StackTest, InsertMany) {
  Stack<int> stack2;
  stack2.insert_many_back(1, 2, 3, 4, 5);
  EXPECT_EQ(stack2.size(), 5);
  EXPECT_EQ(stack2.top(), 5);
}

TEST_F(StackTest, MoveAssignment) {
  stack.push(1);
  stack.push(2);
  Stack<int> moved;
  moved = std::move(stack);
  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(moved.top(), 2);
  EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, Swap) {
  stack.push(1);
  Stack<int> other;
  other.push(2);
  stack.swap(other);
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.top(), 2);
  EXPECT_EQ(other.size(), 1);
  EXPECT_EQ(other.top(), 1);
}

}  // namespace s21
