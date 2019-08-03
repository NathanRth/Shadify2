#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "scene.h"
#include "abstract_event_handler.h"
#include "renderengine.h"

#include <GLFW/glfw3.h>

/**
 * @brief The GlfwWindow class
 */
class GlfwWindow : public AbstractEventHandler
{
public:
    GlfwWindow();

    int init();

    /**
     * @brief Initialise une scène 3D.
     * L'utilisateur peut définir ici quel objet et quel éclairage il souhaite intégrer dans la scène.
     * \c<d d d >
     */
    void setupScene();

    /**
     * @brief Lance la boucle principale de la fenêtre. Ne retourne que lorsque la fenêtre ferme.
     * On initialise d'abord tout les frame buffers, puis on bind les samplers aux shader.
     * La boucle active tout les pass un à un.
     * @see {#mainpage}
     */
    void show();
    GLFWwindow* window() {return m_window;}

    virtual void keyCallback(GLFWwindow *window,int key,int scancode,int action,int mods);
    virtual void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    virtual void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    virtual void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    virtual void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

private:
    GLFWwindow* m_window;
    int m_frameBufferWidth, m_frameBufferHeight;
    Scene *m_scene;

    double prevXPos, prevYPos;

    int m_renderLayer, m_ssdoMode;
    float m_radius;
    bool m_useSkybox;

    // timing
    float m_deltaTime;
    float m_lastFrame;
    float m_startTime;
    float m_stopTime;

    unsigned int m_frames; /**< Nombre de frames écoulées depuis le début du programme */

    std::string m_shaderDir; /**< Chemin du dossier où sont enregistrés les shaders */
    std::string m_resourcesDir; /**< Chemin du dossier où sont enregistrés les modèles 3D (et leurs textures) */

};

#endif // GLFWWINDOW_H
