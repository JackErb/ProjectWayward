cd build/
make
if [ "$1" = "run" ]; then
	open src/wayward.app/Contents/MacOS/wayward
fi
