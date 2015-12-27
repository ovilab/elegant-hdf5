message(Library Deployment)

LIB_NAME = h5cpp

LIB_TARGET = $$qtLibraryTarget($$LIB_NAME)
LIBS += -L$$h5cpp_builddir/src/$${LIB_NAME} -l$${LIB_TARGET}
#LIBS += -L/usr/lib/x86_64-linux-gnu/hdf5/serial

QML_IMPORT_PATH += $$h5cpp_builddir/src/
QML2_IMPORT_PATH += $$QML_IMPORT_PATH
INCLUDEPATH += $$h5cpp_srcdir/src/
INCLUDEPATH += /usr/include/hdf5/serial

# Deployment of library
#ios {
#    simvis_lib.files = $$h5cpp_srcdir/src/imports/SimVis
#    simvis_lib.path = qt_qml
#    QMAKE_BUNDLE_DATA += h5cpp_lib
#    # undocumented Qmake property used to autogenerate Q_IMPORT_PLUGIN
#    QMLPATHS += $$simvis_srcdir/src/imports
#} macx {
#    copy_lib.commands = $(COPY_DIR) $$h5cpp_builddir/dist/$${LIB_NAME} $$OUT_PWD/$${TARGET}.app/Contents/MacOS
#    copy_lib.commands += && install_name_tool -change lib$${LIB_TARGET}.dylib @executable_path/$${LIB_NAME}/lib$${LIB_TARGET}.dylib $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
#} unix:!macx {
#    copy_lib.commands = $(COPY_DIR) $$h5cpp_builddir/dist/$${LIB_NAME} $$OUT_PWD
#}

first.depends = $(first) copy_lib
export(first.depends)
export(copy_lib.commands)
QMAKE_EXTRA_TARGETS += first copy_lib

