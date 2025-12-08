# Source-level build rules for four2one

CXX ?= g++
AR  ?= ar

NAME = four2one
BUNDLE = kaoss-four2one.lv2
PLUGIN = kaoss-four2one.so

# Main plugin source(s)
SRC = four2one.cpp
OBJ = $(SRC:.cpp=.o)

CXXFLAGS += -Wall -Wextra -fvisibility=hidden -fPIC -std=c++11 -O3
LDFLAGS  += -shared

LIBS =

# Installation path
INSTALL_PATH ?= /usr/local/lib/lv2
COMPLETE_INSTALL_PATH = $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)

all: $(PLUGIN)

$(PLUGIN): $(OBJ)
    $(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c -o $@ $<

install: $(PLUGIN)
    mkdir -p $(COMPLETE_INSTALL_PATH)
    cp $(PLUGIN) $(COMPLETE_INSTALL_PATH)/
    cp -r $(BUNDLE)/* $(COMPLETE_INSTALL_PATH)/

clean:
    -rm -f $(OBJ) $(PLUGIN)

.PHONY: all clean install