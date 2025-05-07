#include <gtest/gtest.h>

#include "../s21_containersplus.h"

namespace s21 {

class MultisetTest : public ::testing::Test {
 protected:
  Multiset<int> ms;  // Создаем мультимножество для тестов

  void SetUp() override {
    // Инициализация мультимножества некоторыми значениями
    ms.insert(10);
    ms.insert(20);
    ms.insert(20);  // Дубликат
    ms.insert(30);
  }
};

// Тест конструктора по умолчанию
TEST_F(MultisetTest, DefaultConstructor) {
  Multiset<int> default_ms;
  EXPECT_TRUE(
      default_ms.empty());  // По умолчанию мультимножество должно быть пустым
  EXPECT_EQ(default_ms.size(), 0);
}

// Тест конструктора с использованием списка инициализации
TEST_F(MultisetTest, InitializerListConstructor) {
  Multiset<int> init_ms = {1, 2, 2, 3, 3, 3};
  EXPECT_EQ(init_ms.size(), 6);  // Проверяем количество элементов
  EXPECT_EQ(init_ms.count(2), 2);  // Проверяем количество дубликатов
  EXPECT_EQ(init_ms.count(3), 3);
}

// Тест копирующего конструктора
TEST_F(MultisetTest, CopyConstructor) {
  Multiset<int> copy_ms = ms;
  EXPECT_EQ(copy_ms.size(), 4);
  EXPECT_EQ(copy_ms.count(20), 2);  // Проверяем, что дубликаты скопированы
}

// Тест перемещающего конструктора
TEST_F(MultisetTest, MoveConstructor) {
  Multiset<int> moved_ms = std::move(ms);
  EXPECT_EQ(moved_ms.size(), 4);
  EXPECT_EQ(moved_ms.count(20), 2);  // Проверяем, что дубликаты перемещены
  EXPECT_TRUE(ms.empty());  // После перемещения исходное мультимножество должно
                            // быть пустым
}

// Тест оператора присваивания (копирование)
TEST_F(MultisetTest, CopyAssignmentOperator) {
  Multiset<int> copy_ms;
  copy_ms = ms;
  EXPECT_EQ(copy_ms.size(), 4);
  EXPECT_EQ(copy_ms.count(20), 2);  // Проверяем, что дубликаты скопированы
}

// Тест оператора присваивания (перемещение)
TEST_F(MultisetTest, MoveAssignmentOperator) {
  Multiset<int> moved_ms;
  moved_ms = std::move(ms);
  EXPECT_EQ(moved_ms.size(), 4);
  EXPECT_EQ(moved_ms.count(20), 2);  // Проверяем, что дубликаты перемещены
  EXPECT_TRUE(ms.empty());  // После перемещения исходное мультимножество должно
                            // быть пустым
}

// Тест вставки элементов (дубликаты разрешены)
TEST_F(MultisetTest, Insert) {
  ms.insert(40);
  EXPECT_EQ(ms.size(), 5);  // Проверяем, что элемент добавлен
  EXPECT_TRUE(ms.contains(40));

  ms.insert(20);  // Вставляем дубликат
  EXPECT_EQ(ms.size(), 6);
  EXPECT_EQ(ms.count(20), 3);  // Проверяем, что дубликат добавлен
}

// Тест удаления элементов
TEST_F(MultisetTest, Erase) {
  ms.erase(ms.find(20));  // Удаляем один из дубликатов
  EXPECT_EQ(ms.count(20), 1);  // Проверяем, что один дубликат остался
  EXPECT_EQ(ms.size(), 3);
}

// Тест поиска элементов
TEST_F(MultisetTest, Find) {
  auto it = ms.find(20);
  EXPECT_NE(it, ms.end());  // Проверяем, что элемент найден
  EXPECT_EQ(*it, 20);

  it = ms.find(100);
  EXPECT_EQ(it, ms.end());  // Элемент не должен быть найден
}

// Тест метода `contains`
TEST_F(MultisetTest, Contains) {
  EXPECT_TRUE(ms.contains(10));
  EXPECT_TRUE(ms.contains(20));
  EXPECT_FALSE(ms.contains(100));  // Элемент не должен быть в мультимножестве
}

// Тест очистки мультимножества
TEST_F(MultisetTest, Clear) {
  ms.clear();
  EXPECT_TRUE(ms.empty());  // Мультимножество должно быть пустым
  EXPECT_EQ(ms.size(), 0);
}

// Тест метода `swap`
TEST_F(MultisetTest, Swap) {
  Multiset<int> other_ms = {40, 50, 50, 60};
  ms.swap(other_ms);
  EXPECT_EQ(ms.size(), 4);  // Проверяем, что мультимножества обменялись
  EXPECT_EQ(other_ms.size(), 4);
  EXPECT_EQ(ms.count(50), 2);  // Проверяем дубликаты
  EXPECT_EQ(other_ms.count(20), 2);
}

// Тест итераторов
TEST_F(MultisetTest, Iterators) {
  auto it = ms.begin();
  EXPECT_EQ(*it, 10);  // Проверяем, что итератор указывает на первый элемент
  ++it;
  EXPECT_EQ(*it, 20);  // Переходим к следующему элементу
}

// Тест пустого мультимножества
TEST_F(MultisetTest, EmptyMultiset) {
  Multiset<int> empty_ms;
  EXPECT_TRUE(empty_ms.empty());  // Мультимножество должно быть пустым
  EXPECT_EQ(empty_ms.size(), 0);
}

// Тест метода `count`
TEST_F(MultisetTest, Count) {
  EXPECT_EQ(ms.count(10), 1);  // Проверяем количество элементов
  EXPECT_EQ(ms.count(20), 2);  // Проверяем дубликаты
  EXPECT_EQ(ms.count(100), 0);  // Элемент не должен быть в мультимножестве
}

// Тест метода `equal_range`
TEST_F(MultisetTest, EqualRange) {
  auto range = ms.equal_range(20);
  EXPECT_EQ(std::distance(range.first, range.second),
            2);  // Проверяем диапазон дубликатов
}

// Тест метода `lower_bound`
TEST_F(MultisetTest, LowerBound) {
  auto it = ms.lower_bound(20);
  EXPECT_EQ(*it,
            20);  // Проверяем, что итератор указывает на первый элемент >= 20
}

// Тест метода `upper_bound`
TEST_F(MultisetTest, UpperBound) {
  auto it = ms.upper_bound(20);
  EXPECT_EQ(*it,
            30);  // Проверяем, что итератор указывает на первый элемент > 20
}

}  // namespace s21