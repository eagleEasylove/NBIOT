echo +++++++++++++++++++++++++++++++++++++++++++++++++++
echo             make $1 IOTModule
echo +++++++++++++++++++++++++++++++++++++++++++++++++++
echo
echo

usage()
{
	echo "usage: '$0  clean'" 
	echo "	 '$0  release'" 
	echo "	 '$0  debug'" 
}

if ([ "$1" == "" ])
then
	usage
	exit 1
fi

Release_Cmd=$1
if [ "$1" != "clean" ];then
	Release_Cmd="release -e"
fi

echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "               begin compile "$1", pls wait                    "
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

echo --------------------make $1 IOTModule-----------------------
cd IOTModule
make ${Release_Cmd} C_FLAGS='-fPIC -Wall'

if [ "$1" != "clean" ]
then
	cp -rf lib* ../Demo
fi

cd ../


if [ "$1" == "clean" ]
then
	cd Demo
	make ${Release_Cmd}
	cd ../
fi

if [ "$1" != "clean" ]
then
	echo --------------------make $1 Demo---------------------------
	cd Demo
	make ${Release_Cmd}

	if [ "${Release_Cmd}" == "release -e" ];then
		echo "--------------------copy files ...--------------------"
		cp -rf ./lib ../bin/
		cp -rf Demo ../bin/
	fi
	echo "OK."
fi


echo ++++++++++++++++++++++++++++++++++++++
echo          make $1 IOTModule $STATUS
echo ++++++++++++++++++++++++++++++++++++++
