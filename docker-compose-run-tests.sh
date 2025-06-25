#!/bin/bash
set -e

echo "Запуск running_bot..."
docker compose up -d running_bot

echo "Запуск generator..."
docker compose run --rm generator

echo "Запуск checker..."
docker compose run --rm checker

echo "Остановка тестовых контейнеров..."
docker compose stop running_bot
docker compose rm -f running_bot generator checker