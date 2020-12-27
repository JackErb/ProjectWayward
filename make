cd build/
make
if [ "$1" = "run" ]; then
	open wayward.app/Contents/MacOS/wayward
fi
