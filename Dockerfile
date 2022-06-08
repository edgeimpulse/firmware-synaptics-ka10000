FROM ubuntu:20.04

WORKDIR /app

ARG DEBIAN_FRONTEND=noninteractive

# Install tools
RUN dpkg --add-architecture i386 && \
        apt-get update && apt-get install -y \
        libc6:i386 \
        libncurses5:i386 \
        libstdc++6:i386 \
        python3 \
        python3-pip \
        tclsh \
        gawk \
        unzip \
        pip \
        python3-tk \
        wget \
        software-properties-common \
        libncurses5 \
        libncurses5:i386 \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# GCC ARM
RUN wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 && \
    tar xjf gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 -C /opt/ && rm gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2

# Tensaiflow Compiler
RUN wget https://cdn.edgeimpulse.com/build-system/tensaicc -O /opt/tensaicc && \
    chmod +x /opt/tensaicc

# Set paths
ENV PATH="/opt/gcc-arm-none-eabi-7-2017-q4-major/bin:${PATH}"
ENV TENSAI_COMPILER_DIR=/opt
