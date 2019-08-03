#include "glfwwindow.h"
#include "model.h"
#include <iostream>
#include <random>
#include <glm/gtx/string_cast.hpp>

#ifndef STB_INCLUDED
    #define STB_INCLUDED
    #ifndef STB_IMAGE_IMPLEMENTATION
        #define STB_IMAGE_IMPLEMENTATION
    #endif
    #include "stb_image/stb_image.h"
#endif

#define SCR_WIDTH   1280
#define SCR_HEIGHT  720

#define WHITE   {1,1,1}
#define RED     {1,0,0}
#define BLUE    {0,0,1}
#define GREEN   {0,1,0}
#define CYAN    {0,1,1}
#define MAGENTA {1,0,1}
#define YELLOW  {1,1,0}
#define GREY    {0.4,0.4,0.4}

#define SC_SPONZA

GlfwWindow::GlfwWindow() :
    m_window(0)
{
    m_scene = new Scene;
    m_useSkybox = false;
    m_renderLayer = 5;
    m_ssdoMode = 0;
    m_radius = 2.0;
    m_deltaTime = 0;
    m_lastFrame = 0;
    m_shaderDir = "../Shadify2_save/shaders/";
    m_resourcesDir = "../Shadify2_save/resources/";
}

int GlfwWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shadify 2.0", NULL, NULL);

    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);

    //callback functions for input handling
    glfwSetFramebufferSizeCallback(m_window, AbstractEventHandler::framebufferSizeCallback_dispatch);
    glfwSetKeyCallback(m_window, AbstractEventHandler::keyCallback_dispatch);
    glfwSetScrollCallback(m_window, AbstractEventHandler::scrollCallback_dispatch);
    glfwSetMouseButtonCallback(m_window, AbstractEventHandler::mouseButtonCallback_dispatch);
    glfwSetCursorPosCallback(m_window, AbstractEventHandler::cursorPosCallback_dispacth);
    //glFrontFace(GL_CW);
    //glCullFace(GL_FRONT_AND_BACK);
    glEnable(GL_DEPTH_TEST);
}

void GlfwWindow::setupScene()
{
#ifdef SC_SPONZA
    m_scene->addModel(Model("../Shadify2/models/sponza/sponza.obj",{0,0,0},WHITE));
    // Light( color, position, [intensity], [direction])
    m_scene->addLight(new PointLight({1,0.2,0.8},   {-1.0,2,3.0}, 1.0));
    m_scene->addLight(new PointLight({0.2,1,0.2}, {-1.0,2,0.0}, 1.0));
    m_scene->addLight(new PointLight({0,0.2,1},   {-1.0,2,-3.0}, 1.0));

    m_scene->camera()->setFormat(SCR_WIDTH, SCR_HEIGHT);
#endif
#ifdef SC_TEST
    m_scene->addModel(Model("../Shadify2/models/plane.obj",{0,0,0},WHITE));
    m_scene->addModel(Model("../Shadify2/models/cube_untex.obj",{4,1,0},RED));
    m_scene->addModel(Model("../Shadify2/models/cube_untex.obj",{-4,1,0},CYAN));
    m_scene->addModel(Model("../Shadify2/models/cube_untex.obj",{0,1,4},YELLOW));
    m_scene->addModel(Model("../Shadify2/models/cube_untex.obj",{0,1,-4},GREY));
    //m_scene->addModel(Model("../Shadify2/models/living_room.obj",{0,0,0},WHITE));

    m_scene->models()->at(0).scale(20.0);

    m_scene->addLight(new PointLight(WHITE,{0,3,0}, 1.0));
#endif
}

