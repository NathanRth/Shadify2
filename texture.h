#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

/** @brief Structure de description de texture
  * @typedef Texture
  */
typedef struct s_texture
{
    unsigned int id; /**< Index mémoire OpenGL de la texture. Valeur générée lors de l'importation. */
    std::string type; /**< Type de la texture (diffuse, specular, normal, bump...). Valeur donnée par Assimp lors de l'importation. */
    std::string path; /**< Chemin de la texture */
} Texture;

#endif // TEXTURE_H
