#!/bin/bash -e
source ./set_versions.sh

docker build -t "cwbot_base:${COMMON_IMAGES_VERSION}" -f common_images/Dockerfile.base .