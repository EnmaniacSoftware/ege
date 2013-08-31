TEMPLATE = subdirs

SUBDIRS +=  CorePlugin \
            FontManagerPlugin \
            ResourceLibraryPlugin \
            MaterialManagerPlugin \
            ImagedAnimationPlugin \
            JuiceJumpersProjectPlugin

ResourceLibraryPlugin.depends     = CorePlugin
FontManagerPlugin.depends         = CorePlugin ResourceLibraryPlugin
MaterialManagerPlugin.depends     = CorePlugin
ImagedAnimationPlugin.depends     = CorePlugin
JuiceJumpersProjectPlugin.depends = CorePlugin
