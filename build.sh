#!/usr/bin/env bash

compiler="clang"
os=$(uname)

if [[ "$os" == 'Linux' ]]; then
	compiler="gcc"
fi

if ! [[ -z "${COMPILER}" ]]; then
	compiler="${COMPILER}"
fi

compiler_version=$($compiler --version)
echo "Using compiler: $compiler"

echo "Creating directories ..."

mkdir -p "buildfiles/src/"
mkdir -p "bin"

find src -name "*.c" | grep -v "tests" | grep -v "main.c" | while read -r src_file; do
	echo "Compiling: $src_file ..."
	$compiler -O2 -std=c11 -c $src_file -o buildfiles/$src_file.o -I/usr/local/include
	if ! [[ $? == 0 ]]; then
		break
	fi
done

if ! [[ $? == 0 ]]; then
	echo "Error in compiling sources, will not continue"
	exit $?
fi

echo "Building ..."

buildfiles=$(find buildfiles -name "*.c.o" | paste -sd " " -)
$compiler -O2 -std=c11 -g -o bin/ls_extended src/main.c $buildfiles -I/usr/local/include -L/usr/local/lib
