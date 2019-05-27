mkdir public
ffmpeg -f lavfi -i testsrc=duration=10:size=qcif:rate=30 dummy.mp4
ffmpeg -i dummy.mp4 -f hls public/index.m3u8
rm dummy.mp4
./video-streaming-app
