# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := OpenCL_Wrapper_lib

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

OpenCL_Wrapper_lib: 
	@echo "==== Building OpenCL_Wrapper_lib ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f OpenCL_Wrapper_lib.make

clean:
	@${MAKE} --no-print-directory -C . -f OpenCL_Wrapper_lib.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   OpenCL_Wrapper_lib"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
