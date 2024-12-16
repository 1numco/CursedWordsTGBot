#!/bin/bash -e

source ./set_versions.sh

docker run -it -e command_line_arguments="-token 7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo" "cwbot:${TARGET_IMAGES_VERSION}"
