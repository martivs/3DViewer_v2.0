GCC = -Wall -Werror -Wextra -std=c++17 -pedantic
GTEST = -lgtest -lgmock -pthread

all: install gcov_report

clean:
	rm tests/*.gcda tests/*.gcno; rm tests/3d_viewer_test; rm report.info; rm -rf report;
		
install: 
	mkdir build; cd build; qmake ../view/viewer.pro; make; rm *.o *.h *.cpp .qmake.stash Makefile;

uninstall:
	@rm -rf build

dist:
	tar -czf 3DViewer_v2.tgz controller model view main.cpp Makefile

test:
	g++ $(GCC) --coverage tests/3d_viewer_test.cpp model/model.cpp model/observable.cpp -o tests/3d_viewer_test $(GTEST)
	./tests/3d_viewer_test

gcov_report:	test
	lcov --no-external -t "report" -o report.info -c -d . --exclude "$(PWD)/tests/*"
	genhtml -o report report.info
