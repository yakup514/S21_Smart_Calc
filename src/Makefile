CFLAGS = -Wall -Werror -Wextra -std=c11
ARCH = $(shell arch)
#$(info ARCH  is [$(ARCH)])
ifeq "$(ARCH)" "i386"
MYFLAGS = -lcheck -lpthread -lm  -I /usr/local/include -L/usr/local/lib
endif
ifeq "$(ARCH)" "x86_64"
MYFLAGS = -lcheck -lm -lpthread -lsubunit -g
endif


all: clean test gcov_report install


install:
	mkdir build
	cd build && qmake ../view/calculator && make
	rm -rf build
	
dvi:
	open README.md

dist: install
	tar -czf calc.tar.gz ~/Desktop/Calc/calculator.app
	rm -rf ~/Desktop/Calc

uninstall:
	rm -rf ~/Desktop/Calc
	


vpath %.c model:model/test/ test

test: stack.c string_utils.c calc_core.c test.c
	gcc $^ -o $@ $(CFLAGS) $(MYFLAGS) --coverage
	
gcov_report: test
	./test
	gcovr --html-details rep.html
	open rep.html

vpath %.c model:model/test/ git
vpath %.h model:model/test/:view/calculator/ git
vpath %.cpp model:model/test/:view/calculator/ git
vpath %.pro model:model/test/:view/calculator/ git

git:
	git add *.c
	git add *.h
	git add *.cpp
	git add *.pro
	git add Makefile
	git commit -m "add from make"
	git push

vpath %.o model:model/test/:view/calculator/ clean

clean:
	rm -rf ./test
	rm -rf *.o *.gcda *.a *.html *.gcno *.css
	rm -rf *.tar.gz
	
style:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i model/*.c
	clang-format -i model/test/*.c
	clang-format -i model/*.h
	clang-format -i view/calculator/*.cpp
	clang-format -i view/calculator/*.h

