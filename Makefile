OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := ./LeapSDK/lib/x64/libLeap.so -Wl,-rpath,./LeapSDK/lib/x64
  else ifeq ($(ARCH), armv7l)
    LEAP_LIBRARY :=
  else
    LEAP_LIBRARY := ./LeapSDK/lib/x86/libLeap.so -Wl,-rpath,./LeapSDK/lib/x86
  endif
else ifeq ($(OS), Darwin)
  # OS X
  LEAP_LIBRARY := ./LeapSDK/lib/libLeap.dylib
endif

ECHO 		=	printf
CAT 		=	cat
RM      =	rm
FIND 		=	find
CPPFLAGS=	-std=c++11 -ggdb -Wall -W -Wextra
LDFLAGS	=
CC   		= g++
LD   		= g++

SRCD = src
INCD = -Iinclude -I./LeapSDK/include
OBJD = obj
BIND = bin
LIBD =
LIBS =

EXEC 	=  XboxServer
SRCS	=  $(wildcard $(SRCD)/*.cpp)
OBJ		=  $(SRCS:.cpp=.o)
OBJS 	:= $(addprefix $(OBJD)/, $(notdir $(OBJ)))

default: $(BIND) $(OBJD) $(BIND)/$(EXEC)

$(BIND)/$(EXEC): $(OBJS)
	@$(ECHO) "Linking ..\n"
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBD) $(LIBS) $(LEAP_LIBRARY)

ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib LeapSDK/lib/libLeap.dylib $(BIND)/$(EXEC)
endif

$(OBJD)/%.o: $(SRCD)/%.cpp
	@$(ECHO) "Compiling source file ..\n"
	$(CC) -o $@ -c $< $(CPPFLAGS) $(INCD)

$(OBJD):
	@mkdir -p $(OBJD)
$(BIND):
	@mkdir -p $(BIND)

clean:
	@$(FIND) $(OBJD) -name *.o -delete

mrproper: clean
	@$(RM) -f $(BIND)/$(EXEC)
	@$(RM) -rf $(OBJD) $(BIND)
