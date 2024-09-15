all: r
	docker run --rm -v "`pwd`":/src sgdk
	picodrive out/rom.bin 2> /dev/null
r:
	cd res; make
clean:
	rm -rf out
	cd res; make clean
mp4:
	ffmpeg -y -i *.mkv -b 1200k -r 60  -vf "scale=960:720" -vcodec libx264 -pix_fmt yuv420p aaa.mp4
mp42:
	ffmpeg -y -i *.mkv  a.mp4 && rm *.mkv
