# Makefile
#
# ============================================================================
# Copyright (c) 
# Modified by eagle 2018/05/05
# ============================================================================

include ../config
TARGET = $(notdir $(CURDIR))

C_FLAGS += -Wall -fPIC

CPP_FLAGS += -I$(LINUXKERNEL_INSTALL_DIR)/include -I../Include

LD_FLAGS += -lpthread

COMPILE.c = $(COMPILER)g++ $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c = $(COMPILER)g++ $(LD_FLAGS)

DBGTARGET = ./libd/lib$(TARGET)d.so
RELTARGET = ./lib/lib$(TARGET).so

DBGCFLAGS = -g -D__DEBUG
RELCFLAGS = -O2 -fno-strict-aliasing

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

DBGOBJFILES=$(addprefix debug-$(COMPILER)/,$(SOURCES:%.cpp=%.o))
RELOBJFILES=$(addprefix release-$(COMPILER)/,$(SOURCES:%.cpp=%.o))

RELLDFLAGS = 
DBGLDFLAGS = 

.PHONY: clean debug release install

all:	debug release

install:
	install -d $(EXEC_DIR)
	install $(RELTARGET) $(EXEC_DIR)
	install -m 444 $(TARGET).txt $(EXEC_DIR)

release:	$(RELTARGET)

debug:		$(DBGTARGET)

$(RELTARGET):	$(RELOBJFILES)
	@mkdir -p lib
#	make release -C ./CommonModule
	$(LINK.c) -fPIC -shared -o $@  $(RELLDFLAGS) release-$(COMPILER)/*.o
$(DBGTARGET):	$(DBGOBJFILES)
	@mkdir -p libd
#	make debug -C ./CommonModule
	$(LINK.c) -fPIC -shared -o $@  $(DBGLDFLAGS) debug-$(COMPILER)/*.o

$(RELOBJFILES):	release-$(COMPILER)/%.o: %.cpp  $(HEADERS)
	@mkdir -p release-$(COMPILER)
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

$(DBGOBJFILES):	debug-$(COMPILER)/%.o: %.cpp  $(HEADERS)
	@mkdir -p debug-$(COMPILER)
	$(COMPILE.c) $(DBGCFLAGS) -o $@ $<

clean:
	-$(RM) -rf release* debug* *~ *.d .dep lib*
