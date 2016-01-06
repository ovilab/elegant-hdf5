TEMPLATE = subdirs
SUBDIRS += src
CONFIG += ordered

CONFIG(tests) {
    message(Tests included)
    SUBDIRS += tests
    tests.depends = src
} else {
    message(Skipping tests)
}
