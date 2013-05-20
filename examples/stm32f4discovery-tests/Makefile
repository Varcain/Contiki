CONTIKI_PROJECT = stm32f4discovery-tests
all: $(CONTIKI_PROJECT)

APPS = telnetd shell collect-view

DEFINES=PROJECT_CONF_H=\"webserver-ajax-conf.h\"

PROJECTDIRS = . $(CONTIKI)/apps/webserver
PROJECT_SOURCEFILES = ajax-cgi.c httpd-fs.c http-strings.c \
				httpd.c webserver-dsc.c webserver-nogui.c

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
