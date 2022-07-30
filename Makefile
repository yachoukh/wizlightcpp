
VIP_NAME		= x86_64
CURR_DIR		= $(shell pwd)
COMPILER_PATH	= /usr/bin/

export QUIET = 1
ifeq ($(QUIET), 1)
        HUSH=@
endif

## Build tools
CC				= $(COMPILER_PATH)gcc
CXX 			= $(COMPILER_PATH)g++
AR 				= $(COMPILER_PATH)ar
AS 				= $(COMPILER_PATH)as
RANLIB 			= $(COMPILER_PATH)ranlib
LD 				= $(COMPILER_PATH)ld
STRIP 			= $(COMPILER_PATH)strip

LIBDIR			= /usr/local/lib/
INCLUDEDIR		= /usr/local/include

LDFLAGS			= -L$/usr/lib
LDFLAGS			+= -L$(LIBDIR)
LDFLAGS			+= -L$(LIBDIR)/jansson
LDFLAGS			+= -ljansson -lpthread

CPPFLAGS 		+= -g
CPPFLAGS		+= -std=c++17 -Wall -Werror
CPPFLAGS		+= -I$(INCLUDEDIR)
CPPFLAGS		+= -I$(CURR_DIR)/include

## Build dirs
bin_name        = wizlightcpp
build_config	= $(VIP_NAME)
build_dir       = $(CURR_DIR)/.build_$(build_config)

## Source
bin_srcs        = $(wildcard src/*.cpp)
bin_target      = $(build_dir)/$(bin_name)
bin_objs        = $(addprefix $(build_dir)/,$(bin_srcs:.cpp=.o))

all_objs		= $(bin_objs)


## Build rules
all: $(build_dir) $(bin_target)
	$(HUSH) echo "DONE!"

clean:
	$(HUSH) rm -rf $(build_dir)

$(build_dir):
	$(HUSH) echo "Create Build Dir"
	$(HUSH) mkdir -p $(build_dir)/src
	
$(build_dir)/%.o: %.cpp
	$(HUSH) echo "[[Compiling]] <<<----- $<"
	$(HUSH) $(CXX) $(CPPFLAGS) -c -o $@ $<

$(bin_target): $(all_objs)
	$(HUSH) echo "Building $(bin_name)"
	$(HUSH) $(CXX) -o $@ $^ $(LDFLAGS)
ifeq ($(BUILD), RELEASE)
	$(HUSH) $(STRIP) $(STRIPFLAGS) $(bin_target)
endif
	$(HUSH) chmod +x $(bin_target)
