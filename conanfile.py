import re

from conans import CMake, ConanFile, tools


def get_version():
    try:
        content = tools.load("CMakeLists.txt")
        version = re.search(b"set\\(MORE_CONCEPTS_VERSION (.*)\\)", content).group(1)
        return version.strip()
    except OSError:
        return None


class MoreConcepts(ConanFile):
    name = "more_concepts"
    version = get_version()
    revision_mode = "scm"
    description = "C++20 concepts library, providing container concepts and more."
    homepage = "https://github.com/MiSo1289/more_concepts"
    url = "https://github.com/MiSo1289/more_concepts"
    license = "MIT"
    exports_sources = (
        "include/*",
        "tests/*",
        "CMakeLists.txt",
    )

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.test(target="more_concepts_tests")

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
