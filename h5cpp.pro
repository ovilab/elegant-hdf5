TEMPLATE = subdirs
SUBDIRS += src tests
CONFIG += ordered
tests.depends = src
