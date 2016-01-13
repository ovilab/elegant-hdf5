TEMPLATE = subdirs
SUBDIRS += src
CONFIG += ordered

CONFIG(notests) {
    message(Skipping tests)
} else {
    message(Tests included)
    SUBDIRS += tests
    tests.depends = src
}
