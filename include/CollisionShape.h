/*
 *  Copyright 2010 The Collage Project
 */

#ifndef COLLISIONSHAPE_H_
#define COLLISIONSHAPE_H_

#include "Ogre.h"

class CollisionShape2 {
  friend class CollisionHandler;
 public:
  CollisionShape2();
  virtual ~CollisionShape2();

  enum DefinedBy {
    DEF_POINTS,
    DEF_CONVEX,
    DEF_AABB,
    DEF_BOUNDINGSPHERE
  };

//  struct Polygon{
//    std::vector<Ogre::Vector2> points;
//    std::vector<Ogre::Vector2> edges;
//  };

  struct Convex {
    std::vector<Ogre::Vector2*> points;
    std::vector<Ogre::Vector2> edges;
  };

  struct AABB {
    Ogre::Vector2 *top, *bottom, *left, *right;
    float *minX, *maxX, *minY, *maxY;
  };

//  struct BoundingSphere {
//    Ogre::Vector2 center;
//    float radius;
//  };

  void createShape(std::vector<Ogre::Vector2> points, DefinedBy definedBy);
  void translate(Ogre::Vector2 to);

  void intitDebugRendering(Ogre::SceneManager* sceneManager, std::string name);
  void updateDebugRendering(
      DefinedBy defBy = DEF_CONVEX,
      const std::string materialName = "BaseWhiteNoLighting");

// private:
  DefinedBy definedBy;
  std::vector<Ogre::Vector2> relativePoints;
  std::vector<Ogre::Vector2> absolutePoints;
//  Polygon polygon;
  Convex convex;
  AABB aabb;
//  BoundingSphere boundingSphere;

  void calculateShapeRepresentations();
  void quickHullRecursion(
      std::vector<Ogre::Vector2*>* inputPoints,
      Ogre::Vector2* a,
      Ogre::Vector2* b);

  Ogre::ManualObject* debugRendering;
};

#endif /* COLLISIONSHAPE_H_ */
