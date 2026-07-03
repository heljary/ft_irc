NAME        = ircserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

SRCS        = Nick.cpp \
              Pass.cpp \
              User.cpp \
              Privmsg.cpp \
              Notice.cpp \
              Join.cpp \
              Part.cpp \
              Kick.cpp \
              Invite.cpp \
              Topic.cpp \
              Mode.cpp \
              Ping.cpp \
              Quit.cpp

OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re