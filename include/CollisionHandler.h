/*
 *  Copyright 2010 The Collage Project
 */

#ifndef COLLISIONHANDLER_H_
#define COLLISIONHANDLER_H_

#include "Ogre.h"
#include "CollisionShape.h"

class CollisionHandler {
  friend class CollisionShape;
 public:
  CollisionHandler();
  virtual ~CollisionHandler();

  static inline bool collisionTest(
      CollisionShape2 *a,
      CollisionShape2 *b,
      Ogre::Vector2 *relativeTranslation,
      bool constrainTranslation = true) {
    if (!collisionTestAABB(a, b, relativeTranslation, false))
      return false;
// TODO(Gerrit) add further complexity levels
    return true;
  }

  static inline bool collisionTestAABB(
      CollisionShape2 *a,
      CollisionShape2 *b,
      Ogre::Vector2 *relativeTranslation,
      bool constrainTranslation = true) {
    float distLeft, distRight, distTop, distBottom;
    if (relativeTranslation->x < 0.0) {
      distLeft = *a->aabb.minX + relativeTranslation->x - *b->aabb.maxX;
      distRight = *a->aabb.maxX - *b->aabb.minX;
    } else {
      distLeft = *a->aabb.minX - *b->aabb.maxX;
      distRight = *a->aabb.maxX + relativeTranslation->x - *b->aabb.minX;
    }
    if (relativeTranslation->y < 0.0) {
      distTop = *a->aabb.minY + relativeTranslation->y - *b->aabb.maxY;
      distBottom = *a->aabb.maxY - *b->aabb.minY;
    } else {
      distTop = *a->aabb.minY - *b->aabb.maxY;
      distBottom = *a->aabb.maxY + relativeTranslation->y - *b->aabb.minY;
    }
//    float distLeft, distRight, distTop, distBottom;
//      distLeft = *a->aabb.minX + relativeTranslation->x - *b->aabb.maxX;
//      distRight = *a->aabb.maxX + relativeTranslation->x - *b->aabb.minX;
//      distTop = *a->aabb.minY + relativeTranslation->x - *b->aabb.maxY;
//      distBottom = *a->aabb.maxY + relativeTranslation->y - *b->aabb.minY;
    if ((distLeft > 0.0 || distRight < 0.0)
        || (distTop > 0.0 || distBottom < 0.0)) {
      return false;
    } else {
      if (constrainTranslation) {
        if (relativeTranslation->x < 0.0) {
          if (relativeTranslation->x < distLeft)
            relativeTranslation->x -= distLeft * 1.001;
        } else {
          if (relativeTranslation->x > distRight)
            relativeTranslation->x -= distRight * 1.001;
        }
        if (relativeTranslation->y < 0.0) {
          if (relativeTranslation->y < distTop)
            relativeTranslation->y -= distTop * 1.001;
        } else {
          if (relativeTranslation->y > distLeft)
            relativeTranslation->y -= distBottom * 1.001;
        }
      }
    }
    return true;
  }

  static inline void calcScalarsLineLine(
      Ogre::Vector2* pos1,
      Ogre::Vector2* dir1,
      Ogre::Vector2* pos2,
      Ogre::Vector2* dir2,
      float* retScal1,
      float* retScal2) {
    float quot = dir1->y * dir2->x - dir1->x * dir2->y;
    *retScal1 = ((pos2->y - pos1->y) * dir2->x +
        (pos1->x - pos2->x) * dir2->y)/quot;
    *retScal2 = ((pos1->y - pos2->y) * dir1->x +
        (pos2->x - pos1->x) * dir1->y)/quot;
  }

  static inline bool isVertexRightToLine(
      Ogre::Vector2* pos1,
      Ogre::Vector2* pos2,
      Ogre::Vector2* dir2) {
    Ogre::Vector2 newPos1 = *pos1 - *pos2;
    return (newPos1.x * dir2->y - newPos1.y * dir2->y) > 0.0;
  }

  static inline bool testLineLine(
      Ogre::Vector2* pos1,
      Ogre::Vector2* dir1,
      Ogre::Vector2* pos2,
      Ogre::Vector2* dir2,
      Ogre::Vector2* retIntersection,
      float* retScal1,
      float* retScal2) {
    calcScalarsLineLine(pos1, dir1, pos2, dir2, retScal1, retScal2);
    if (*retScal1 < 1.0 && *retScal2 < 1.0 && *retScal1 * *retScal2 > 0.0) {
      *retIntersection = *pos1 + *dir1 * *retScal1;
      return true;
    }
    return false;
  }

