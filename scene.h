#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "model.h"
#include "mesh.h"
#include "cameraorbit.h"
#include "light.h"

/**
 * @brief Une scène sert à décrire un environement 3D.
 * Elle contient les différents objets et lumières présents
 * dans le monde 3D.
 */
class Scene
{
public:
    Scene();

    CameraOrbit* camera() {return m_camera;}

    /**
     * @brief Ajouter un objet 3D dans la scène (uniquement depuis un fichier externe, .obj par exemple).
     * @param Le model à ajouter \see Model
     */
    void addModel(const Model &model);

    /**
     * @brief Ajouter une lumière dans la scène.
     * @param light La lumière à ajouter. La lumière doit être de type \c PointLight* ou \c DirectionalLight*
     */
    void addLight(Light* light);

    /**
     * @brief Supprimer un objet 3D
     * @param Index de l'objet dans la liste
     */
    void removeModel(unsigned int index);

    /**
     * @brief Supprimer une lumière
     * @param Index de la lumière dans la liste
     */
    void removeLight(unsigned int index);

    std::vector<Model>* models() {return &m_models;} /**< Renvoi la liste des objets de la scène */
    std::vector<Light*>* lights() {return &m_lights;} /**< Renvoi la liste des lumière de la scène */

private:
    std::vector<Model> m_models; /**< Liste des model de la scène */
    std::vector<Light*> m_lights; /**< Liste des lumière de la scène. Exprimée sous la forme
                                    d'une liste de pointeur sur la classe abstraite Light */
    CameraOrbit *m_camera; /**< La camera de la scène */
};

#endif // SCENE_H
