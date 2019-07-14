#! /bin/bash

#############################################################################
# Detect and report terminal type, name, version.
# works with bash, dash, posh, zsh, ksh->mksh
# not with tcsh, fish

case "$1" in
-h)	echo "Usage: $0 [-t|-n|-v|-p]"
	echo "Query and report the terminal type, using Secondary Device Attribute report."
	echo
	echo Arguments:
	echo "  -t  report numeric type"
	echo "  -n  report terminal name"
	echo "  -v  report version"
	echo "  -p  report 3rd parameter"
	return
	;;
esac

#############################################################################
# query functions

query () {
  echo -n "$*" > /dev/tty
  dd < /dev/tty 2> /dev/null
}

terminaltype () {

case "$BASH" in
?*)	# try simplified query as suggested by Brian Inglis:
	# https://github.com/mintty/mintty/issues/776#issuecomment-475761566
	read -s -dc -p $'\E[>c' da < /dev/tty
	da=${da##$'\E'[>}
	set - ${da//;/ }
	;;
*)	# other shells do not have flexible read and string operations
	# so it's a bit more tricky...
	test -t 0 && stty=`stty -g`
	test -t 0 && stty raw -echo min 0 time 5
	da=`query "[>c"`
	test -t 0 && stty "$stty"
	da=${da#??}
	type="${da%%;*}"
	rest="${da#*;}"
	vers="${rest%%;*}"
	rest="${rest#*;}"
	rest="${rest%%c}"
	set - $type $vers $rest
;;
esac

case "$1" in
0)	t=VT100
	case "$2" in
	95)	t=tmux;;
	115)	t=KDE-konsole;;
	136)	t=PuTTY;;
	304)	t=VT125;;
	esac;;
1)	t=VT220
	case "$2" in
	2)	t=openwin-xterm;;
	96)	t=mlterm;;
	304)	t=VT241/VT382;;
	1115)	t=gnome-terminal;;
	esac
	if [ 0$2 -ge 2000 ]
	then	t=gnome-terminal
	fi
	;;
2)	t=VT240;;
24)	t=VT320;;
18)	t=VT330;;
19)	t=VT340;;
41)	t=VT420;;
61)	t=VT510;;
64)	t=VT520;;
65)	t=VT525;;
28)	t=DECterm;;
67)	t=cygwin;;
77)	t=mintty;;
82)	t=rxvt;;
83)	t=screen;;
84)	t=tmux; set - $1 200;;
85)	t=rxvt-unicode;;
*)	t=unknown;;
esac

tt=$1
v=$2
pc=$3

}

#############################################################################
# invoke the query and report as requested

terminaltype

case "$1" in
-t)	echo $tt;;
-n)	echo $t;;
-v)	echo $v;;
-p)	echo $pc;;
*)	echo $tt $t $v $pc;;
esac

#############################################################################
# end
