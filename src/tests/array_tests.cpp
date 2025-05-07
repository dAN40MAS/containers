#include <gtest/gtest.h>

#include "../s21_containersplus.h"

namespace s21 {

class ArrayTest : public ::testing::Test {
 protected:
  array<int, 5> arr;  // Create an array with 5 elements

  void SetUp() override {
    // Initialize the array with some values
    arr.fill(10);
  }
};

// Test default constructor and filling with default values
TEST_F(ArrayTest, DefaultConstructor) {
  array<int, 5> default_arr;
  EXPECT_EQ(default_arr.size(), 5);
  EXPECT_EQ(default_arr[0],
            0);  // Default initialized values should be 0 for `int`
}

// Test initializer list constructor
TEST_F(ArrayTest, InitializerListConstructor) {
  array<int, 5> init_arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(init_arr.size(), 5);
  EXPECT_EQ(init_arr[0], 1);
  EXPECT_EQ(init_arr[4], 5);
}

// Test the copy constructor
TEST_F(ArrayTest, CopyConstructor) {
  array<int, 5> copy_arr = arr;
  EXPECT_EQ(copy_arr.size(), 5);
  EXPECT_EQ(copy_arr[0], 10);  // All elements should be 10 after copying
  EXPECT_EQ(copy_arr[4], 10);
}

// Test the move constructor
TEST_F(ArrayTest, MoveConstructor) {
  array<int, 5> moved_arr = std::move(arr);
  EXPECT_EQ(moved_arr.size(), 5);
  EXPECT_EQ(moved_arr[0], 10);  // Should have the same values after move
  EXPECT_EQ(moved_arr[4], 10);
  // After moving, the original array should be empty (since it's moved)
  EXPECT_EQ(arr.size(), 5);  // Moved but not modified in this case, might be
                             // empty if logic was written differently
}

// Test assignment operator (copy)
TEST_F(ArrayTest, CopyAssignmentOperator) {
  array<int, 5> copy_arr;
  copy_arr = arr;
  EXPECT_EQ(copy_arr.size(), 5);
  EXPECT_EQ(copy_arr[0], 10);
  EXPECT_EQ(copy_arr[4], 10);
}

// Test assignment operator (move)
TEST_F(ArrayTest, MoveAssignmentOperator) {
  array<int, 5> moved_arr;
  moved_arr = std::move(arr);
  EXPECT_EQ(moved_arr.size(), 5);
  EXPECT_EQ(moved_arr[0], 10);
  EXPECT_EQ(moved_arr[4], 10);
  // After move, the original array should not hold values
  EXPECT_EQ(arr.size(), 5);
}

// Test the `at()` method for valid index
TEST_F(ArrayTest, AtValidIndex) {
  EXPECT_EQ(arr.at(0), 10);
  EXPECT_EQ(arr.at(4), 10);
}

// Test the `at()` method for invalid index
TEST_F(ArrayTest, AtInvalidIndex) {
  EXPECT_THROW(arr.at(5), std::out_of_range);  // Index out of range
}

// Test the `operator[]` for valid index
TEST_F(ArrayTest, OperatorBracket) {
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[4], 10);
}

// Test front() method
TEST_F(ArrayTest, Front) { EXPECT_EQ(arr.front(), 10); }

// Test back() method
TEST_F(ArrayTest, Back) { EXPECT_EQ(arr.back(), 10); }

// Test filling the array with a specific value
TEST_F(ArrayTest, Fill) {
  arr.fill(20);
  EXPECT_EQ(arr[0], 20);
  EXPECT_EQ(arr[4], 20);
}

// Test swap method
TEST_F(ArrayTest, Swap) {
  array<int, 5> arr2 = {5, 4, 3, 2, 1};
  arr.swap(arr2);
  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr2[0], 10);
}

// Test iterators (begin and end)
TEST_F(ArrayTest, Iterators) {
  auto it = arr.begin();
  EXPECT_EQ(*it, 10);
  it++;
  EXPECT_EQ(*it, 10);
}

// Test empty array (edge case for size 0 array)
TEST_F(ArrayTest, EmptyArray) {
  array<int, 0> empty_arr;
  EXPECT_TRUE(empty_arr.empty());
}

}  // namespace s21