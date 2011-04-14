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

  static inline bool collisionTestConvex(
      CollisionShape2 *a,
      CollisionShape2 *b,
      Ogre::Vector2 *relativeTranslation) {
  }

  static inline bool collisionTestAABB(
      CollisionShape2 *a,
      CollisionShape2 *b,
      Ogre::Vector2 *relativeTranslation,
      bool constrainTranslation = true) {
    float distLeft = *a->aabb.minX + relativeTranslation->x - *b->aabb.maxX;
    float distRight = *a->aabb.maxX + relativeTranslation->x - *b->aabb.minX;
    float distTop = *a->aabb.minY + relativeTranslation->y - *b->aabb.maxY;
    float distBottom = *a->aabb.maxY + relativeTranslation->y - *b->aabb.minY;
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
    std::vector<Ogre::Vector2*>* convex,
    float* retMin,
    float* retMax) {
    // To project a point on an axis use the dot product

    float dotProduct = axis->dotProduct(*convex->at(0));
    *retMin = dotProduct;
    *retMax = dotProduct;
    for (int i = 0; i < convex->size(); i++) {
      dotProduct = convex->at(0)->dotProduct(*axis);
      if (dotProduct < *retMin) {
        *retMin = dotProduct;
      } else {
        if (dotProduct > *retMax) {
          *retMax = dotProduct;
        }
      }
    }
  }

  static inline float intervalDistance(
      float* minA,
      float* maxA,
      float* minB,
      float* maxB) {
    if (*minA < *minB) {
      return *minB - *maxA;
    } else {
      return *minA - *maxB;
    }
  }
//
// public PolygonCollisionResult PolygonCollision(Polygon polygonA,
//                               Polygon polygonB, Vector velocity) {
//     PolygonCollisionResult result = new PolygonCollisionResult();
//     result.Intersect = true;
//     result.WillIntersect = true;
//
//     int edgeCountA = polygonA.Edges.Count;
//     int edgeCountB = polygonB.Edges.Count;
//     float minIntervalDistance = float.PositiveInfinity;
//     Vector translationAxis = new Vector();
//     Vector edge;
//
//     // Loop through all the edges of both polygons
//
//     for (int edgeIndex = 0; edgeIndex <
// edgeCountA + edgeCountB; edgeIndex++) {
//         if (edgeIndex < edgeCountA) {
//             edge = polygonA.Edges[edgeIndex];
//         } else {
//             edge = polygonB.Edges[edgeIndex - edgeCountA];
//         }
//
//         // ===== 1. Find if the polygons are currently intersecting =====
//
//
//         // Find the axis perpendicular to the current edge
//
//         Vector axis = new Vector(-edge.Y, edge.X);
//         axis.Normalize();
//
//         // Find the projection of the polygon on the current axis
//
//         float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
//         ProjectPolygon(axis, polygonA, ref minA, ref maxA);
//         ProjectPolygon(axis, polygonB, ref minB, ref maxB);
//
//         // Check if the polygon projections are currentlty intersecting
//
//         if (IntervalDistance(minA, maxA, minB, maxB) > 0)\
//             result.Intersect = false;
//
//         // ===== 2. Now find if the polygons *will* intersect =====
//
//
//         // Project the velocity on the current axis
//
//         float velocityProjection = axis.DotProduct(velocity);
//
//         // Get the projection of polygon A during the movement
//
//         if (velocityProjection < 0) {
//             minA += velocityProjection;
//         } else {
//             maxA += velocityProjection;
//         }
//
//         // Do the same test as above for the new projection
//
//         float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
//         if (intervalDistance > 0) result.WillIntersect = false;
//
//         // If the polygons are not
//         // intersecting and won't intersect, exit the loop
//
//         if (!result.Intersect && !result.WillIntersect) break;
//
//         // Check if the current interval distance is the minimum one.
//         // If so store
//
//         // the interval distance and the current distance.
//
//         // This will be used to calculate the minimum translation vector
//
//         intervalDistance = Math.Abs(intervalDistance);
//         if (intervalDistance < minIntervalDistance) {
//             minIntervalDistance = intervalDistance;
//             translationAxis = axis;
//
//             Vector d = polygonA.Center - polygonB.Center;
//             if (d.DotProduct(translationAxis) < 0)
//                 translationAxis = -translationAxis;
//         }
//     }
//
//     // The minimum translation vector
//
//     // can be used to push the polygons appart.
//
//     if (result.WillIntersect)
//         result.MinimumTranslationVector =
//                translationAxis * minIntervalDistance;
//
//     return result;
// }

  // Check if polygon A is going to collide with polygon B.

  // The last parameter is the *relative* velocity

  // of the polygons (i.e. velocityA - velocityB)

