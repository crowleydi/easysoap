#! /bin/sh

#
# Create the libtool helper files
#
echo "Copying libtool helper files ..."

#
# --force to ensure that we replace with current files
# --copy to avoid symlinks; we want originals for the distro
# --automake to make it shut up about "things to do"
#
libtoolize --force --copy --automake

#
# Generate the autoconf header template (config.h.in) and ./configure
#
echo "Creating macros ..."
aclocal

echo "Creating config.h.in ..."
autoheader

echo "Creating makefiles ..."
automake --add-missing --copy

echo "Creating configure ..."
### do some work to toss config.cache?
autoconf

# exit with the right value, so any calling script can continue
exit 0
