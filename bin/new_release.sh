#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/new_release.sh" ]
then
	echo "bin/new_release.sh: must be executed from its parent directory"
	exit 1
fi
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
repository="https://projects.coin-or.org/svn/CppAD"
stable_version="20150000"
release='0'
release_version="$stable_version.$release"
# -----------------------------------------------------------------------------
# Check release version
if svn list $repository/releases | grep "$release_version" > /dev/null
then
	echo bin/"new_release.sh: Release number $release_version already exists."
	echo "You must first change the assigment"
	echo "	release=$release"
	echo "in file bin/new_release.sh to a higher release number."
	exit 1
fi
# -----------------------------------------------------------------------------
echo_eval git checkout stable/$stable_version
# -----------------------------------------------------------------------------
#
check_one=`bin/version.sh get`
echo_eval git checkout doc.omh
check_two=`grep "cppad-$stable_version" doc.omh \
	| sed -e 's|cppad-\([0-9.]*\):.*|\1|'`
if [ "$check_one" != "$release_version" ] || [ "$check_one" != "$check_two" ]
then
	echo 'bin/new_release.sh:'
	echo 'Set new version number with following commands:'
	echo "	bin/version.sh set $release_version"
	echo '	bin/version.sh copy'
	echo 'then test and then commit the changes.'
	exit 1
fi
# -----------------------------------------------------------------------------
# tag this version of the repository
if git tag --list | grep "$release_version"
then
	git tag -d $release_version
fi
git tag -a \
	-m "corresponds $repository/releases/$release_version" \
	$release_version
#
# copy committed changes to coin repository
echo_eval git svn dcommit
#
# copy commited changes to github repository
echo_eval git push github stable/$stable_version
echo_eval git push --tabs github
# -----------------------------------------------------------------------------
msg="Creating releases/$release_version"
rep_stable="$repository/stable/$stable_version"
rep_release="$repository/releases/$release_version"
echo_eval svn copy $rep_stable $rep_release -m \"$msg\"
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	echo_eval mkdir -p build
fi
echo_eval cd build 
echo_eval svn checkout $repository/conf conf
#
echo_eval cd conf
#
msg="Update stable and release numbers in projDesc.xml"
echo 'Settting stable and advance release in ../../conf/projDesc.xml.'
sed -i projDesc.xml \
	-e "/^ *<stable/,/^ *<\/stable/s/[0-9]\{8\}/$stable_version/" \
	-e "/^ *<release/,/^ *<\/release/s/[0-9]\{8\}\.[0-9]*/$release_version/"
#
echo "Use the command the following command to finish the process"
echo "	svn commit -m \"$msg\" build/conf/projDesc.xml"
