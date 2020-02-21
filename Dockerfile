FROM debian:buster-slim

RUN apt-get update && \
  apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    gcc \
    git \
    lftp \
    libc6-dev \
    make \
    openssh-client && \
  rm -rf /var/lib/apt/lists/*
