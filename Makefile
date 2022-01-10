#Compilers
CC          := g++ -std=c++20
DGEN        := doxygen

#The Target Binary Program
TARGET      := libyase.a

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := ./src
SYNDIR      := ./synths
INCDIR      := ./include
BUILDDIR    := ./build
SYNDIR      := ./synths
TARGETDIR   := ./lib
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -Wno-psabi -O3
LIB         := -lgtest -lpthread 
INC         := -I$(INCDIR)  -I./synths -I ../json/include -I ../json/include/nlohmann
INCDEP      := -I$(INCDIR)

#Files
DGENCONFIG  := docs.config
HEADERS     := $(wildcard $(INCDIR)/*.h) $(wildcard $(SYNDIR)/*.h) 

SOURCES     := $(wildcard $(SRCDIR)/*.cc) 
OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

SYNSOURCES  := $(wildcard $(SYNDIR)/*.cc)
SYNOBJECTS  := $(patsubst %.cc, $(SYNDIR)/%.o, $(notdir $(SYNSOURCES)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET) tests example

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

tests:
	cd test && $(MAKE) -j 8

example:
	cd examples && $(MAKE) -j 8

docs: docs/index.html

docs/index.html: $(SOURCES) $(SYNSOURCES) $(HEADERS) README.md docs.config examples/*.cc
	$(DGEN) $(DGENCONFIG)
	cp .nojekyll docs

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o
	cd test && $(MAKE) clean
	cd examples && $(MAKE) clean

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TARGETDIR)/$(TARGET) *.db
	@$(RM) -rf build lib html latex
	cd test && $(MAKE) spotless

huh:
	echo $(SOURCES)
	echo $(OBJECTS)

wc:
	wc -l */*.cc */*.hh | sort

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS) $(SYNOBJECTS) $(HEADERS)
	ar rvs $@ $(OBJECTS) $(SYNOBJECTS)
	
#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -fPIC -o $@ $<

#Compile
$(SYNDIR)/%.o: $(SYNDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -fPIC -o $@ $<	