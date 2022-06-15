#Compilers
CC          := g++ -std=c++20 
CFLAGS      := -Wno-psabi -O3
DGEN        := doxygen
SRCEXT      := cc

#The Target Binary Program
TARGET      := libyase.a

#The Directories, Source, Includes, Objects, Binary and Resources
ROOT_DIR    := /Users/eric/Code/yase
SRCDIR      := $(ROOT_DIR)/src
BUILDDIR    := $(ROOT_DIR)/build
TARGETDIR   := $(ROOT_DIR)/lib

EXAMPLE_DIRS := $(wildcard $(ROOT_DIR)/examples/*)

# Directories where include files are
INCDIR      := -I $(ROOT_DIR)/include \
			   -I /usr/local/Cellar/nlohmann-json/3.10.5/include/nlohmann \
			   -I /usr/local/Cellar/adamstark-audiofile/1.1.0/include \
			   -I /usr/local/Cellar/rtmidi/5.0.0/include/rtmidi

# Libraries for executables
LIBDIR      := -L $(ROOT_DIR)/lib
LIBS        := -lpthread -lyase -lportaudio -lrtmidi
