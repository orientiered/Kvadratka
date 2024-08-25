ifeq ($(origin CC),default)
	CC=g++
endif

NAME=main.exe
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = source
SRCS := $(wildcard $(SRCDIR)/*.cpp)
TOBJS := $(SRCS:%.cpp=%.o)
OBJS := $(TOBJS:$(SRCDIR)%=$(OBJDIR)%)

override CFLAGS +=	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code									\
		-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe						\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers                \
		-Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo						\
		-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

override CFLAGS += -I./$(INCLUDEDIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | init
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:init
init:
	mkdir -p $(OBJDIR)

.PHONY:clean
clean:
	rm ./$(OBJDIR)/*.o
#	del .\$(OBJDIR)\*.o


