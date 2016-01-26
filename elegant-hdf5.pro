TEMPLATE = subdirs
SUBDIRS += src
CONFIG += ordered

CONFIG(notests) {
    message(NOTE: Skipping tests)
} else {
    SUBDIRS += tests
    tests.depends = src

    SUBDIRS += example
    example.depends = src
}
