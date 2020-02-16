#!/bin/sh
DYLD_FRAMEWORK_PATH=/Users/will/projects/cpprtx/include/qtbuild/qtbase/lib${DYLD_FRAMEWORK_PATH:+:$DYLD_FRAMEWORK_PATH}
export DYLD_FRAMEWORK_PATH
QT_PLUGIN_PATH=/Users/will/projects/cpprtx/include/qtbuild/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /Users/will/projects/cpprtx/include/qtbuild/qtbase/bin/uic "$@"
