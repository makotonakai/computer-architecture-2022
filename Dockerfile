FROM ubuntu:20.04


RUN apt-get update \
    && apt-get install -y software-properties-common \  
    && apt-get install -y git \
    && apt-get install -y make \
    && apt-get install -y cmake \
    && apt-get install -y wget \
    && apt-get install -y curl \
    && apt-get install -y sudo \
    && apt-get install -y swig \
    && apt-get install -y build-essential \
    && apt-get install -y libomp-dev \
    && apt-get install -y spim \
    && apt-get install -y python3.7 \
    && apt-get install -y qemu \
    && apt-get install -y libc6-armel-cross \
    && apt-get install -y libc6-dev-armel-cross \
    && apt-get install -y binutils-arm-linux-gnueabi \
    && apt-get install -y libncurses5-dev \
    && apt-get install -y bison \
    && apt-get install -y flex \
    && apt-get install -y libssl-dev \
    && apt-get install -y bc \
    && apt-get install -y gcc-arm-linux-gnueabi \
    && apt-get install -y gcc-arm-none-eabi \
    && apt-get install -y gdb-multiarch \
    && apt-get install -y net-tools \
    && apt-get install -y mpi \
    && apt-get install -y mpich \
    && apt-get install -y openssh-server \
    && apt-get install -y emacs \
    && apt-get install -y vim

RUN alias python=python3.7
RUN mkdir computer-architecture
WORKDIR /root/computer-architecture
COPY ./computer-architecture-2022 /root/computer-architecture