void GlfwWindow::show()
{

    std::cout<<SCR_WIDTH<<" "<<SCR_HEIGHT<<std::endl;

    // gBuffer initialisation

    unsigned int gBuffer;
    glGenFramebuffers(1,&gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);

    unsigned int gPosition, gAlbedo, gNormal, gDepth,gSpecular;

    glGenTextures(1,&gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,gPosition, 0);

    glGenTextures(1,&gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,gNormal, 0);

    glGenTextures(1,&gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,gAlbedo, 0);

    glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDepth, 0);

    glGenTextures(1, &gSpecular);
    glBindTexture(GL_TEXTURE_2D, gSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gSpecular, 0);

    unsigned int attachments[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};

    glDrawBuffers(5, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"Framebuffer not complete !"<<std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // frame buffers initialisation

    unsigned int lightingFBO, ssaoFBO, ssaoBlurFBO, ssdoFBO, ssdoBlurFBO, finalFBO;
    glGenFramebuffers(1, &lightingFBO);
    glGenFramebuffers(1, &ssdoFBO);
    glGenFramebuffers(1, &ssdoBlurFBO);
    glGenFramebuffers(1, &finalFBO);
    glGenFramebuffers(1, &ssaoFBO);
    glGenFramebuffers(1, &ssaoBlurFBO);

    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    unsigned int ssdoColorBuffer, ssdoColorBufferBlur;
    unsigned int lightingBuffer, finalBuffer;

    // lighting
    glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO);
    glGenTextures(1, &lightingBuffer);
    glBindTexture(GL_TEXTURE_2D, lightingBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingBuffer, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSDO Framebuffer not complete !"<<std::endl;

    // ssdo
    glBindFramebuffer(GL_FRAMEBUFFER, ssdoFBO);
    glGenTextures(1, &ssdoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssdoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssdoColorBuffer, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSDO Framebuffer not complete !"<<std::endl;

    // ssdo blur
    glBindFramebuffer(GL_FRAMEBUFFER, ssdoBlurFBO);
    glGenTextures(1, &ssdoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssdoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssdoColorBufferBlur, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSDO Blur Framebuffer not complete !"<<std::endl;

    // ssao
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSAO Framebuffer not complete !"<<std::endl;

    // ssao blur

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSAO Blur Framebuffer not complete !"<<std::endl;

    // final layering buffer

    glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
    glGenTextures(1, &finalBuffer);
    glBindTexture(GL_TEXTURE_2D, finalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalBuffer, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr<<"SSDO Blur Framebuffer not complete !"<<std::endl;


    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    auto noiseTexture = RenderEngine::genNoiseTexture(4);
    auto kernel = RenderEngine::genSampleKernel(64);

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    GLuint skyboxTexture = RenderEngine::loadCubemap(m_resourcesDir + "textures/skybox/");

    // Shader intialization

    // shader paths

    Shader shaderGeometryPass(m_shaderDir+"gbuffer/gbuffer.vs",
                              m_shaderDir+"gbuffer/gbuffer.fs");

    Shader shaderLightingPass(m_shaderDir+"common/vert_common.vs",
                              m_shaderDir+"common/phong_lighting.fs");

    Shader shaderSSDO(m_shaderDir+"common/vert_common.vs",
                      m_shaderDir+"ssdo/ssdo.fs");

    Shader shaderBlur(m_shaderDir+"common/vert_common.vs",
                      m_shaderDir+"common/simple_blur.fs");

    Shader shaderSSAO(m_shaderDir+"common/vert_common.vs",
                      m_shaderDir+"ssao/ssao.fs");

    Shader shaderFinal(m_shaderDir+"common/vert_common.vs",
                       m_shaderDir+"common/render_layers.fs");

    Shader shaderSkybox(m_shaderDir+"skybox/skybox.vs",
                        m_shaderDir+"skybox/skybox.fs");

    // uniform binding

    shaderLightingPass.bind();
    shaderLightingPass.setSamplers({"gPosition","gNormal", "gAlbedo","gSpecular"});

    shaderSSDO.bind();
    shaderSSDO.setSamplers({"gPosition","gNormal","gAlbedo",
                            "texLighting","texNoise","skybox"});

    shaderBlur.bind();
    shaderBlur.setSamplers({"samplerInput"});

    shaderSSAO.bind();
    shaderSSAO.setSamplers({"gPosition","gNormal","texNoise"});

    shaderFinal.bind();
    shaderFinal.setSamplers({"gPositionDepth","gNormal","gAlbedo","gDepth",
                             "gSpecular","texLighting","texSSDO","texSSAO"});

    shaderSkybox.bind();
    shaderSkybox.setSamplers({"skybox","final","gDepth"});

    // main loop

    while (!glfwWindowShouldClose(m_window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        //m_scene->lights()->at(0).position(glm::vec3(cos(currentFrame)*3.0,2,0));
        //std::cout<<glm::to_string(m_scene->lights()->at(0).position())<<std::endl;

        m_scene->camera()->setFormat(SCR_HEIGHT, SCR_WIDTH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


// ######################## GEOMETRY PASS #######################################

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderGeometryPass.bind();
        shaderGeometryPass.setUniformMat4("projection", m_scene->camera()->projectionMatrix());
        shaderGeometryPass.setUniformMat4("view", m_scene->camera()->viewMatrix());
        shaderGeometryPass.setUniformVec3("viewPos", m_scene->camera()->position());


        for(std::vector<Model>::iterator model = m_scene->models()->begin(); model != m_scene->models()->end(); model++)
        {
            shaderGeometryPass.setUniformMat4("model", model->modelMatrix());

            model->draw(shaderGeometryPass);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


//######################### LIGHTING PASS ######################################

        if(m_renderLayer == 2 || m_renderLayer == 4 || m_renderLayer == 5 || m_renderLayer == 6)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shaderLightingPass.bind();
            int i = 0;
            // for each light
            for(Light* light: *(m_scene->lights())){
                //shaderLightingPass.setUniformVec3("lights["+std::to_string(i)+"].Position", glm::mat3(m_scene->camera()->viewMatrix()) * light->position());
                shaderLightingPass.setUniformVec3("lights["+std::to_string(i)+"].Position", light->position());
                shaderLightingPass.setUniformVec3("lights["+std::to_string(i)+"].Color", light->color);
                shaderLightingPass.setUniformFloat("lights["+std::to_string(i)+"].Linear",0.01);
                shaderLightingPass.setUniformFloat("lights["+std::to_string(i)+"].Quadratic",0.012);
                shaderLightingPass.setUniformFloat("lights["+std::to_string(i)+"].Intensity",light->intensity);
                shaderLightingPass.setUniformInt("lights["+std::to_string(i)+"].Type",light->type);
                shaderLightingPass.setUniformVec3("lights["+std::to_string(i)+"].Direction",glm::mat3(m_scene->camera()->viewMatrix()) * light->direction);
                i++;
            }
            shaderLightingPass.setUniformMat4("view",m_scene->camera()->viewMatrix());
            shaderLightingPass.setUniformVec3("viewPos",m_scene->camera()->position());
            shaderLightingPass.setUniformInt("nbLights", m_scene->lights()->size());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gAlbedo);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, gSpecular);

            RenderEngine::renderBuffer();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }


// ########################### SSDO #############################################

        /* Le SSDO prend en entrée la position, les normales et l'albedo du gBuffer.
         * Il prend aussi le buffer d'éclairage (pour le scond pass de transport de lumière),
         * une noise texture pour randomiser l'orientation des kernel
         * et la skybox pour l'utilisation classique du SSDO.
         */

        if(m_renderLayer == 5 || m_renderLayer == 6)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, ssdoFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            shaderSSDO.bind();
            //send kernel + rotation
            for(unsigned int i = 0; i < kernel.size(); ++i)
                shaderSSDO.setUniformVec3("samples["+std::to_string(i)+"]",kernel[i]);

            shaderSSDO.setUniformMat4("projection",m_scene->camera()->projectionMatrix());
            shaderSSDO.setUniformMat4("view",m_scene->camera()->viewMatrix());
            shaderSSDO.setUniformMat4("iview",glm::inverse(m_scene->camera()->viewMatrix()));
            shaderSSDO.setUniformInt("mode", m_ssdoMode);
            shaderSSDO.setUniformFloat("radius", m_radius);
            shaderSSDO.setUniformVec3("viewPos", m_scene->camera()->position());
            shaderSSDO.setUniformInt("kernelSize", kernel.size());
            shaderSSDO.setUniformInt("nbLights", m_scene->lights()->size());
            shaderSSDO.setUniformBool("useSkybox", m_useSkybox);

            int i = 0;
            for(Light* light: *(m_scene->lights())){
                //std::cout<<"light["<<i<<"]:"<<glm::to_string(light->position())<<std::endl;
                //shaderSSDO.setUniformVec3("lights["+std::to_string(i)+"].Position", glm::mat3(m_scene->camera()->viewMatrix()) * light->position());
                shaderSSDO.setUniformVec3("lights["+std::to_string(i)+"].Position", light->position());
                shaderSSDO.setUniformVec3("lights["+std::to_string(i)+"].Color", light->color);
                shaderSSDO.setUniformFloat("lights["+std::to_string(i)+"].Linear",0.01);
                shaderSSDO.setUniformFloat("lights["+std::to_string(i)+"].Quadratic",0.012);
                shaderSSDO.setUniformFloat("lights["+std::to_string(i)+"].Intensity",light->intensity);
                shaderSSDO.setUniformInt("lights["+std::to_string(i)+"].Type",light->type);
                shaderSSDO.setUniformVec3("lights["+std::to_string(i)+"].Direction",glm::mat3(m_scene->camera()->viewMatrix()) * light->direction);
                i++;
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gAlbedo);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, lightingBuffer);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, noiseTexture);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

            RenderEngine::renderBuffer();

            glBindFramebuffer(GL_FRAMEBUFFER,0);

            // 3 - blur ssdo texture to remove noise artifacts

            glBindFramebuffer(GL_FRAMEBUFFER, ssdoBlurFBO); // output FBO
            glClear(GL_COLOR_BUFFER_BIT);
            shaderBlur.bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,ssdoColorBuffer); // input texture

            RenderEngine::renderBuffer();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }


// ############################ SSAO ###################################

        /* Implémentation d'un SSAO classique pour comparer avec le SSDO */

        if(m_renderLayer == 3 || m_renderLayer == 4)
        {
            // generate ssao texture
            glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            shaderSSAO.bind();

            //send kernel + rotation
            for(unsigned int i = 0; i < kernel.size(); ++i)
                shaderSSAO.setUniformVec3("samples["+std::to_string(i)+"]",kernel[i]);

            shaderSSAO.setUniformMat4("projection",m_scene->camera()->projectionMatrix());
            shaderSSAO.setUniformMat4("view",m_scene->camera()->viewMatrix());

            glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, noiseTexture);

            RenderEngine::renderBuffer();

            glBindFramebuffer(GL_FRAMEBUFFER,0);

            // 3 - blur ssao texture to remove noise artifacts
            glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO); // output FBO
            glClear(GL_COLOR_BUFFER_BIT);
            shaderBlur.bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,ssaoColorBuffer); // input texture
            RenderEngine::renderBuffer();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }


