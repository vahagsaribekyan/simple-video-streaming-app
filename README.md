# This repository contains hls streaming simple application 

## Build docker image with application

In order to build 0.0.1 version image go to Dockerfile directory and type

docker build -t simple-video-streaming-app:0.0.1

## Run commands

1. To start the app locally you need to install dependencies then luanch run.sh script.

You need to have this packages installed: openCV, cmake, build-essential, libboost-all-dev, libssl-dev, ffmpeg

Then run this commands.
cmake .
make
run.sh

2. You can launch docker container with following command

docker run simple-video-streaming-app:0.0.1

## Example
1. curl https://localhost:8080 -k (please pass -k flag to skip certificate verification)
2. curl -H "Authorization: Basic dXNlcjpwYXNzd29yZA==" -d "" -X POST https://localhost:8080/record -k
3. curl -H "Authorization: Basic dXNlcjpwYXNzd29yZA==" -d "" -X POST https://localhost:8080/frames -k

Note: In above example we pass empty payload with -d flag, it is related to c++ http library implementation.