#Compilers
CC          := g++ -std=c++20 
CFLAGS      := -Wno-psabi -O3
DGEN        := doxygen
SRCEXT      := cc

#The Target Binary Program
TARGET      := libyase.a

#The Directories, Source, Includes, Objects, Binary and Resources
ROOT_DIR    := /Users/ericklavins/Code/yase
SRCDIR      := $(ROOT_DIR)/src
BUILDDIR    := $(ROOT_DIR)/build
TARGETDIR   := $(ROOT_DIR)/lib

EXAMPLE_DIRS := $(wildcard $(ROOT_DIR)/examples/*)

# Directories where include files are
INCDIR      := -I $(ROOT_DIR)/include \
			   -I /opt/homebrew/include \
			   -I /opt/homebrew/include/nlohmann \
			   -I /opt/homebrew/include/rtmidi

# Libraries for executables
LIBDIR      := -L $(ROOT_DIR)/lib -L /opt/homebrew/lib
LIBS        := -lpthread -lyase -lportaudio -lrtmidi
