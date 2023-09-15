FROM fedora:latest

LABEL maintainer="ad.tomik@seznam.cz"
LABEL version="0.1.0"

RUN dnf update -y && \
    dnf install -y \
    cmake \
    clang \
    clang-tools-extra \
    cppcheck \
    gtest \
    gtest-devel \
    boost \
    boost-devel \
    ninja-build \
    gdb \
    git \
    curl \
    && dnf clean all