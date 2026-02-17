CC = gcc
CXX = g++

CFLAGS = -Wall -Wextra -Werror -Werror -std=c11 -Iinclude
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -fPIC -Iinclude
GTEST_FLAGS = -lgtest -lgtest_main -lpthread
QT_CFLAGS = $(shell pkg-config --cflags Qt5Widgets 2>/dev/null || echo "")
QT_LIBS = $(shell pkg-config --libs Qt5Widgets 2>/dev/null || echo "")
GCOV=-lgcov --coverage
GTEST=-lgtest

BUILD_DIR = ./build
LIB_TETRIS_DIR = ./brick_game/tetris
LIB_SNAKE_DIR = ./brick_game/snake
DESKTOP_GUI_DIR = ./gui/desktop
CLI_GUI_DIR = ./gui/cli
CONTROLLER_DIR = ./controller
REPORT_DIR = ./report
TEST_DIR = ./tests

# Snake Desktop
TARGET_SNAKE_QT = snake_game
SOURCES_SNAKE_QT = $(DESKTOP_GUI_DIR)/main.cpp \
                   $(DESKTOP_GUI_DIR)/snake_view.cpp \
                   $(DESKTOP_GUI_DIR)/name_dialog.cpp \
                   $(DESKTOP_GUI_DIR)/message_dialog.cpp \
                   $(CONTROLLER_DIR)/snake_controller.cpp
INCLUDES_SNAKE_QT = -I./gui/desktop -I./controller -I./brick_game -I./brick_game/snake

# Tetris Desktop
TARGET_TETRIS_QT = tetris_game
SOURCES_TETRIS_QT = $(DESKTOP_GUI_DIR)/main_tetris.cpp \
                    $(DESKTOP_GUI_DIR)/snake_view.cpp \
                    $(DESKTOP_GUI_DIR)/name_dialog.cpp \
                    $(DESKTOP_GUI_DIR)/message_dialog.cpp \
                    $(CONTROLLER_DIR)/tetris_controller.cpp
INCLUDES_TETRIS_QT = -I./gui/desktop -I./controller -I./brick_game -I./brick_game/tetris

# Snake Cli
TARGET_SNAKE_CLI = snake_game_cli
SOURCES_SNAKE_CLI = $(CLI_GUI_DIR)/main_cli.c \
                    $(CLI_GUI_DIR)/snake_controller_c.c \
                    $(CLI_GUI_DIR)/frontend.c \
                    $(LIB_SNAKE_DIR)/snake_model.cpp \
                    $(LIB_SNAKE_DIR)/snake_model_c.cpp
OBJECTS_SNAKE_CLI = $(CLI_GUI_DIR)/main_cli.o \
                    $(CLI_GUI_DIR)/snake_controller_c.o \
                    $(CLI_GUI_DIR)/frontend.o \
                    $(LIB_SNAKE_DIR)/snake_model.o \
                    $(LIB_SNAKE_DIR)/snake_model_c.o

TARGET_TEST = snake_tests

all: uninstall install
	QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1 $(BUILD_DIR)/$(TARGET_SNAKE_QT)

build_dir:
	mkdir -p $(BUILD_DIR)

# Snake Desktop
$(TARGET_SNAKE_QT): build_dir snake_lib $(SOURCES_SNAKE_QT)
	$(CXX) -g -O0 $(CXXFLAGS) $(QT_CFLAGS) $(INCLUDES_SNAKE_QT) $(SOURCES_SNAKE_QT) -o $(BUILD_DIR)/$(TARGET_SNAKE_QT) -L$(BUILD_DIR) -lsnake $(QT_LIBS)
	$(CXX)  $(CXXFLAGS) $(QT_CFLAGS) $(INCLUDES_SNAKE_QT) $(SOURCES_SNAKE_QT) -o $(BUILD_DIR)/$(TARGET_SNAKE_QT) -L$(BUILD_DIR) -lsnake $(QT_LIBS)

# Lib Snake
snake_lib: build_dir
	$(CXX) $(CXXFLAGS) -c $(LIB_SNAKE_DIR)/snake_model.cpp -o snake_model.o
	ar rcs $(BUILD_DIR)/libsnake.a snake_model.o
	rm -f snake_model.o

# Tetris Desktop
$(TARGET_TETRIS_QT): build_dir tetris_lib $(SOURCES_TETRIS_QT)
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) $(INCLUDES_TETRIS_QT) $(SOURCES_TETRIS_QT) -o $(BUILD_DIR)/$(TARGET_TETRIS_QT) -L$(BUILD_DIR) -ltetris $(QT_LIBS)

# Lib Tetris
tetris_lib: build_dir
	$(CC) $(CFLAGS) -c $(LIB_TETRIS_DIR)/fsm.c -o fsm.o
	$(CC) $(CFLAGS) -c $(LIB_TETRIS_DIR)/board.c -o board.o
	$(CC) $(CFLAGS) -c $(LIB_TETRIS_DIR)/figure.c -o figure.o
	ar rcs $(BUILD_DIR)/libtetris.a fsm.o board.o figure.o
	rm -f fsm.o board.o figure.o

