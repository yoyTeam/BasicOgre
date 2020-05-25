QT += core \
        gui

DEFINES += USE_QT_WINDOW

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testYoy
TEMPLATE = app

QMAKE_CXXFLAGS += \
    -O3 -DNDEBUG

SOURCES += \
    MainWindow.cpp \
    OgreWidget.cpp \
    main.cpp \
    yEngine.cpp

HEADERS += \
    MainWindow.h \
    OgreWidget.h \
    yEngine.h

INCLUDEPATH += \
    $$PWD/ \
    $$PWD/../../yoyEngine/Dependencies/ \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/ \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Animation \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus/GL \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus/GLSL \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus/windowing/GLX \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus/windowing/gtk \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/GL3Plus/windowing/SDL \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/Direct3D11 \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/RenderSystems/Direct3D11/Vao \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Hlms/Common \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Hlms/Unlit \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Hlms/Pbs \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Hlms/Pbs/Cubemaps \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Hlms/Pbs/InstantRadiosity \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/Overlay \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/MeshLodGenerator \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/PlanarReflections \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/SceneFormat \
    $$PWD/../../yoyEngine/Dependencies/Ogre/include/OGRE/PlugIns/ParticleFX


LIBS += \
    user32.lib \ #needed by D3D11 renderer
    gdi32.lib \ #needed by D3D11 renderer
    D3D11.lib \ #needed by D3D11 renderer
    dxgi.lib \
    dxguid.lib \ #needed by D3D11 renderer
    gdiplus.lib \
    opengl32.lib \
    ws2_32.lib
CONFIG(debug, debug|release) {
LIBS += \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreOverlayStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreHlmsPbsStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreHlmsUnlitStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/opt/RenderSystem_GL3PlusStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/opt/RenderSystem_Direct3D11Static_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreMainStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreMeshLodGeneratorStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgrePlanarReflectionsStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/opt/Plugin_ParticleFXStatic_d.lib \
#    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/OgreSceneFormatStatic_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/zlib_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/zziplib_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/FreeImage_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/freetype_d.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Debug/Remotery_d.lib
} else {
LIBS += \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreOverlayStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreHlmsPbsStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreHlmsUnlitStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/opt/RenderSystem_GL3PlusStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/opt/RenderSystem_Direct3D11Static.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreMainStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreMeshLodGeneratorStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgrePlanarReflectionsStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/opt/Plugin_ParticleFXStatic.lib \
#    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/MinSizeRel/OgreSceneFormatStatic.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Release/zlib.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Release/zziplib.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Release/FreeImage.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Release/freetype.lib \
    $$PWD/../../yoyEngine/Dependencies/Ogre/lib/Release/Remotery.lib
}

FORMS += \
    MainWindow.ui
