# -*- Makefile -*-

include Makefile.compilers

SOURCES = $(wildcard *.cc)
DEPENDENCIES = $(SOURCES:.cc=.d)
OBJECTS = $(SOURCES:.cc=.o)
TARGET = libcpt.a

.SUFFIXES:
.SUFFIXES: .cc .d .o

# Make the library by default.
default: $(TARGET)

$(TARGET): $(OBJECTS) $(DEPENDENCIES)
	ar -r $(TARGET) $(OBJECTS)

clean: 
	$(RM) *.o *.d *~ 

distclean: 
	$(MAKE) clean 
	$(RM) $(TARGET) $(DEPENDENCIES)

again: 
	$(MAKE) distclean 
	$(MAKE) 

# Implicit rules.

.cc.d: 
	$(CXX) -MM $(CXXINCLUDE) $< > $@.$$$$; \
  sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  $(RM) $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(DEPENDENCIES)
endif
endif