# Snake CLI
$(TARGET_SNAKE_CLI): $(OBJECTS_SNAKE_CLI)
	$(CXX) $^ -o $(BUILD_DIR)/$@ $(LDFLAGS) -lncurses
	rm -f $(OBJECTS_SNAKE_CLI)

# Правила компиляции объектных файлов
main_cli.o: $(CLI_GUI_DIR)/main_cli.c
	$(CC) $(CFLAGS) -c $< -o $@

gui/cli/snake_controller_c.o: $(CLI_GUI_DIR)/snake_controller_c.c
	$(CC) $(CFLAGS) -c $< -o $@

gui/cli/frontend.o: $(CLI_GUI_DIR)/frontend.c
	$(CC) $(CFLAGS) -c $< -o $@

brick_game/snake/snake_model.o: $(LIB_SNAKE_DIR)/snake_model.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

brick_game/snake/snake_model_c.o: $(LIB_SNAKE_DIR)/snake_model_c.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: clean
	$(CXX) $(CXXFLAGS) $(LIB_SNAKE_DIR)/snake_model.cpp $(TEST_DIR)/tests.cpp -o $(TARGET_TEST) $(GTEST) $(GCOV)
	./$(TARGET_TEST)

gcov_report: test
	lcov -t "gcov_report" --include 'brick_game/snake/snake_model.cpp' --include 'tests/tests.cpp' -o report.info --ignore-errors mismatch -c -d .
	genhtml -o report report.info
	xdg-open ./report/index.html

valgrind: $(TARGET_SNAKE_QT)
	valgrind --leak-check=full --track-origins=yes --log-file="valgrind.log" $(BUILD_DIR)/$(TARGET_SNAKE_QT)

valgrind_tetris: $(TARGET_TETRIS_QT)
	valgrind --leak-check=full --track-origins=yes --log-file="valgrind.log" $(BUILD_DIR)/$(TARGET_TETRIS_QT)

valgrind_cli: $(TARGET_SNAKE_CLI)
	valgrind --leak-check=full --track-origins=yes --log-file="valgrind.log" $(BUILD_DIR)/$(TARGET_SNAKE_CLI)

valgrind_test: test
	valgrind --leak-check=full --track-origins=yes --log-file="valgrind.log" ./$(TARGET_TEST)

clang-test:
	clang-format -style=Google -n $(LIB_SNAKE_DIR)/*.cpp $(LIB_TETRIS_DIR)/*.c $(CLI_GUI_DIR)/*.c $(CONTROLLER_DIR)/*.cpp $(DESKTOP_GUI_DIR)/*.cpp $(TEST_DIR)/*.cpp
	clang-format -style=Google -n $(LIB_SNAKE_DIR)/*.h $(LIB_TETRIS_DIR)/headers/*.h $(CLI_GUI_DIR)/headers/*.h $(CONTROLLER_DIR)/*.h $(DESKTOP_GUI_DIR)/*.h ./brick_game/*.h

clang:
	clang-format -style=Google -i $(LIB_SNAKE_DIR)/*.cpp $(LIB_TETRIS_DIR)/*.c $(CLI_GUI_DIR)/*.c $(CONTROLLER_DIR)/*.cpp $(DESKTOP_GUI_DIR)/*.cpp $(TEST_DIR)/*.cpp
	clang-format -style=Google -i $(LIB_SNAKE_DIR)/*.h $(LIB_TETRIS_DIR)/headers/*.h $(CLI_GUI_DIR)/headers/*.h $(CONTROLLER_DIR)/*.h $(DESKTOP_GUI_DIR)/*.h ./brick_game/*.h

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(REPORT_DIR)
	rm -f $(TARGET_SNAKE_QT) $(TARGET_TETRIS_QT) $(TARGET_SNAKE_CLI) $(TARGET_TEST)\
	      *.o *.a *.gch *.gcda *.gcno $(LIB_TETRIS_DIR)/*.o $(CONTROLLER_DIR)/*.o tests/*.o \
		  $(DESKTOP_GUI_DIR)/*.o $(LIB_SNAKE_DIR)/*.o $(CLI_GUI_DIR)/*.o report.info valgrind.log

install: $(TARGET_SNAKE_QT) $(TARGET_TETRIS_QT) $(TARGET_SNAKE_CLI)

uninstall: clean

snake: $(TARGET_SNAKE_QT)
	QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1 $(BUILD_DIR)/$(TARGET_SNAKE_QT)

tetris: $(TARGET_TETRIS_QT)
	QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1 $(BUILD_DIR)/$(TARGET_TETRIS_QT)

snake_cli: $(TARGET_SNAKE_CLI)
	$(BUILD_DIR)/$(TARGET_SNAKE_CLI)

.PHONY: all clean snake tetris snake_cli