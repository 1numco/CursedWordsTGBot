#!/bin/bash -e
source ./set_versions.sh

docker build -t "tgbot:${TARGET_IMAGE_VERSION}" -f common_images/Dockerfile.base .
