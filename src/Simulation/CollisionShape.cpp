/*
 *  Copyright 2010 The Collage Project
 */

#include "CollisionShape.h"

CollisionShape2::CollisionShape2() {
}

CollisionShape2::~CollisionShape2() {
}

void CollisionShape2::createShape(
    std::vector<Ogre::Vector2> points,
    DefinedBy definedBy) {
  for (int i = 0; i < points.size(); i++) {
    this->relativePoints.push_back(points[i]);
    this->absolutePoints.push_back(points[i]);
  }
  this->definedBy = definedBy;
  calculateShapeRepresentations();
}

void CollisionShape2::translate(Ogre::Vector2 to) {
  for (int i = 0; i < absolutePoints.size(); i++) {
    absolutePoints[i] = relativePoints[i] + to;
  }
//  boundingSphere.center += dir;
}


void CollisionShape2::calculateShapeRepresentations() {
  // aabb stuff
  aabb.top = &absolutePoints[0];
  aabb.bottom = &absolutePoints[0];
  aabb.left = &absolutePoints[0];
  aabb.right = &absolutePoints[0];
  std::vector<Ogre::Vector2*> convexTestPoints;

  for (int i = 0; i < absolutePoints.size(); i++) {
    if (absolutePoints[i].y > aabb.top->y) {
      aabb.top = &absolutePoints[i];
    }
    if (absolutePoints[i].y < aabb.bottom->y) {
      aabb.bottom = &absolutePoints[i];
    }
    if (absolutePoints[i].x > aabb.right->x) {
      aabb.right = &absolutePoints[i];
    }
    if (absolutePoints[i].x < aabb.left->x) {
      aabb.left = &absolutePoints[i];
    }
    convexTestPoints.push_back(&absolutePoints[i]);
  }

  aabb.maxY = &aabb.top->y;
  aabb.minY = &aabb.bottom->y;
  aabb.maxX = &aabb.right->x;
  aabb.minX = &aabb.left->x;

//  aabb.topLeft = Ogre::Vector2(*minX, *maxY);
//  aabb.topRight = Ogre::Vector2(*maxX, *maxY);
//  aabb.bottomLeft = Ogre::Vector2(*minX, *minY);
//  aabb.bottomRight = Ogre::Vector2(*maxX, *minY);
//  aabb.aabbConvex.points.push_back(&topRight);
//  aabb.aabbConvex.points.push_back(&topLeft);
//  aabb.aabbConvex.points.push_back(&bottomLeft);
//  aabb.aabbConvex.points.push_back(&bottomRight);
//  aabb.aabbConvex.points.push_back(&topRight);
//  for (int i = 0; i <  aabbConvex.points.size()-1; i++) {
//    aabbConvex.edges.push_back(
//        *aabbConvex.points[i+1] - *aabbConvex.points[i]);
//  }

  // convex stuff
  convex.points.clear();
  if (aabb.right != aabb.top)
    quickHullRecursion(&convexTestPoints, aabb.right, aabb.top);
  if (aabb.top != aabb.left)
    quickHullRecursion(&convexTestPoints, aabb.top, aabb.left);
  if (aabb.left != aabb.bottom)
    quickHullRecursion(&convexTestPoints, aabb.left, aabb.bottom);
  if (aabb.bottom != aabb.right)
    quickHullRecursion(&convexTestPoints, aabb.bottom, aabb.right);

  convex.points.push_back(convex.points[0]);
  for (int i = 0; i < convex.points.size()-1; i++) {
    convex.edges.push_back(*(convex.points[i+1]) - *(convex.points[i]));
  }

  // linestrip stuff
  for (int i = 0; i < absolutePoints.size()-1; i++) {
    linestrip.points.push_back(&absolutePoints[i]);
    Ogre::Vector2 edge = absolutePoints[i+1] - absolutePoints[i];
    linestrip.edges.push_back(edge);
    AABB edgeAABB;
    if (edge.y < 0.0) {
      edgeAABB.top = &absolutePoints[i];
      edgeAABB.bottom = &absolutePoints[i+1];
    } else {
      edgeAABB.top = &absolutePoints[i+1];
      edgeAABB.bottom = &absolutePoints[i];
    }
    if (edge.x < 0.0) {
      edgeAABB.right = &absolutePoints[i];
      edgeAABB.left = &absolutePoints[i+1];
    } else {
      edgeAABB.right = &absolutePoints[i+1];
      edgeAABB.left = &absolutePoints[i];
    }
    edgeAABB.maxY = &edgeAABB.top->y;
    edgeAABB.minY = &edgeAABB.bottom->y;
    edgeAABB.maxX = &edgeAABB.right->x;
    edgeAABB.minX = &edgeAABB.left->x;
    linestrip.edgeAABBs.push_back(edgeAABB);
  }
  linestrip.points.push_back(&absolutePoints[absolutePoints.size()-1]);
}

