[ $2 ] || { echo "args: <filename> <blur radius>"; return 1 2>/dev/null; exit 1; }
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"
ffmpeg -y \
	-loglevel 0 \
	-i $1 \
	-vf boxblur=$2:$2 \
	$filename-blurred.jpg