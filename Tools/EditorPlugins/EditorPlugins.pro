TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += CorePlugin \
           WorkspacePlugin \
           ResourceLibraryPlugin \
           JuiceJumpersProjectPlugin

            #FontManagerPlugin \
            #MaterialManagerPlugin \
            #ImagedAnimationPlugin \

#ResourceLibraryPlugin.depends     = CorePlugin
#FontManagerPlugin.depends         = CorePlugin ResourceLibraryPlugin
#MaterialManagerPlugin.depends     = CorePlugin
#ImagedAnimationPlugin.depends     = CorePlugin
#JuiceJumpersProjectPlugin.depends = CorePlugin
