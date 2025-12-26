add_subdirectory(LCommon)
set_property(TARGET LCommon PROPERTY FOLDER "Core")

add_subdirectory(LBase)
set_property(TARGET LBase PROPERTY FOLDER "Core") 

add_subdirectory(PluginManager)
set_property(TARGET PluginManager PROPERTY FOLDER "Core")