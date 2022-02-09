#Compilers
CC          := g++ -std=c++20
DGEN        := doxygen

#The Target Binary Program
TARGET      := monophonic

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := .
INCDIR      := ../../include
LIBDIR      := ../../lib
BUILDDIR    := .
TARGETDIR   := ./bin
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -Wno-psabi -O3
LIB         := -lpthread -lyase -lportaudio -lrtmidi
LIBDIR      := -L$(LIBDIR) -L../../../rtmidi
INC         := -I$(INCDIR)  -I ../../../json/include -I ../../../json/include/nlohmann

#Files
HEADERS     := $(wildcard $(INCDIR)/*.h) $(wildcard $(SYNDIR)/*.h) 
SOURCES     := $(wildcard $(SRCDIR)/*.cc) 
OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET) 

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o $(TARGETDIR)/$(TARGET)

huh:
	echo $(SOURCES)
	echo $(OBJECTS)

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(LIB) $(LIBDIR) $(OBJECTS) -o $@
	
#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -fPIC -o $@ $<
