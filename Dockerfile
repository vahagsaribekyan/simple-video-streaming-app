FROM spmallick/opencv-docker:opencv

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/dockertest1
 
# Specify the working directory
WORKDIR /usr/src/dockertest1

RUN apt-get update 
RUN apt-get install --yes libboost-all-dev openssl libssl-dev
RUN cmake .
RUN make

# Run the program output from the previous step
CMD ["./video-streaming-app"]
