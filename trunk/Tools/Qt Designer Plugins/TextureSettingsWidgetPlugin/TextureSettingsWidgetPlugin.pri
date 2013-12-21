HEADERS     = Implementation\TextureSettingsWidget.h \
              Interface\TextureSettingsWidgetPlugin.h

SOURCES     = Implementation\TextureSettingsWidget.cpp \
              Implementation\TextureSettingsWidgetPlugin.cpp

FORMS +=      Forms/widget.ui

Debug:UI_DIR   = debug/.ui
Release:UI_DIR = release/.ui
