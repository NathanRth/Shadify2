#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>

#include "shader.h"
#include "mesh.h"

/**
 * @brief TextureFromFile
 * @param path
 * @param directory
 * @return
 */
unsigned int TextureFromFile(const char *path, const std::string &directory);

/**
 * @brief The Model class
 * Un model est un objet 3D composé d'un ou plusieurs mesh et d'une ou plusieurs textures.
 * Un model est importé grâce à la librairie Assimp (les images grâce à stb).
 */
class Model : public Object
{
public:
    /**
      * @brief Constructeur de la class Model
      * @param path Le chemin du fichier 3D
      * @param p La position de l'objet à l'instnciation
      * @param c La couleur par défaut de l'objet s'il n'a pas de texture
      * ou qu'une erreur est survenue au chargement.
      */
    Model(const char *path,const glm::vec3 &p, const glm::vec3 &c = {1.0,1.0,1.0});

    /**
     * @brief Lance le rendu du model avec \a shader . Cette fonction ne doit être appellé que lors
     * du pass de géométrie.
     * @param shader Le shader à utiliser pour le rendu
     */
    void draw(Shader shader);

    /**
     * @return La couleur par défaut de l'objet
     */
    glm::vec3 albedo() {return m_albedo;}
private:
    std::vector<Mesh> m_meshes;
    std::string directory;
    std::vector<Texture> m_texturesLoaded;

    /**
     * @brief Charge le model dans l'importer de Assimp.
     * @param path Le chemin du fichier à importer/parser
     */
    void loadModel(std::string path);

    /**
     * @brief Assimp specific function
     * @param node
     * @param scene
     */
    void processNode(aiNode *node, const aiScene *scene);

    /**
     * @brief Assimp specific function
     * @param mesh
     * @param scene
     * @return
     */
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    /**
     * @brief Assimp specific function
     * @param mat
     * @param type
     * @param typeName
     * @return
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);

    glm::vec3 m_albedo; /**< La couleur par défaut de l'objet */

};

#endif // MODEL_H