  static inline bool testLineLine(
      Ogre::Vector2* pos1,
      Ogre::Vector2* dir1,
      Ogre::Vector2* pos2,
      Ogre::Vector2* dir2,
      Ogre::Vector2* retIntersection) {
    float scal1, scal2;
    return testLineLine(
        pos1, dir1, pos2, dir2, retIntersection, &scal1, &scal2);
  }

  static inline void projectConvex(
    Ogre::Vector2* axis,
    CollisionShape2::Convex* convex,
    float* retMin,
    float* retMax) {
    // To project a point on an axis use the dot product

    float dotProduct = axis->dotProduct(*convex->points[0]);
    *retMin = dotProduct;
    *retMax = dotProduct;
    for (int i = 0; i < convex->points.size(); i++) {
      dotProduct = convex->points[i]->dotProduct(*axis);
      if (dotProduct < *retMin) {
        *retMin = dotProduct;
      } else {
        if (dotProduct > *retMax) {
          *retMax = dotProduct;
        }
      }
    }
  }

  static inline bool collisionTestConvex(
       CollisionShape2 *a,
       CollisionShape2 *b,
       Ogre::Vector2 *relativeTranslation,
       bool constrainTranslation = true) {
    Ogre::Vector2 minDistAxis;
    float minDist = 100.0;
    // Loop through all the edges of both polygons
    Ogre::Vector2* currentEdge;
    for (int i = 0; i < a->convex.edges.size() + b->convex.edges.size(); i++) {
      if (i < a->convex.edges.size()) {
       currentEdge = &a->convex.edges[i];
      } else {
       currentEdge = &b->convex.edges[i - a->convex.edges.size()];
      }

      Ogre::Vector2 axis(-currentEdge->y, currentEdge->x);
      axis.normalise();

      float minA = 0.0;
      float minB = 0.0;
      float maxA = 0.0;
      float maxB = 0.0;
      projectConvex(&axis, &a->convex, &minA, &maxA);
      projectConvex(&axis, &b->convex, &minB, &maxB);

      float velocityProjection = axis.dotProduct(*relativeTranslation);

      if (velocityProjection < 0.0) {
          minA += velocityProjection;
      } else {
          maxA += velocityProjection;
      }

      float dist;
      if (minA < minB) {
        dist = minB - maxA;
        if (dist > 0.0) return false;
      } else {
        dist = minA - maxB;
        if (dist > 0.0) return false;
      }

      if (constrainTranslation) {
        if (fabsf(dist) < fabsf(minDist)) {
            minDist = dist;
            minDistAxis = axis;
        }
      }
    }
    std::cout << minDistAxis.x << ", " << minDistAxis.y << " * " << minDist <<
        std::endl;
    *relativeTranslation = minDistAxis * minDist * 0.999;
    return true;
  }

  static inline bool collisionTestAABBLinestrip(
       CollisionShape2 *a,
       CollisionShape2 *b,
       Ogre::Vector2 *relativeTranslation,
       bool constrainTranslation = true) {
    bool intersects = false;
    float maximumRatio = 0.0;

    for (int i = 0; i < a->linestrip.edges.size(); i++) {
      Ogre::Vector2 currentLine = a->linestrip.edges[i].perpendicular();
      currentLine.normalise();

      float translProj = currentLine.dotProduct(*relativeTranslation);

      float current = currentLine.dotProduct(a->aabb.bottomLeft());
      float max = current;
      float min = current;
      current = currentLine.dotProduct(a->aabb.bottomRight());
      if (current > max)
        max = current;
      if (current < min)
        min = current;
      current = currentLine.dotProduct(a->aabb.topRight());
      if (current > max)
        max = current;
      if (current < min)
        min = current;
      current = currentLine.dotProduct(a->aabb.topLeft());
      if (current > max)
        max = current;
      if (current < min)
        min = current;

      if (translProj < 0.0)
        min += translProj;
      if (translProj > 0.0)
        max += translProj;
      std::cout << "point " << *a->linestrip.points[i] << std::endl;
      std::cout << "line " << currentLine << std::endl;
      float pivot = currentLine.dotProduct(*a->linestrip.points[i]);
      if (min < pivot && max > pivot) {
        std::cout << min << " " << pivot << " " << max << std::endl;
        intersects = true;
        float ratio;
        if (translProj < 0.0) {
          ratio = translProj + pivot - min;
        } else {
          ratio = translProj + pivot - max;
        }
        ratio = ratio / fabsf(translProj);
        if (ratio > maximumRatio)
          maximumRatio = ratio;
      }
    }
    if (intersects) {
      *relativeTranslation -= *relativeTranslation * maximumRatio;
      return true;
    } else {
      return false;
    }
  }
};

#endif /* COLLISIONHANDLER_H_ */
