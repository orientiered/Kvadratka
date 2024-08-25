#Almost universal makefile
#This version is made for Windows
#To compile on linux uncomment rm and mkdir, delete 'del' and long IF with mkdir

ifeq ($(origin CC),default)
	CC=g++
endif

#Name of compiled executable
NAME=main.exe
#Name of directory where .o and .d files will be stored
OBJDIR = obj
#Name of directory with headers
INCLUDEDIR = include
#Name of directory with .cpp
SRCDIR = source

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
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

#Automatic target to compile object files
$(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@IF exist "$(OBJDIR)/" ( echo "" ) ELSE ( mkdir "$(OBJDIR)/" )
#	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Idk how it works, but is uses compiler preprocessor to automatically generate
#.d files with included headears that make can use
$(DEPS) : $(OBJDIR)/%.d : $(SRCDIR)/%.cpp
	@IF exist "$(OBJDIR)/" ( echo "" ) ELSE ( mkdir "$(OBJDIR)/" )
#	mkdir -p $(OBJDIR)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


.PHONY:init
init:
	IF exist "$(OBJDIR)/" ( echo "" ) ELSE ( mkdir "$(OBJDIR)/" )
#	mkdir -p $(OBJDIR)

#Deletes all object and .d files
.PHONY:clean
clean:
#	rm ./$(OBJDIR)/*.o
	del .\$(OBJDIR)\*.o .\$(OBJDIR)\*.d

#This 'if' should deactivate generation of .d files on clean target but it doesn't work
#So I commented it out
NODEPS = clean

#Includes make dependencies
#ifeq (0, $(words, $(findstring $(MAKECMDGOALS),$(NODEPS))))
include $(DEPS)
#endif

