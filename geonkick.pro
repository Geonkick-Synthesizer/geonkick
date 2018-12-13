TEMPLATE = subdirs

PRE_TARGETS = geonkickapi

SUBDIRS = standalone
#lv2

geonkickapi.target = libgeonkick.a
geonkickapi.commands = mkdir api && cd api && make -f ../../api/Makefile

QMAKE_EXTRA_TARGETS += geonkickapi



