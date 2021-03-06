#                           How To Create a Makefile
#                           ~~~~~~~~~~~~~~~~~~~~~~~~
# Large C programs are built out of header (.h), source (.c), object (.o), and
# library (.a) files.  make is a rule-based expert system that determines which
# of these files must be created (because they do not exist) or recompiled /
# relinked (because they are not up to date) and issues the commands to do so.
#
# These commands and the dependencies among the files (that are used to decide
# which files must be (re)compiled or (re)linked) are specified in a file whose
# name can be either "Makefile" or "makefile".  This sample Makefile is somewhat
# contrived.  Consult the entry for make in the info system for full details.
#
# Within a Makefile lines that are blank or begin with # are ignored.  The rest
# contain macro definitions and rules.
#
# A macro definition has the form
#
#       VAR = VALUE
#
# and causes any subsequent occurrences of the strings "${VAR}" and "$(VAR)" to
# be replaced by the string "VALUE".  For example, the macros

CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs223/Hwk2

# specify
#
# * the C compiler to use (the default is gcc)
#
# * the options to pass to the C compiler (-g3 generates supplementary
#   information needed by ddd and gdb; -std=c99 specifies the C99 standard;
#   and -pedantic and -Wall check for certain questionable coding practices)
#
# * the class directory for the first programming assignment (${HWK} is thus
#   shorthand for /c/cs223/Hwk1).
#
# Note that macro definitions must begin in the first column.
#
# A rule has the form
#
#       TARGET: <list of prerequisite files>
#               COMMAND
#
# and specifies that the file TARGET depends upon each file in the list of
# prerequisites; and that, if TARGET does not exist or is older than at least
# one of these files, it can be created/updated by issuing the shell command
# COMMAND.  For example, the rule

Pack:  Pack.o
	${CC} ${CFLAGS} -lm -o Pack Pack.o

# states that the executable file Inflate depends upon the object file
# Inflate.o (*).  The command given will invoke the C compiler to create or
# relink Inflate (without the -o option the executable would be called a.out).
# -----
#   (*) The executable does not depend directly on the source file Inflate.c,
#   so the command to create Inflate should not refer to it either.  Rules for
#   creating object files from source files are discussed below.
#
# Note that the name of the target file must begin in the first column; the
# list of prerequisite files is delimited by whitespace and may be empty; and
# the shell command (which may be omitted) MUST BE PRECEDED BY A TAB CHARACTER.
#
# The source file Inflate.c must be recompiled whenever it changes, or more
# generally whenever Inflate.o does not exist or is older than Inflate.c.  This
# dependency could be specified by the rule
#
#       Inflate.o: Inflate.c
#               ${CC} ${CFLAGS} -c Inflate.c
#
# where the -c option specifies compilation but no linking.  However, it occurs
# sufficiently often that this rule is built into make and thus is almost never
# stated explicitly.
#
# If the source files Inflate.c and Psched.c (your solution to Homework #2)
# #include the header file common.h, then both files must be recompiled
# whenever that header file is modified.  This is specified by the rules
#
# Inflate.o: common.h
# Psched.o: common.h
#
# where the command to recompile is implicit and is thus omitted (see above).
# These dependencies can also be written as the single rule
#
#       Inflate.o Psched.o: common.h
#
# Note that the targets are the .o files, NOT the .c files, which do not depend
# upon common.h.
#
# Finally assume that another executable Subst16 depends upon the object files
# Subst16.o and library.o, and that Subst16.c and library.c also #include the
# header file library.h.  This could be specified by the rules

Subst16: Subst16.o library.o
	$(CC) $(CFLAGS) $^ -o $@

Subst16.o library.o: library.h

# where the special macro $@ expands to the target of the rule (here Subst16);
# the special macro $^ expands to the list of prerequisites (here Subst16.o and
# library.o).
#
# make is invoked as
#
#       % make <list of target files>
#
# which causes each of the named targets to be updated.  For example,
#
#       % make Subst16
#
# updates Subst16.  If no target is specified, then the targets of the first
# rule to appear in the Makefile is assumed.  Thus with this Makefile
#
#       % make
#
# is equivalent to
#
#       % make Inflate
#
# A few more details:
#
# * A file may appear in the target list of more than one rule, in which case
#   it depends on all of the files in the prerequisite lists of these rules.
#   However, only one of these rules may be followed by a shell command.
#
# * If a prerequisite file is itself a target, then it may have prerequisites
#   of its own.  Thus make recursively creates/updates each prerequisite of a
#   target BEFORE checking whether that target is older than its prerequisites.
#   However, no file is updated more than once.
#
# * The shell command can be a sequence of commands, one per line, but each
#   line must begin with a tab.  A blank line ends the sequence.
#
###############################################################################
# GOTCHA:  Each command line must begin with a tab character, NOT eight space #
#   characters, even though they look the same when output.                   #
###############################################################################
#
#                                                               CS-223-01/19/16
