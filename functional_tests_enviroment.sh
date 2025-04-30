#!/bin/bash

# Проверка параметра
if [ "$1" == "up" ]; then
  echo "Starting the infrastructure..."
  docker-compose up -d grpc_server
elif [ "$1" == "down" ]; then
  echo "Shutting down the infrastructure..."
  docker-compose  down
else
  echo "Usage: $0 {up|down}"
  exit 1
fi