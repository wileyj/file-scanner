AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
sysconf_DATA = \
		conf/backdoors.txt \
		conf/exclude \
		conf/include \
		conf/scan.config

bin_PROGRAMS = filescanner 
filescanner_SOURCES = \
		src/epoch.cc \
	   	src/sha1.cc \
		src/main.cc \
		src/filelisting.cc \
		src/smtpfuncs.cc \
		src/email.cc \
		src/help.cc \
		src/symlink.cc \
		src/regex.cc \
		src/filestats.cc \
		src/magic.cc \
		src/readfile.cc \
		src/log.cc
