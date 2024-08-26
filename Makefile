#Almost universal makefile
#This version is made for Windows
#To compile on linux uncomment rm and mkdir, delete 'del' and long IF with mkdir
CMD_DEL_LINUX = rm -rf ./$(OBJDIR)/*.o ./$(OBJDIR)/*.d
CMD_DEL_WIN   = del .\$(OBJDIR)\*.o .\$(OBJDIR)\*.d
CMD_MKDIR_LINUX = @mkdir -p $(OBJDIR)
CMD_MKDIR_WIN = @IF exist "$(OBJDIR)/" ( echo "" ) ELSE ( mkdir "$(OBJDIR)/" )

#WIN for windows, LINUX for linux
SYSTEM = WIN
ifeq ($(SYSTEM),WIN)
	CMD_DEL = $(CMD_DEL_WIN)
	CMD_MKDIR = $(CMD_MKDIR_WIN)
else
	CMD_DEL = $(CMD_DEL_LINUX)
	CMD_MKDIR = $(CMD_MKDIR_LINUX)
endif

#compilier
ifeq ($(origin CC),default)
	CC=g++
endif

#Name of compiled executable
NAME=main.exe
#Name of directory where .o and .d files will be stored
OBJDIR = build
#Name of directory with headers
INCLUDEDIR = include
#Name of directory with .cpp
SRCDIR = source
#Name of directory where doxygen documentation will be generated
DOXYDIR = doxDocs

#Note: ALL cpps in source dir will be compiled
#Getting all cpps
SRCS := $(wildcard $(SRCDIR)/*.cpp)
#Replacing .cpp with .o, temporary variable
TOBJS := $(SRCS:%.cpp=%.o)
#Replacing src dir to obj dir
OBJS := $(TOBJS:$(SRCDIR)%=$(OBJDIR)%)

#Dependencies for .cpp files, they are stored with .o objects
DEPS := $(OBJS:%.o=%.d)

override CFLAGS +=	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code									\
		-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe						\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers                \
		-Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo						\
		-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

#flag to tell compiler where headers are located
override CFLAGS += -I./$(INCLUDEDIR)

#Main target to compile executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@


#Automatic target to compile object files
$(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CMD_MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Idk how it works, but is uses compiler preprocessor to automatically generate
#.d files with included headears that make can use
$(DEPS) : $(OBJDIR)/%.d : $(SRCDIR)/%.cpp
	$(CMD_MKDIR)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY:init
init:
	$(CMD_MKDIR)

#Deletes all object and .d files

.PHONY:clean
clean:
	$(CMD_DEL)


.PHONY:doxygen
doxygen:
ifeq ($(SYSTEM), WIN)
	@IF exist "$(DOXYDIR)/" ( echo "" ) ELSE ( mkdir "$(DOXYDIR)/" )
else
	@mkdir -p $(DOXYDIR)
endif
	doxygen Doxyfile


NODEPS = clean

#Includes make dependencies
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
