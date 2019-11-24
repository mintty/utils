#! /bin/sh

width=
height=
pAR=
while case "$1" in
	-w|-width)	width=";width=$2"
			shift; shift;;
	-h|-height)	height=";height=$2"
			shift; shift;;
	-fit)		pAR=";preserveAspectRatio=0"
			shift;;
	*)	false;;
      esac
do	true
done

case "$*" in
"")	echo "Usage: `basename $0` [-w WIDTH] [-h HEIGHT] [-par] IMAGE [...]" >&2
	echo >&2
	echo "This script outputs images to the terminal for display." >&2
	echo "Parameters:" >&2
	echo "  -w, -width N	width in N cell units, or Npx, or N%" >&2
	echo "  -h, -height N	height in N cell units, or Npx, or N%" >&2
	echo "  -fit		with both width and height: do not preserve aspect ratio" >&2
	exit;;
esac

case "$BASH" in
?*)	echo=echo;;
*)	echo=/bin/echo;;
esac

for img in "$@"
do
  if [ -r "$img" ]
  then
    #eval $(file "$img" | sed -e 's/.*, *\([0-9]*\) *x *\([0-9]*\).*/width=\1px; height=\2px/')
    $echo -en "\e]1337;File=name=$(echo "$img" | base64 -w 0);inline=1"
    $echo -en "${width}${height}${pAR}:$(base64 -w 0 "$img")\a"
  else
    echo cannot read file "$img" >&2
  fi
done
