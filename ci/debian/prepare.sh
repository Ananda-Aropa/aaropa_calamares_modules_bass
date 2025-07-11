#!/bin/bash

# Apply patches
for patch in patches/*.sh; do
	./"$patch"
done

# Modules to build:
_modules="
	bassoptions
	presets
	"

cd src/modules

# Check if $1 is in $_modules
is_module_enabled() {
	case "$_modules" in
	*$1*) return 0 ;;
	*) return 1 ;;
	esac
}

# Fill $_skip_modules, list disabled modules
for i in *; do
	if ! [ -d "$i" ] || is_module_enabled "$i"; then
		continue
	fi
	_skip_modules="$_skip_modules $i"
done

cd ../..

dh_auto_configure -- \
	-DCMAKE_BUILD_TYPE=Debug \
	-DINSTALL_CONFIG=OFF \
	-DKDE_INSTALL_USE_QT_SYS_PATHS=ON \
	-DSKIP_MODULES="$_skip_modules" \
	-DWEBVIEW_FORCE_WEBKIT=1 \
	-DWITH_QT6=ON
