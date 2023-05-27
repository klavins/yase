#Compilers
CC          := g++ -std=c++20 -D POTATO
CFLAGS      := -Wno-psabi -O3
DGEN        := doxygen
SRCEXT      := cc

#The Target Binary Program
TARGET      := libyase.a

#The Directories, Source, Includes, Objects, Binary and Resources
ROOT_DIR    := /home/klavins//yase
SRCDIR      := $(ROOT_DIR)/src
BUILDDIR    := $(ROOT_DIR)/potato/build
TARGETDIR   := $(ROOT_DIR)/potato/lib

EXAMPLE_DIRS := $(wildcard $(ROOT_DIR)/examples/*)

# Directories where include files are
INCDIR      := -I $(ROOT_DIR)/include \
               -I /usr/include/nlohmann \
	       -I /usr/include/rtmidi

# Libraries for executables
LIBDIR      := -L $(ROOT_DIR)/potato/lib
LIBS        := -lpthread -lyase -lrtmidi
