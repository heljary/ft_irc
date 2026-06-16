NAME        = ircserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

SRCS        = main.cpp \
              CommandHandler.cpp \
              Client.cpp \
              Server.cpp \
              Ping.cpp \
              Pass.cpp \
              Nick.cpp \
              User.cpp \
              Quit.cpp \
              Privmsg.cpp \
              Channel.cpp \
              Join.cpp \
              Part.cpp \
              Kick.cpp \
              Invite.cpp \
              Topic.cpp \
              Notice.cpp \
              Mode.cpp \

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