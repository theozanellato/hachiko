cmake_minimum_required(VERSION 3.28)
project(Hachiko VERSION 0.0.1)

# export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

#FetchContent_Declare(juce
#GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
#        GIT_TAG        7.0.12
#        GIT_SHALLOW    TRUE)
#FetchContent_MakeAvailable(juce)

FetchContent_Declare(tracktion_engine
        GIT_REPOSITORY  https://github.com/Tracktion/tracktion_engine
        GIT_TAG         682bee39e8c9290758fde396f12065cb1aca4437)
FetchContent_MakeAvailable(tracktion_engine)

FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        527b2c45af2f8964f95826bd16ab7c7ed372ae41)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(imgui-knobs
        GIT_REPOSITORY https://github.com/altschuler/imgui-knobs
        GIT_TAG        4f207526f9ef036a0aff7edfaad92cfbe12d987a)
FetchContent_MakeAvailable(imgui-knobs)

FetchContent_Declare(ImGuiFileDialog
       GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog
        GIT_TAG        ddccdd60db331deb6ec2b5df8a080ead8dbfbd47)
FetchContent_MakeAvailable(ImGuiFileDialog)

FetchContent_Declare(imgui_juce
        GIT_REPOSITORY https://github.com/Krasjet/imgui_juce
        GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(imgui_juce)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui-knobs_SOURCE_DIR}/imgui-knobs.cpp
        ${ImGuiFileDialog_SOURCE_DIR}/ImGuiFileDialog.cpp
)
target_include_directories(imgui
        PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${imgui-knobs_SOURCE_DIR}
        ${ImGuiFileDialog_SOURCE_DIR}
)

juce_add_gui_app(Hachiko
        COMPANY_NAME zanellato
        PRODUCT_NAME "hachiko")

target_sources(Hachiko
        PRIVATE
        src/Main.cpp
        src/View.cpp
        src/Mouse.cpp
)
target_include_directories(Hachiko
        PRIVATE
        src
)

juce_generate_juce_header(Hachiko)
target_compile_definitions(Hachiko
        PUBLIC
        JUCE_PLUGINHOST_AU=1
        JUCE_PLUGINHOST_LADSPA=1
        JUCE_PLUGINHOST_VST3=1
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
        JUCER_ENABLE_GPL_MODE=1
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_STRICT_REFCOUNTEDPOINTER=1
        TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH=1
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Hachiko,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Hachiko,JUCE_VERSION>")

target_link_libraries(Hachiko
        PRIVATE
        juce::juce_opengl
        juce::juce_audio_devices
        juce::juce_audio_processors
        juce::juce_audio_utils
        imgui
        imgui_impl_juce
        tracktion::tracktion_core
        tracktion::tracktion_engine
        tracktion::tracktion_graph
        atomic
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ALSA REQUIRED)

#set(SOURCES
#        main.cpp
#)

#set(IMGUI_SOURCES
#        ui/imgui/imgui.cpp
#        ui/imgui/imgui_demo.cpp
#        ui/imgui/imgui_draw.cpp
#        ui/imgui/imgui_widgets.cpp
#        ui/imgui/imgui_tables.cpp
#        ui/imgui/backends/imgui_impl_glfw.cpp
#        ui/imgui/backends/imgui_impl_glfw.h
#        ui/imgui/backends/imgui_impl_opengl3.cpp
#        ui/imgui/backends/imgui_impl_opengl3.h
#        ui/imgui/misc/cpp/imgui_stdlib.cpp
#        ui/imgui/misc/cpp/imgui_stdlib.h
#        ui/imgui-knobs/imgui-knobs.cpp
#)

#add_executable(hachiko ${SOURCES} ${IMGUI_SOURCES})

#target_include_directories(hachiko PRIVATE
#        ui/imgui
#        ui/imgui/backends
#        ui/imgui/misc/cpp
#        ui/imgui-knobs
#        ${ALSA_INCLUDE_DIRS}
#)

#target_link_libraries(
#        hachiko
#        PRIVATE
#        freetype
#        glfw
#        ${OPENGL_LIBRARIES}
#        ${ALSA_LIBRARIES}
#)
