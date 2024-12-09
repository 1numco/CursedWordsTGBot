#!/bin/bash -e

source ./set_versions.sh

docker run --rm -it -v $(pwd):/tgbot -w /tgbot tgbot_base:${BASE_IMAGE_VERSION} /bin/bash -c "./build-local.sh"

docker build -t "tgbot:${TARGET_IMAGE_VERSION}" --build-arg BASE_IMAGE_VERSION=${BASE_IMAGE_VERSION} -f target_images/Dockerfile.CursedWordsTgBot .