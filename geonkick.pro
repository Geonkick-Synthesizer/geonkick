TEMPLATE = subdirs

PRE_TARGETDEPS += libgeonkick.a

SUBDIRS = api \
          standalone \
          lv2

geonkickapi.target = libgeonkick.a
geonkickapi.commands = mkdir api && cd api && make -f ../../api/Makefile

QMAKE_EXTRA_TARGETS += geonkickapi

standalone.depends = libgeonkick.a


