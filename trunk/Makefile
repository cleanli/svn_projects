###############################################################################
#
## Generic Makefile for C/C++ Program
#
## Author: whyglinux (whyglinux AT hotmail DOT com)
# Date:   2006/03/04
# # Description:
# # The makefile searches in  directories for the source files
# # with extensions specified in , then compiles the sources
# # and finally produces the
# , the executable file, by linking
# # the objectives.
# # Usage:
# #   $ make           compile and link the program.
# #   $ make objs      compile only (no linking. Rarely used).
# #   $ make clean     clean the objectives and dependencies.
# #   $ make cleanall  clean the objectives, dependencies and executable.
# #   $ make rebuild   rebuild the program. The same as make clean && make all.
# #==============================================================================
# ## Customizing Section: adjust the following if necessary.
# ##=============================================================================
# # The executable file name.
# # It must be specified.
# # PROGRAM   := a.out    # the executable name
 PROGRAM   := cleanboot
# # The directories in which source files reside.
# # At least one path should be specified.
# # SRCDIRS   := .        # current directory
 SRCDIRS   := .
# # The source file types (headers excluded).
# # At least one type should be specified.
# # The valid suffixes are among of .c, .C, .cc, .cpp, .CPP, .c++, .cp, or .cxx.
# # SRCEXTS   := .c      # C program
# # SRCEXTS   := .cpp    # C++ program
# # SRCEXTS   := .c .cpp # C/C++ program
 SRCEXTS   := .c .S
# # The flags used by the cpp (man cpp for more).
# # CPPFLAGS  := -Wall -Werror # show all warnings and take them as errors
# CPPFLAGS  :=
# # The compiling flags used only for C.
# # If it is a C++ program, no need to set these flags.
# # If it is a C and C++ merging program, set these flags for the C parts.
 CFLAGS    := -g  -Os   -fno-strict-aliasing  -fno-common -ffixed-r8 -mshort-load-bytes -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -march=armv4 -mapcs-32 -mno-thumb-interwork -I/home/clean/arm-linux/arm-tools/lib/gcc-lib/arm-linux/3.3.6/include/
 SFLAGS	   := -D__ASSEMBLY__
# CFLAGS    +=
# # The compiling flags used only for C++.
# # If it is a C program, no need to set these flags.
# # If it is a C and C++ merging program, set these flags for the C++ parts.
# CXXFLAGS  :=
# CXXFLAGS  +=
# # The library and the link options ( C and C++ common).
 LDFLAGS   := -Bstatic -L /home/clean/arm-linux/arm-tools/bin/../lib/gcc-lib/arm-linux/3.3.6 -lgcc -lm /home/clean/arm-linux/arm-tools/lib/gcc-lib/arm-linux/3.3.6/libgcc.a
# LDFLAGS   +=
# ## Implict Section: change the following only when necessary.
# ##=============================================================================
# # The C program compiler. Uncomment it to specify yours explicitly.
CC      = arm-linux-gcc
ARMLD      = arm-linux-ld
# # The C++ program compiler. Uncomment it to specify yours explicitly.
# #CXX     = g++
# # Uncomment the 2 lines to compile C programs as C++ ones.
# #CC      = $(CXX)
# #CFLAGS  = $(CXXFLAGS)
# # The command used to delete file.
RM        = rm -f
# ## Stable Section: usually no need to be changed. But you can add more.
##=============================================================================
SHELL   = /bin/sh
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS    = $(foreach x,$(SRCEXTS), \
      $(patsubst %$(x),%.o,$(filter %$(x),$(SOURCES))))
DEPS    = $(patsubst %.o,%.d,$(OBJS))

.PHONY : all objs clean cleanall rebuild

all : $(PROGRAM).bin

# Rules for creating the dependency files (.d).
# #---------------------------------------------------

%.d : %.c
	@$(CC) -MM -MD $(CFLAGS) $<

%.d : %.S
	@$(CC) -MM -MD $(SFLAGS) $(CFLAGS) $<

%.d : %.cc
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cpp
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.CPP
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.c++
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cp
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cxx
	@$(CC) -MM -MD $(CXXFLAGS) $<

#---------------------------------------------------

objs : $(OBJS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

%.o : %.S
	$(CC) -c $(SFLAGS) $(CFLAGS) $<

%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.CPP
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.c++
	$(CXX -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

# Rules for producing the executable.
# #----------------------------------------------
#
$(PROGRAM).bin : $(PROGRAM).elf 
	arm-linux-objcopy -O binary $(PROGRAM).elf $(PROGRAM).bin      
	arm-linux-objdump -d -S $(PROGRAM).elf > $(PROGRAM).antiasm
	#cp $(PROGRAM).bin /var/ftp/pub
	tftp -p -l $(PROGRAM).bin 192.168.58.43

$(PROGRAM).elf : $(OBJS) $(PROGRAM).lds
	$(ARMLD) -T $(PROGRAM).lds -Map $(PROGRAM).map $(OBJS) $(LDFLAGS) -o $(PROGRAM).elf
	#$(ARMLD) -T $(PROGRAM).lds -Map $(PROGRAM).map -o $(PROGRAM).elf $(LDFLAGS)

-include $(DEPS)

rebuild: clean all

clean :
	@$(RM) *.o *.d

cleanall: clean
	@$(RM) $(PROGRAM) $(PROGRAM).exe

### End of the Makefile ##  Suggestions are welcome  ## All rights reserved ###
###############################################################################
