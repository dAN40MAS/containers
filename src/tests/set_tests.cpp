#include <gtest/gtest.h>

#include "../s21_containers.h"

namespace s21 {

class SetTest : public ::testing::Test {
 protected:
  Set<int> set;  // Создаем множество для тестов

  void SetUp() override {
    // Инициализация множества некоторыми значениями
    set.insert(10);
    set.insert(20);
    set.insert(30);
  }
};

// Тест конструктора по умолчанию
TEST_F(SetTest, DefaultConstructor) {
  Set<int> default_set;
  EXPECT_TRUE(
      default_set.empty());  // По умолчанию множество должно быть пустым
  EXPECT_EQ(default_set.size(), 0);
}

// Тест конструктора с использованием списка инициализации
TEST_F(SetTest, InitializerListConstructor) {
  Set<int> init_set = {1, 2, 3, 4, 5};
  EXPECT_EQ(init_set.size(), 5);
  EXPECT_TRUE(init_set.contains(1));  // Проверяем наличие элементов
  EXPECT_TRUE(init_set.contains(5));
}

// Тест копирующего конструктора
TEST_F(SetTest, CopyConstructor) {
  Set<int> copy_set = set;
  EXPECT_EQ(copy_set.size(), 3);
  EXPECT_TRUE(copy_set.contains(10));  // Проверяем, что элементы скопированы
  EXPECT_TRUE(copy_set.contains(30));
}

// Тест перемещающего конструктора
TEST_F(SetTest, MoveConstructor) {
  Set<int> moved_set = std::move(set);
  EXPECT_EQ(moved_set.size(), 3);
  EXPECT_TRUE(moved_set.contains(10));  // Проверяем, что элементы перемещены
  EXPECT_TRUE(moved_set.contains(30));
  EXPECT_TRUE(
      set.empty());  // После перемещения исходное множество должно быть пустым
}

// Тест оператора присваивания (копирование)
TEST_F(SetTest, CopyAssignmentOperator) {
  Set<int> copy_set;
  copy_set = set;
  EXPECT_EQ(copy_set.size(), 3);
  EXPECT_TRUE(copy_set.contains(10));
  EXPECT_TRUE(copy_set.contains(30));
}

// Тест оператора присваивания (перемещение)
TEST_F(SetTest, MoveAssignmentOperator) {
  Set<int> moved_set;
  moved_set = std::move(set);
  EXPECT_EQ(moved_set.size(), 3);
  EXPECT_TRUE(moved_set.contains(10));
  EXPECT_TRUE(moved_set.contains(30));
  EXPECT_TRUE(
      set.empty());  // После перемещения исходное множество должно быть пустым
}

// Тест вставки элементов
TEST_F(SetTest, Insert) {
  auto result = set.insert(40);
  EXPECT_TRUE(result.second);  // Проверяем, что вставка прошла успешно
  EXPECT_TRUE(set.contains(40));
  EXPECT_EQ(set.size(), 4);

  // Попытка вставить дубликат
  auto duplicate_result = set.insert(10);
  EXPECT_FALSE(duplicate_result.second);  // Дубликат не должен быть вставлен
  EXPECT_EQ(set.size(), 4);
}

// Тест удаления элементов
TEST_F(SetTest, Erase) {
  set.erase(set.find(20));  // Удаляем элемент
  EXPECT_FALSE(set.contains(20));  // Проверяем, что элемент удален
  EXPECT_EQ(set.size(), 2);
}

// Тест поиска элементов
TEST_F(SetTest, Find) {
  auto it = set.find(10);
  EXPECT_NE(it, set.end());  // Проверяем, что элемент найден
  EXPECT_EQ(*it, 10);

  it = set.find(100);
  EXPECT_EQ(it, set.end());  // Элемент не должен быть найден
}

// Тест метода `contains`
TEST_F(SetTest, Contains) {
  EXPECT_TRUE(set.contains(10));
  EXPECT_TRUE(set.contains(30));
  EXPECT_FALSE(set.contains(100));  // Элемент не должен быть в множестве
}

// Тест очистки множества
TEST_F(SetTest, Clear) {
  set.clear();
  EXPECT_TRUE(set.empty());  // Множество должно быть пустым
  EXPECT_EQ(set.size(), 0);
}

// Тест метода `swap`
TEST_F(SetTest, Swap) {
  Set<int> other_set = {40, 50, 60};
  set.swap(other_set);
  EXPECT_TRUE(set.contains(40));  // Проверяем, что множества обменялись
  EXPECT_TRUE(other_set.contains(10));
  EXPECT_EQ(set.size(), 3);
  EXPECT_EQ(other_set.size(), 3);
}

// Тест итераторов
TEST_F(SetTest, Iterators) {
  auto it = set.begin();
  EXPECT_EQ(*it, 10);  // Проверяем, что итератор указывает на первый элемент
  ++it;
  EXPECT_EQ(*it, 20);  // Переходим к следующему элементу
}

// Тест пустого множества
TEST_F(SetTest, EmptySet) {
  Set<int> empty_set;
  EXPECT_TRUE(empty_set.empty());  // Множество должно быть пустым
  EXPECT_EQ(empty_set.size(), 0);
}

}  // namespace s21