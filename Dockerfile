FROM spmallick/opencv-docker:opencv

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/develandoo

# Specify the working directory
WORKDIR /usr/src/develandoo

RUN apt-get update 
RUN apt-get install --yes libboost-all-dev libssl-dev ffmpeg
RUN cmake .
RUN make

# Run the program output from the previous step
CMD ./run.sh
