#The Target Binary Program
TESTTARGET      := test

#The Directories, Source, Includes, Objects, Binary and Resources

include ../defs.mk
TESTSRCDIR      := .
TESTBUILDDIR    := ./build
TESTTARGETDIR   := ./bin

#Flags, Libraries and Includes
CFLAGS      := -ggdb
LIB         := $(LIBDIR) $(LIBS) -lgtest 

#Files
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.cc)
OBJECTS     := $(patsubst %.cc, $(TESTBUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(TESTTARGETDIR)/$(TESTTARGET)

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TESTTARGETDIR)
	@mkdir -p $(TESTBUILDDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(TESTBUILDDIR)/*.o

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TESTTARGETDIR)/$(TESTTARGET) 

#Link
$(TESTTARGETDIR)/$(TESTTARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TESTTARGETDIR)/$(TESTTARGET) $^ $(LIB)

#Compile
$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.$(SRCEXT) 
	$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

.PHONY: directories remake clean cleaner apidocs $(TESTBUILDDIR) $(TESTTARGETDIR)