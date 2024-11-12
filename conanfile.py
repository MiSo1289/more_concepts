import re

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake
from conan.tools.files import load, copy

class MoreConcepts(ConanFile):
    name = "more_concepts"
    revision_mode = "scm"
    description = "C++20 concepts library, providing container concepts and more."
    homepage = "https://github.com/MiSo1289/more_concepts"
    url = "https://github.com/MiSo1289/more_concepts"
    license = "MIT"
    settings = ("os", "compiler", "arch", "build_type")
    exports_sources = (
        "include/*",
        "tests/*",
        "CMakeLists.txt",
    )
    no_copy_source = True

    def package(self):
        copy(self, "*.hpp", self.source_folder, self.package_folder)

    def package_id(self):
        self.info.clear()

    def package_info(self):
         self.cpp_info.bindirs = []
         self.cpp_info.libdirs = []

    def set_version(self):
        try:
            content = load(self, "CMakeLists.txt")
            version = re.search("set\\(MORE_CONCEPTS_VERSION (.*)\\)", content).group(1)
            self.version = version.strip()
        except OSError:
            return None
