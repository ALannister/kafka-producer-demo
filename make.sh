#!/bin/bash
if [ $# -eq 1 ] ; then
	if [ $1 == help ] ; then
		echo "1) make.sh 341: for GCC341"
		echo "2) make.sh 444: for GCC444"
		echo "2) make.sh 342: for GCC342"
		echo "2) make.sh 434: for GCC434"
		echo "3) make.sh pc: for pc"
	elif [ $1 == "341" ] ; then 
		echo "complie agent for GCC341"
		./make_arm341.sh
	elif [ $1 == "444" ] ; then
		echo "complie agent for GCC444"
		./make_arm444.sh
	elif [ $1 == "342" ] ; then
		echo "complie agent for GCC342"
		./make_mips342.sh
	elif [ $1 == "346" ] ; then
		echo "complie agent for GCC346"
		./make_mips346.sh
	elif [ $1 == "434" ] ; then
		echo "complie agent for GCC434"
		./make_mips434.sh
	elif [ $1 == "446" ] ; then
		echo "complie agent for GCC446"
		./make_mips446.sh
	else
		echo "complie agent for pc"
		./make_i386.sh
	fi
else
	 echo "complie agent for all"
	./make_i386.sh
	./make_arm341.sh
	./make_mips342.sh
	./make_arm444.sh
	./make_mips434.sh
	./make_mips346.sh
	./make_mips446.sh
fi
