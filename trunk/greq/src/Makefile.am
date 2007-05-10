-include local_options.mk
bin_PROGRAMS = greq
greq_SOURCES = greq.c check_button.c entry_field.c okay_cancel.c\
	       option_menu.c greq.h
MOSTLYCLEANFILES= *~ *.s
MAINTAINERCLEANFILES = Makefile.in stamp-h.in
CFLAGS += $(GTK_CFLAGS)
LDFLAGS += $(GTK_LIBS)
%.s: %.c
	$(COMPILE) -S $<