void CollisionShape2::quickHullRecursion(
    std::vector<Ogre::Vector2*>* inputPoints,
    Ogre::Vector2* a,
    Ogre::Vector2* b) {

  // compute the line parameters in homogenous coordinates
  float lx = b->y - a->y;
  float ly = a->x - b->x;
  float lz = b->x * a->y - a->x * b->y;

  // discard non extrema of the Convex Hull
  float maxD = 0.0;
  Ogre::Vector2* thirdPoint = a;
  std::vector<Ogre::Vector2*> nextPoints;

  for (int i = 0; i < inputPoints->size(); i++) {
    if ((*inputPoints)[i] != a && (*inputPoints)[i] != b) {
      // compute the distance between all given points and the line
      float d = (lx * (*inputPoints)[i]->x) + (ly * (*inputPoints)[i]->y) + lz;
      // keep only the points on the right side of the line
      if (d > 0.0) {
        nextPoints.push_back((*inputPoints)[i]);
        // keep particulary the point with the highest distance
        if (d > maxD) {
          maxD = d;
          thirdPoint = nextPoints.back();
        }
      }
    }
  }

  // call the function recoursively with only the poins beyond the already
  // explored area of the shape
  if (maxD > 0.0) {
    quickHullRecursion(&nextPoints, a, thirdPoint);
    quickHullRecursion(&nextPoints, thirdPoint, b);
  } else {
    // recoursion break if no point was found beyond the given edge
    convex.points.push_back(a);
  }
}

void CollisionShape2::intitDebugRendering(
    Ogre::SceneManager* sceneManager,
    std::string name) {
  debugRendering = sceneManager->createManualObject(name);
  sceneManager->getRootSceneNode()->attachObject(debugRendering);
}

void CollisionShape2::updateDebugRendering(
    DefinedBy defBy,
    const std::string materialName) {
  debugRendering->clear();
  debugRendering->begin(
      materialName,
      Ogre::RenderOperation::OT_LINE_STRIP);

  switch (defBy) {
    case DEF_CONVEX:
      for (int i = 0; i < convex.points.size(); i++) {
        debugRendering->position(convex.points[i]->x, convex.points[i]->y, 0.0);
      }
      break;
    case DEF_AABB:
      debugRendering->position(*aabb.maxX, *aabb.maxY, 0.0);
      debugRendering->position(*aabb.minX, *aabb.maxY, 0.0);
      debugRendering->position(*aabb.minX, *aabb.minY, 0.0);
      debugRendering->position(*aabb.maxX, *aabb.minY, 0.0);
      debugRendering->position(*aabb.maxX, *aabb.maxY, 0.0);
      break;
    case DEF_LINESTRIP:
      for (int i = 0; i < linestrip.points.size(); i++) {
        debugRendering->position(
            linestrip.points[i]->x, linestrip.points[i]->y, 0.0);
      }
  }


  debugRendering->end();
}