// ########################### RENDER LAYER ######################################

        /* Shader de debug.
         *
         * Il permet de switcher entre les différentes configurations possibles avec le SSAO et le SSDO.
         * En utilisant le programme, on switch entre les calques 1 à 7 (rangé numérique sur le clavier)
         * 1. gBuffer (à choisir dans le shader)
         * 2. Eclairage seul
         * 3. SSAO seul
         * 4. SSAO + éclairage
         * 5. SSDO seul
         * 6. SSDO + éclairage
         * 7. autre (à choisir dans le shader)
         *
         * Il est aussi possbible de changer le mode d'affichage du SSDO:
         * 8. direct pass
         * 9. indirect pass
         * 0. direct + indirect
         *
         */
        glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderFinal.bind();
        shaderFinal.setUniformInt("draw_mode",m_renderLayer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gDepth);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gSpecular);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, lightingBuffer);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, ssdoColorBufferBlur);

        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);



        RenderEngine::renderBuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

// ############################ SKYBOX RENDERING #################################

        /* On affiche enfin la skybox, en dernier
         * Ce shader prend en entrée la sortie du shader de debug,
         * si on n'affiche pas la skybox, ce shader n'a alors aucun effet et se contente
         * d'afficher le rendu final
         */

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shaderSkybox.bind();
        glm::mat4 view = glm::mat4(glm::mat3(m_scene->camera()->viewMatrix())); // On retire la composante de translation en passant de mat4->mat3->mat4
        shaderSkybox.setUniformMat4("view", view);
        shaderSkybox.setUniformMat4("projection", m_scene->camera()->projectionMatrix());
        shaderSkybox.setUniformBool("useSkybox", m_useSkybox);


        glBindVertexArray(skyboxVAO);// skybox cube
        glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, finalBuffer); // sortie du shader de debug
        glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gDepth);

        glDrawArrays(GL_TRIANGLES, 0, 36); // rendu de la skybox
        glBindVertexArray(0);

        glDepthFunc(GL_LESS); // set depth function back to default

