CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -g 
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Директории
TEST_DIR = tests
BUILD_DIR = build
TEST_BIN = $(BUILD_DIR)/tests/bin/tests  # Исполняемый файл
GCOV_REPORT_DIR = gcov_report

# Заголовочные файлы
VECTOR_HDR = ./vector/s21_vector.h
QUEUE_HDR = ./queue/s21_queue.h
STACK_HDR = ./stack/s21_stack.h
ARRAY_HDR = ./array/s21_array.h
LIST_HDR = ./list/s21_list.h
SET_HDR = ./set/s21_set.h
MULTISET_HDR = ./multiset/s21_multiset.h

# Исходные файлы тестов
TEST_SRC = $(TEST_DIR)/main_test.cpp    \
           $(TEST_DIR)/queue_tests.cpp  \
           $(TEST_DIR)/stack_tests.cpp  \
           $(TEST_DIR)/vector_tests.cpp \
           $(TEST_DIR)/array_tests.cpp  \
           $(TEST_DIR)/list_tests.cpp   \
           $(TEST_DIR)/map_tests.cpp    \
           $(TEST_DIR)/set_tests.cpp    \
           $(TEST_DIR)/multiset_tests.cpp

# Объектные файлы
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/$(TEST_DIR)/%.o, $(TEST_SRC))
TEST_OBJ_COV = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/$(TEST_DIR)/%.gcov.o, $(TEST_SRC))

.PHONY: all clean test gcov_report rebuild

# Основная цель
all: test

# Сборка объектных файлов
$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp $(VECTOR_HDR) $(QUEUE_HDR) $(STACK_HDR) $(ARRAY_HDR) $(LIST_HDR) $(SET_HDR) $(MULTISET_HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка объектных файлов с покрытием
$(BUILD_DIR)/$(TEST_DIR)/%.gcov.o: $(TEST_DIR)/%.cpp $(VECTOR_HDR) $(QUEUE_HDR) $(STACK_HDR) $(ARRAY_HDR) $(LIST_HDR) $(SET_HDR) $(MULTISET_HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

# Линковка тестов
$(TEST_BIN): $(TEST_OBJ_COV)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_OBJ_COV) -o $(TEST_BIN) $(GTEST_FLAGS)

# Запуск тестов
test: $(TEST_BIN)
	./$(TEST_BIN)

# Генерация отчета покрытия
gcov_report: $(TEST_BIN)
	./$(TEST_BIN)
	@mkdir -p $(GCOV_REPORT_DIR)
	lcov --capture --directory . --output-file $(GCOV_REPORT_DIR)/coverage.info --include '*/src/*' --ignore-errors mismatch
	genhtml -o $(GCOV_REPORT_DIR) $(GCOV_REPORT_DIR)/coverage.info
	open $(GCOV_REPORT_DIR)/index.html

# Очистка
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(GCOV_REPORT_DIR)
	rm -f *.gcda *.gcno *.gcov

# Пересборка
rebuild: clean all

# Форматирование кода
clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(TEST_DIR)/*.cpp $(VECTOR_HDR) $(QUEUE_HDR) $(STACK_HDR) $(ARRAY_HDR) $(LIST_HDR) $(SET_HDR) $(MULTISET_HDR)

# Проверка форматирования
clang_check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(TEST_DIR)/*.cpp $(VECTOR_HDR) $(QUEUE_HDR) $(STACK_HDR) $(ARRAY_HDR) $(LIST_HDR) $(SET_HDR) $(MULTISET_HDR)