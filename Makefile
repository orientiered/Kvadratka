.PHONY: all clean init

CC=g++
NAME=main
NAME_OBJ=obj

CFLAGS=	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code									\
		-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe						\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers				\
		-Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo						\
		-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

TARGETS =$(NAME_OBJ)/main.o $(NAME_OBJ)/quadraticSolver.o $(NAME_OBJ)/inputHandler.o  $(NAME_OBJ)/quadraticPrinter.o $(NAME_OBJ)/unitTester.o $(NAME_OBJ)/utils.o

all: kvadratka

kvadratka: $(TARGETS)
	$(CC) $(CFLAGS) $(TARGETS) -o $(NAME).exe

$(NAME_OBJ)/main.o: main.cpp
	$(CC) -c main.cpp $(CFLAGS) -o $(NAME_OBJ)/main.o

$(NAME_OBJ)/quadraticSolver.o: quadraticSolver.cpp
	$(CC) -c quadraticSolver.cpp $(CFLAGS) -o $(NAME_OBJ)/quadraticSolver.o

$(NAME_OBJ)/quadraticPrinter.o: quadraticPrinter.cpp
	$(CC) -c quadraticPrinter.cpp $(CFLAGS) -o $(NAME_OBJ)/quadraticPrinter.o

$(NAME_OBJ)/unitTester.o: unitTester.cpp
	$(CC) -c unitTester.cpp $(CFLAGS) -o $(NAME_OBJ)/unitTester.o

$(NAME_OBJ)/inputHandler.o: inputHandler.cpp
	$(CC) -c inputHandler.cpp $(CFLAGS) -o $(NAME_OBJ)/inputHandler.o

$(NAME_OBJ)/utils.o: utils.cpp
	$(CC) -c utils.cpp $(CFLAGS) -o $(NAME_OBJ)/utils.o

init:
	IF exist "$(NAME_OBJ)/" ( echo "$(NAME_OBJ)/ exists" ) ELSE ( mkdir "$(NAME_OBJ)/" )

clean:
	del ".\$(NAME_OBJ)\*.o"