// #################################################################################

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(m_window);

        glfwPollEvents();
        m_frames++;
    }
    glfwTerminate();
}


// gère les entrées du clavier
// /!\ Les macros des touches correspondent à la disposition qwerty
void GlfwWindow::keyCallback(GLFWwindow *window,int key,int scancode,int action,int mods)
{
    //exit
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // activate skybox
    if(key == GLFW_KEY_B && action == GLFW_PRESS)
        m_useSkybox = !m_useSkybox;

    // start fps counter (W)
    if(key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        m_frames = 0;
        m_startTime = glfwGetTime();
        std::cout<<"FPS counter started, press X to end."<<std::endl;
    }

    // end fps counter (X)
    if(key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        m_stopTime = glfwGetTime();
        float total = m_stopTime - m_startTime;
        std::cout<<"Average framerate for the last "<<total<<" seconds: "<<float(m_frames)/total<<std::endl;
    }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        m_renderLayer = 1;
        std::cout<<"albedo"<<std::endl;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        m_renderLayer = 2;
        std::cout<<"lighting"<<std::endl;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS){
        m_renderLayer = 3;
        std::cout<<"ssao"<<std::endl;
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS){
        m_renderLayer = 4;
        std::cout<<"ssao + lighting"<<std::endl;
    }
    if(key == GLFW_KEY_5 && action == GLFW_PRESS){
        m_renderLayer = 5;
        std::cout<<"ssdo"<<std::endl;
    }
    if(key == GLFW_KEY_6 && action == GLFW_PRESS){
        m_renderLayer = 6;
        std::cout<<"ssdo + lighting"<<std::endl;
    }
    if(key == GLFW_KEY_7 && action == GLFW_PRESS){
        m_renderLayer = 7;
    }
    if(key == GLFW_KEY_8 && action == GLFW_PRESS){
        m_ssdoMode = 0;
    }
    if(key == GLFW_KEY_9 && action == GLFW_PRESS){
        m_ssdoMode = 1;
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS){
        m_ssdoMode = 2;
    }
    if(key == GLFW_KEY_K && action == GLFW_PRESS){
        m_radius += 0.5;
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        if(m_radius > 0) m_radius -= 0.5;
    }
}

void GlfwWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_1)
        glfwGetCursorPos(m_window, &prevXPos, &prevYPos);
}

void GlfwWindow::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    m_scene->camera()->setDist(yoffset);
}

void GlfwWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GlfwWindow::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    double dx = xpos - prevXPos;
    double dy = ypos - prevYPos;
    if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        // pan view if left shift
        if(glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            m_scene->camera()->translateBy(dx,dy);
        }
        // rotate view
        else
        {
            m_scene->camera()->rotateBy(dx, dy);
        }
    }
    prevXPos = xpos;
    prevYPos = ypos;
}
