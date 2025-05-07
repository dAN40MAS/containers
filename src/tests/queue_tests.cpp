#include <gtest/gtest.h>

#include "../s21_containers.h"

namespace s21 {

class QueueTest : public ::testing::Test {
 protected:
  Queue<int> queue;

  void SetUp() override {}

  void TearDown() override {}
};

// Тест на создание очереди
TEST_F(QueueTest, DefaultConstructor) {
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

// Тест на копирующий конструктор
TEST_F(QueueTest, CopyConstructor) {
  queue.push(1);
  queue.push(2);
  Queue<int> copy(queue);
  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.front(), 1);
  EXPECT_EQ(copy.back(), 2);
}

// Тест на перемещающий конструктор
TEST_F(QueueTest, MoveConstructor) {
  queue.push(1);
  queue.push(2);
  Queue<int> moved_queue(std::move(queue));
  EXPECT_EQ(moved_queue.size(), 2);
  EXPECT_EQ(moved_queue.front(), 1);
  EXPECT_EQ(moved_queue.back(), 2);
  EXPECT_TRUE(queue.empty());  // Исходная очередь должна быть пустой
}

// Тест на копирующее присваивание
TEST_F(QueueTest, CopyAssignmentOperator) {
  queue.push(1);
  queue.push(2);
  Queue<int> another_queue;
  another_queue = queue;
  EXPECT_EQ(another_queue.size(), 2);
  EXPECT_EQ(another_queue.front(), 1);
  EXPECT_EQ(another_queue.back(), 2);
}

// Тест на перемещающее присваивание
TEST_F(QueueTest, MoveAssignmentOperator) {
  queue.push(1);
  queue.push(2);
  Queue<int> another_queue;
  another_queue = std::move(queue);
  EXPECT_EQ(another_queue.size(), 2);
  EXPECT_EQ(another_queue.front(), 1);
  EXPECT_EQ(another_queue.back(), 2);
  EXPECT_TRUE(queue.empty());  // Исходная очередь должна быть пустой
}

// Тест на добавление элемента
TEST_F(QueueTest, Push) {
  queue.push(1);
  EXPECT_FALSE(queue.empty());
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.front(), 1);
  queue.push(2);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.back(), 2);
}

// Тест на извлечение элемента
TEST_F(QueueTest, Pop) {
  queue.push(1);
  queue.push(2);
  queue.pop();
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.front(), 2);
}

// Тест на исключение при попытке извлечь из пустой очереди
TEST_F(QueueTest, PopEmptyQueue) {
  EXPECT_THROW(queue.pop(), std::out_of_range);
}

// Тест на получение элемента в начале очереди
TEST_F(QueueTest, Front) {
  queue.push(1);
  queue.push(2);
  EXPECT_EQ(queue.front(), 1);
}

// Тест на получение элемента в начале очереди для пустой очереди
TEST_F(QueueTest, FrontEmptyQueue) {
  EXPECT_THROW(queue.front(), std::out_of_range);
}

// Тест на получение элемента в конце очереди
TEST_F(QueueTest, Back) {
  queue.push(1);
  queue.push(2);
  EXPECT_EQ(queue.back(), 2);
}

// Тест на получение элемента в конце очереди для пустой очереди
TEST_F(QueueTest, BackEmptyQueue) {
  EXPECT_THROW(queue.back(), std::out_of_range);
}

// Тест на очистку очереди
TEST_F(QueueTest, Clear) {
  queue.push(1);
  queue.push(2);
  queue.clear();
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

// Тест на размер очереди после добавления и удаления элементов
TEST_F(QueueTest, SizeAfterOperations) {
  EXPECT_EQ(queue.size(), 0);
  queue.push(1);
  EXPECT_EQ(queue.size(), 1);
  queue.push(2);
  EXPECT_EQ(queue.size(), 2);
  queue.pop();
  EXPECT_EQ(queue.size(), 1);
  queue.pop();
  EXPECT_EQ(queue.size(), 0);
}

// Тест на обмен очередями
TEST_F(QueueTest, Swap) {
  queue.push(1);
  queue.push(2);
  Queue<int> another_queue;
  another_queue.push(3);
  another_queue.push(4);
  queue.swap(another_queue);
  EXPECT_EQ(queue.front(), 3);
  EXPECT_EQ(another_queue.front(), 1);
}

// Тест на изменение размера очереди
TEST_F(QueueTest, Resize) {
  for (int i = 0; i < 10; ++i) {
    queue.push(i);
  }
  EXPECT_EQ(queue.size(), 10);
  EXPECT_EQ(queue.front(), 0);
  queue.pop();
  EXPECT_EQ(queue.front(), 1);
}

// Тест на добавление нескольких элементов
TEST_F(QueueTest, InsertManyBack) {
  queue.insert_many_back(1, 2, 3, 4, 5);  // Добавляем несколько элементов
  EXPECT_EQ(queue.size(), 5);
  EXPECT_EQ(queue.front(), 1);
  EXPECT_EQ(queue.back(), 5);
}

}  // namespace s21
