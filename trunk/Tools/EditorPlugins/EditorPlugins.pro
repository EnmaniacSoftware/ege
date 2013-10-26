TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += CorePlugin \
           WorkspacePlugin \
           JuiceJumpersProjectPlugin
            #FontManagerPlugin \
            #ResourceLibraryPlugin \
            #MaterialManagerPlugin \
            #ImagedAnimationPlugin \

#ResourceLibraryPlugin.depends     = CorePlugin
#FontManagerPlugin.depends         = CorePlugin ResourceLibraryPlugin
#MaterialManagerPlugin.depends     = CorePlugin
#ImagedAnimationPlugin.depends     = CorePlugin
#JuiceJumpersProjectPlugin.depends = CorePlugin
