# Makefile
#
# ============================================================================
# Copyright (c) Texas Instruments Inc 2007
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

include ../config
TARGET = $(notdir $(CURDIR))

C_FLAGS += -Wall 

CPP_FLAGS += -I$(LINUXKERNEL_INSTALL_DIR)/include -I../Include -I./

LD_FLAGSD +=  -L ./libd/
LD_FLAGSD += -lIOTModuled

LD_FLAGS +=  -L ./lib/
LD_FLAGS += -lIOTModule 


COMPILE.c = $(COMPILER)g++ $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c = $(COMPILER)g++ $(LD_FLAGS)
LINKD.c = $(COMPILER)g++ $(LD_FLAGSD)

DBGTARGET = ./Demod
RELTARGET = ./Demo

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
	$(LINK.c) -o $@ $^ $(RELLDFLAGS)
	$(COMPILER)strip ./lib/*

$(DBGTARGET):	$(DBGOBJFILES)
	$(LINKD.c) -o $@ $^ $(DBGLDFLAGS)

$(RELOBJFILES):	release-$(COMPILER)/%.o: %.cpp $(HEADERS)
	@mkdir -p release-$(COMPILER)
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

$(DBGOBJFILES):	debug-$(COMPILER)/%.o: %.cpp $(HEADERS)
	@mkdir -p debug-$(COMPILER)
	$(COMPILE.c) $(DBGCFLAGS) -o $@ $<

clean:
	-$(RM) -rf *~ *.d .dep debug* release* $(DBGTARGET) $(RELTARGET) lib
