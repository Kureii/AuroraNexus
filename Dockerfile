FROM fedora:latest

LABEL maintainer="ad.tomik@seznam.cz"
LABEL version="0.1.1"

RUN dnf update -y && \
    dnf install -y \
    git \
    curl \
    cmake \
    gcc \
    clang \
    clang-tools-extra \
    cppcheck \
    ninja-build \
    gdb \
    valgrind \
    gtest \
    gtest-devel \
    gmock \
    gmock-devel \
    boost \
    boost-devel \
    && dnf clean all

# Copying source files
COPY . /app

# Setting up a working directory
WORKDIR /app

# Compilers
RUN mkdir build -p && cd build && cmake .. && make

# Work folder for manual execution of unit tests (it is not possible to use auto start)
# WORKDIR /app/build/bin/tests/

# Notification that port 8080 will be used
EXPOSE 8080

# Automatic test execution
# CMD ["./build/bin/tests/AuroraNexus_tests"]

# Automatic project start
# CMD ["./build/bin/AuroraNexus"]