TEMPLATE = subdirs

SUBDIRS += \
    boost/object \
    libs/object/examples \
    libs/object/test

test.depends = object examples
