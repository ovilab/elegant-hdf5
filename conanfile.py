from conans import ConanFile, CMake


class H5CppConan(ConanFile):
    name = "h5cpp"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    exports = "elegant-hdf5.pro", "src/*", "tests/*", "libs/*", ".qmake.conf", "library_deployment.pri"
    url = "http://github.com/ovilab/elegant-hdf5"
    license = "gplv3"

    def build(self):
        cmake = CMake(self.settings)
        print cmake.command_line
        print cmake.build_config
        self.run('mkdir build')
        self.run('cd build && qmake ../elegant-hdf5.pro CONFIG+=notests')
        self.run("cd build && make -j4")

    def package(self):
        self.copy("*", dst="include/h5cpp", src="src/h5cpp")
        self.copy("*.h", dst="include/h5cpp", src="src")
        self.copy("*.so*", dst="lib", src="build/src")
        # self.copy("*.a", dst="lib", src="hello/lib")

    def package_info(self):
        self.cpp_info.libs = ["elegant_hdf5"]
