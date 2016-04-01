ECHO 		=	printf
CAT 		=	cat
RM              =	rm
FIND 		=	find
UNAME		=       $(shell uname)
CPPFLAGS	=	-std=c++11 -ggdb -Wall -W -Wextra
LDFLAGS		=
CC   		= 	g++
LD   		= 	g++

SRCD = src
INCD = include
OBJD = obj
BIND = bin
LIBD = -L/usr/lib/Leap/
LIBS = /usr/lib/libLeap.so

EXEC 	=  LeapServer
SRCS	=  $(wildcard $(SRCD)/*.cpp)
OBJ	=  $(SRCS:.cpp=.o)
OBJS 	:= $(addprefix $(OBJD)/, $(notdir $(OBJ)))

default: $(BIND) $(OBJD) $(BIND)/$(EXEC)

$(BIND)/$(EXEC): $(OBJS)
	@$(ECHO) "Linking ..\n"
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBD) $(LIBS) -Wl,-rpath ./

$(OBJD)/%.o: $(SRCD)/%.cpp
	@$(ECHO) "Compiling source file ..\n"
	$(CC) -o $@ -c $< $(CPPFLAGS) -I$(INCD)

$(OBJD):
	@mkdir -p $(OBJD)
$(BIND):
	@mkdir -p $(BIND)

clean:
	@$(FIND) $(OBJD) -name *.o -delete

mrproper: clean
	@$(RM) -f $(BIND)/$(EXEC)
	@$(RM) -rf $(OBJD) $(BIND)
