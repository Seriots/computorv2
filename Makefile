
NAME		:= ComputorV2

SRCS_FOLDER	:= srcs
OBJS_FOLDER := objs
HEADER_FOLDER := includes

MAKE 		:= /bin/make --no-print-directory

CXXFLAGS		:= -Wall -Wextra -Werror
CXX			:= c++

SRCS		:= main.cpp
OBJS		:=  $(addprefix $(OBJS_FOLDER)/,$(SRCS:.cpp=.o))
DEPS		:= $(OBJS:.o=.d)

INCLUDES	:= $(addprefix -I,$(HEADER_FOLDER))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

fclean:
	@$(MAKE) clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re


$(OBJS_FOLDER)/%.o: $(SRCS_FOLDER)/%.cpp
	@mkdir -p $(OBJS_FOLDER)
	$(CXX) $(CXXFLAGS) -MMD $(INCLUDES) -o $@ -c $<

-include $(DEPS)