//  public PolygonCollisionResult PolygonCollision(Polygon polygonA,
//                                Polygon polygonB, Vector velocity) {
//      PolygonCollisionResult result = new PolygonCollisionResult();
//      result.Intersect = true;
//      result.WillIntersect = true;
//
//      int edgeCountA = polygonA.Edges.Count;
//      int edgeCountB = polygonB.Edges.Count;
//      float minIntervalDistance = float.PositiveInfinity;
//      Vector translationAxis = new Vector();
//      Vector edge;
//
//      // Loop through all the edges of both polygons
//
//  for (int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++) {
//          if (edgeIndex < edgeCountA) {
//              edge = polygonA.Edges[edgeIndex];
//          } else {
//              edge = polygonB.Edges[edgeIndex - edgeCountA];
//          }
//
//          // ===== 1. Find if the polygons are currently intersecting =====
//
//
//          // Find the axis perpendicular to the current edge
//
//          Vector axis = new Vector(-edge.Y, edge.X);
//          axis.Normalize();
//
//          // Find the projection of the polygon on the current axis
//
//          float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
//          ProjectPolygon(axis, polygonA, ref minA, ref maxA);
//          ProjectPolygon(axis, polygonB, ref minB, ref maxB);
//
//          // Check if the polygon projections are currentlty intersecting
//
//          if (IntervalDistance(minA, maxA, minB, maxB) > 0)\
//              result.Intersect = false;
//
//          // ===== 2. Now find if the polygons *will* intersect =====
//
//
//          // Project the velocity on the current axis
//
//          float velocityProjection = axis.DotProduct(velocity);
//
//          // Get the projection of polygon A during the movement
//
//          if (velocityProjection < 0) {
//              minA += velocityProjection;
//          } else {
//              maxA += velocityProjection;
//          }
//
//          // Do the same test as above for the new projection
//
//          float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
//          if (intervalDistance > 0) result.WillIntersect = false;
//
//  // If the polygons are not intersecting and won't intersect, exit the loop
//
//          if (!result.Intersect && !result.WillIntersect) break;
//
//  // Check if the current interval distance is the minimum one. If so store
//
//          // the interval distance and the current distance.
//
//          // This will be used to calculate the minimum translation vector
//
//          intervalDistance = Math.Abs(intervalDistance);
//          if (intervalDistance < minIntervalDistance) {
//              minIntervalDistance = intervalDistance;
//              translationAxis = axis;
//
//              Vector d = polygonA.Center - polygonB.Center;
//              if (d.DotProduct(translationAxis) < 0)
//                  translationAxis = -translationAxis;
//          }
//      }
//
//      // The minimum translation vector
//
//      // can be used to push the polygons appart.
//
//      if (result.WillIntersect)
//          result.MinimumTranslationVector =
//                 translationAxis * minIntervalDistance;
//
//      return result;
//  }

//  static inline int testConvexLine(
//      Ogre::Vector2* pos1,
//      std::vector<Ogre::Vector2*>* convex1,
//      Ogre::Vector2* pos2,
//      Ogre::Vector2* dir2,
//      Ogre::Vector2* intersection1,
//      Ogre::Vector2* intersection2) {
//    Ogre::Vector2 relPos2 = *pos2 - *pos1;
//    convex1.push_back(convex1[0]);
//    bool bothInside = true;
//    Ogre::Vector2* currentReturnPoint = intersection1;
//
//    for (int i = 0; i < convex1.size()-1; i++) {
//      Ogre::Vector2 currentVertex =  pos1 + convex1[i];
//      Ogre::Vector2 currentDirection = convex1[i+1] - convex1[i];
//
//      if (isVertexRightToLine(pos2, currentVertex, currentDirection)) {
//        if (!isVertexRightToLine(
//            pos2 + dir2, currentVertex, currentDirection)) {
//          bothInside = false;
//          if (testLineLine(
//              pos2, dir2,
//              currentVertex, currentDirection, currentReturnPoint)) {
//            currentReturnPoint = intersection2;
//          }
//        }
//
//      } else {
//        bothInside = false;
//        if (isVertexRightToLine(
//            pos2 + dir2, currentVertex, currentDirection)) {
//          if (testLineLine(
//              pos2, dir2, currentVertex,
//              currentDirection, currentReturnPoint)) {
//            currentReturnPoint = intersection2;
//          }
//        } else {
//          return 0;
//        }
//      }
//
//      if (bothInside) return -1;
//      if (currentReturnPoint == intersection1) {
//        return 1;
//      } else {
//        return 2;
//      }
//    }
//  }

//  static inline bool testConvexLine(
//      Ogre::Vector2 pos1,
//      std::vector<Ogre::Vector2> convex1,
//      Ogre::Vector2 pos2,
//      Ogre::Vector2 dir2,
//      Ogre::Vector2* collisionPoint) {
//
//  }
};

#endif /* COLLISIONHANDLER_H_ */
