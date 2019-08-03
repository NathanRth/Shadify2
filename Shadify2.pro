TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    shader.cpp \
    mesh.cpp \
    object.cpp \
    glfwwindow.cpp \
    scene.cpp \
    abstract_event_handler.cpp \
    model.cpp \
    light.cpp \
    renderengine.cpp \
    camerafps.cpp \
    cameraorbit.cpp \
    camera.cpp

LIBS += -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl -lassimp

INCLUDEPATH += GLFW GL glm

HEADERS += \
    shader.h \
    vertex.h \
    mesh.h \
    object.h \
    glfwwindow.h \
    scene.h \
    abstract_event_handler.h \
    texture.h \
    model.h \
    stb_image/stb_image.h \
    light.h \
    renderengine.h \
    camerafps.h \
    cameraorbit.h \
    camera.h \
    env_scene.h
