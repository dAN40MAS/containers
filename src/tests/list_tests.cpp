#include <gtest/gtest.h>

#include "../list/s21_list.h"

// 1. DefaultConstructor: Проверяет, что после создания списка он пуст и его
// размер равен 0.
TEST(ListTest, DefaultConstructor) {
  s21::list<int> my_list;
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 2. PushBack: Тестирует добавление элементов в конец списка и проверяет
// корректность размера и последнего элемента.
TEST(ListTest, PushBack) {
  s21::list<int> my_list;
  my_list.push_back(10);
  my_list.push_back(20);
  my_list.push_back(30);
  EXPECT_EQ(my_list.size(), 3);
  EXPECT_EQ(my_list.back(), 30);
}

// 3. PushFront: Тестирует добавление элементов в начало списка и проверяет
// корректность размера и первого элемента.
TEST(ListTest, PushFront) {
  s21::list<int> my_list;
  my_list.push_front(10);
  my_list.push_front(20);
  my_list.push_front(30);
  EXPECT_EQ(my_list.size(), 3);
  EXPECT_EQ(my_list.front(), 30);
}

// 4. PopBack: Тестирует удаление элемента с конца списка и проверяет
// обновленный размер и последний элемент.
TEST(ListTest, PopBack) {
  s21::list<int> my_list;
  my_list.push_back(10);
  my_list.push_back(20);
  my_list.pop_back();
  EXPECT_EQ(my_list.size(), 1);
  EXPECT_EQ(my_list.back(), 10);
}

// 5. PopFront: Тестирует удаление элемента с начала списка и проверяет
// обновленный размер и первый элемент.
TEST(ListTest, PopFront) {
  s21::list<int> my_list;
  my_list.push_front(10);
  my_list.push_front(20);
  my_list.pop_front();
  EXPECT_EQ(my_list.size(), 1);
  EXPECT_EQ(my_list.front(), 10);
}

// 6. Size: Проверяет корректность изменения размера списка при добавлении
// элементов.
TEST(ListTest, Size) {
  s21::list<int> my_list;
  EXPECT_EQ(my_list.size(), 0);
  my_list.push_back(1);
  EXPECT_EQ(my_list.size(), 1);
  my_list.push_back(2);
  EXPECT_EQ(my_list.size(), 2);
}

// 7. Empty: Проверяет, правильно ли работает метод empty() при пустом и
// непустом списке.
TEST(ListTest, Empty) {
  s21::list<int> my_list;
  EXPECT_TRUE(my_list.empty());
  my_list.push_back(1);
  EXPECT_FALSE(my_list.empty());
}

// 8. Clear: Тестирует очистку списка и проверяет, что размер становится 0 и
// список пуст.
TEST(ListTest, Clear) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  my_list.clear();
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 9. IteratorIncrement: Проверяет корректность операции инкремента итератора.
TEST(ListTest, IteratorIncrement) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  auto it = my_list.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
}

// 10. IteratorDecrement: Проверяет корректность операции декремента итератора.
TEST(ListTest, IteratorDecrement) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  auto it = my_list.end();  // it.current_ == nullptr
  --it;  // current_ устанавливается на tail_ (элемент со значением 2)
  EXPECT_EQ(*it, 2);
  --it;  // current_ переходит на предыдущий узел (элемент со значением 1)
  EXPECT_EQ(*it, 1);
}

// 11. CopyConstructor: Тестирует копирующий конструктор и проверяет, что новый
// список содержит те же элементы.
TEST(ListTest, CopyConstructor) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  s21::list<int> copied_list(my_list);
  EXPECT_EQ(copied_list.size(), 2);
  EXPECT_EQ(copied_list.front(), 1);
  EXPECT_EQ(copied_list.back(), 2);
}

// 12. MoveConstructor: Тестирует перемещающий конструктор и проверяет перенос
// данных без копирования.
TEST(ListTest, MoveConstructor) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  s21::list<int> moved_list(std::move(my_list));
  EXPECT_EQ(moved_list.size(), 2);
  EXPECT_EQ(moved_list.front(), 1);
  EXPECT_EQ(moved_list.back(), 2);
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 13. AssignmentOperator: Тестирует оператор присваивания копированием.
TEST(ListTest, AssignmentOperator) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  s21::list<int> assigned_list;
  assigned_list = my_list;
  EXPECT_EQ(assigned_list.size(), 2);
  EXPECT_EQ(assigned_list.front(), 1);
  EXPECT_EQ(assigned_list.back(), 2);
}

// 14. MoveAssignmentOperator: Тестирует оператор присваивания перемещением.
TEST(ListTest, MoveAssignmentOperator) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  s21::list<int> assigned_list;
  assigned_list = std::move(my_list);
  EXPECT_EQ(assigned_list.size(), 2);
  EXPECT_EQ(assigned_list.front(), 1);
  EXPECT_EQ(assigned_list.back(), 2);
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 15. Front: Проверяет корректность метода front() при добавлении элементов в
// начало и конец.
TEST(ListTest, Front) {
  s21::list<int> my_list;
  my_list.push_back(10);
  EXPECT_EQ(my_list.front(), 10);
  my_list.push_front(20);
  EXPECT_EQ(my_list.front(), 20);
}

// 16. Back: Проверяет корректность метода back() при добавлении элементов в
// конец.
TEST(ListTest, Back) {
  s21::list<int> my_list;
  my_list.push_back(10);
  EXPECT_EQ(my_list.back(), 10);
  my_list.push_back(20);
  EXPECT_EQ(my_list.back(), 20);
}

// 17. IteratorEquality: Тестирует операцию сравнения итераторов на равенство.
TEST(ListTest, IteratorEquality) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  auto it1 = my_list.begin();
  auto it2 = my_list.begin();
  EXPECT_TRUE(it1 == it2);
  ++it1;
  EXPECT_FALSE(it1 == it2);
}

// 18. IteratorInequality: Тестирует операцию сравнения итераторов на
// неравенство.
TEST(ListTest, IteratorInequality) {
  s21::list<int> my_list;
  my_list.push_back(1);
  my_list.push_back(2);
  auto it1 = my_list.begin();
  auto it2 = my_list.begin();
  EXPECT_FALSE(it1 != it2);
  ++it1;
  EXPECT_TRUE(it1 != it2);
}

// 19. PopBackEmptyList: Краевой случай — удаление из пустого списка с конца.
// Проверяет, что исключение не выбрасывается и размер остается 0.
TEST(ListTest, PopBackEmptyList) {
  s21::list<int> my_list;
  EXPECT_NO_THROW(my_list.pop_back());
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 20. PopFrontEmptyList: Краевой случай — удаление из пустого списка с начала.
// Проверяет, что исключение не выбрасывается и размер остается 0.
TEST(ListTest, PopFrontEmptyList) {
  s21::list<int> my_list;
  EXPECT_NO_THROW(my_list.pop_front());
  EXPECT_EQ(my_list.size(), 0);
  EXPECT_TRUE(my_list.empty());
}

// 21. FrontBackEmptyList: Краевой случай — доступ к первому и последнему
// элементу в пустом списке. Ожидается выброс исключения std::runtime_error.
TEST(ListTest, FrontBackEmptyList) {
  s21::list<int> my_list;
  EXPECT_THROW(my_list.front(), std::runtime_error);
  EXPECT_THROW(my_list.back(), std::runtime_error);
}
