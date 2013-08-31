TEMPLATE = subdirs

CONFIG += debug_and_release

SUBDIRS += EGEEditor \
           EditorPlugins

EGEEditor.depends = EditorPlugins
