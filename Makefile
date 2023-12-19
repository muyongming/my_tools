###
###
###
# %.o: %.c
# 	$(Q)$(CC) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

GCC_PREFIX = x86_64-w64-mingw32-

CC  = $(GCC_PREFIX)gcc
CXX = $(GCC_PREFIX)g++
AR  = $(GCC_PREFIX)ar



target += get_file_header_path.exe 
target += get_file_header_path_clangd.exe




Q := @


all: $(target)


get_file_header_path.exe: get_file_header_path.c
	$(Q)gcc -o $@  $<

get_file_header_path_clangd.exe: get_file_header_path_clangd.c
	$(Q)gcc -o $@  $<
	
	
.PHONY: clean

clean: 
	$(Q)rm -rf $(target)
	
help:
	@echo "make all       -- build all target"
	@echo "male clean     -- clean all target"