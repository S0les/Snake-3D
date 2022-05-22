FROM debian:buster

RUN apt-get update \
    && env DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    gawk curl wget git diffstat unzip texinfo gcc build-essential sudo ssh \
    chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev pylint3 xterm python3-subunit mesa-common-dev tar locales zip rsync bc xxd file python python-pip binutils bzip2 g++ gcc gzip libncurses5-dev libdevmapper-dev libsystemd-dev mercurial whois patch perl vim bison flex libssl-dev libfdt-dev bmake libglfw3-dev libglfw3 libglew-dev \
    && apt-get autoremove && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN useradd -u 1000 snake-builder
RUN mkdir /home/snake-builder
RUN chown snake-builder:snake-builder /home/snake-builder
RUN localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV MESA_GL_VERSION_OVERRIDE 3.3
ENV LANG en_US.UTF-8
ENV VIDEO_CARDS "intel i965"

