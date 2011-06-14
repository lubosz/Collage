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
//    if (!collisionTestAABB(a, b, relativeTranslation, false))
//      return false;
// TODO(Gerrit) add further complexity levels
    return true;
  }

  static inline bool CollisionTestAABB(
      CollisionShape2::AABB *a,
      CollisionShape2::AABB *b,
      Ogre::Vector2 *relativeTranslation,
      float *distLeft,
      float *distRight,
      float *distTop,
      float *distBottom) {
    if (relativeTranslation->x < 0.0) {
      *distRight = *a->minX + relativeTranslation->x - *b->maxX;
      *distLeft = *a->maxX - *b->minX;
    } else {
      *distRight = *a->minX - *b->maxX;
      *distLeft = *a->maxX + relativeTranslation->x - *b->minX;
    }
    if (relativeTranslation->y < 0.0) {
      *distBottom = *a->minY + relativeTranslation->y - *b->maxY;
      *distTop = *a->maxY - *b->minY;
    } else {
      *distBottom = *a->minY - *b->maxY;
      *distTop = *a->maxY + relativeTranslation->y - *b->minY;
    }
    if ((*distRight > 0.001 || *distLeft < -0.001)
        || (*distBottom > 0.001 || *distTop < -0.001))
      return false;
    return true;
  }

  static inline bool CollisionTestAABB(
        CollisionShape2::AABB *a,
        CollisionShape2::AABB *b,
        Ogre::Vector2 *relativeTranslation) {
      float distRight, distLeft, distBottom, distTop;
      return CollisionTestAABB(a, b, relativeTranslation,
          &distLeft, &distRight, &distTop, &distBottom);
  }

  static inline bool collisionTestAABB(
      CollisionShape2 *a,
      CollisionShape2 *b,
      Ogre::Vector2 *relativeTranslation,
      float *possibleTranslationScale,
      Ogre::Vector2 *collisionNormal) {
    float distRight, distLeft, distBottom, distTop;
    if (CollisionTestAABB(
        &a->aabb,
        &b->aabb,
        relativeTranslation,
        &distLeft, &distRight, &distTop, &distBottom)) {
      return false;
    } else {
      float ratio = 1.0;
      *possibleTranslationScale = ratio;
      if (relativeTranslation->x < 0.0) {
        if (relativeTranslation->x <= distRight) {
          ratio -= distRight / relativeTranslation->x;
          *collisionNormal = Ogre::Vector2(-1.0, 0.0);
        }
      } else {
        if (relativeTranslation->x >= distLeft) {
          ratio -= distLeft / relativeTranslation->x;
          *collisionNormal = Ogre::Vector2(1.0, 0.0);
        }
      }
      if (relativeTranslation->y < 0.0) {
        if (relativeTranslation->y <= distBottom) {
          ratio -= distBottom / relativeTranslation->y;
          *collisionNormal = Ogre::Vector2(0.0, 1.0);
        }
      } else {
        if (relativeTranslation->y >= distTop) {
          ratio -= distTop / relativeTranslation->y;
          *collisionNormal = Ogre::Vector2(0.0, -1.0);
        }
      }
      if (fabsf(ratio) < 1.0) *possibleTranslationScale = ratio;
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
    return (newPos1.x * dir2->y - newPos1.y * dir2->x) > 0.0;
  }

  static inline bool isVecRightToVec(
      Ogre::Vector2* dir1,
      Ogre::Vector2* dir2) {
    return (dir1->x * dir2->y - dir1->y * dir2->x) > 0.0;
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
       CollisionShape2::Linestrip *linestrip,
       Ogre::Vector2 *relativeTranslation,
       float *ratio,
       Ogre::Vector2 *minCollNormal,
       Ogre::Vector2 *maxCollNormal) {
    bool intersects = false;

    CollisionShape2::AABB *aabb = &a->aabb;
    Ogre::Vector2 topRight = aabb->topRight();
    Ogre::Vector2 topLeft = aabb->topLeft();
    Ogre::Vector2 bottomLeft = aabb->bottomLeft();
    Ogre::Vector2 bottomRight = aabb->bottomRight();
    CollisionShape2::Convex aabbConvex;
    aabbConvex.points.push_back(&topRight);
    aabbConvex.points.push_back(&topLeft);
    aabbConvex.points.push_back(&bottomLeft);
    aabbConvex.points.push_back(&bottomRight);
    aabbConvex.points.push_back(&topRight);
    for (int i = 0; i <  aabbConvex.points.size()-1; i++) {
      aabbConvex.edges.push_back(
          *aabbConvex.points[i+1] - *aabbConvex.points[i]);
    }

    for (int i = 0; i < linestrip->edges.size(); i++) {
      CollisionShape2::AABB *currentAABB = &linestrip->edgeAABBs[i];
      if (CollisionTestAABB(
          aabb, currentAABB, relativeTranslation)) {
        Ogre::Vector2 currentCollNormal;

        if (collisionTestConvexLine(
            &a->convex, linestrip->points[i], linestrip->points[i+1],
            relativeTranslation, ratio, &currentCollNormal)) {
          intersects = true;
//          std::cout << "collNor " << currentCollNormal << std::endl;
          if (constrainMove(&currentCollNormal, minCollNormal, maxCollNormal)) {
            *ratio = 0.0;
          }
//          *minCollNormal = currentCollNormal;
        }
      }
    }
    return intersects;
  }

//  static inline bool collisionTestAABBLine(
//       CollisionShape2::AABB *aabb,
//       Ogre::Vector2 *pos,
//       Ogre::Vector2 *dir,
//       Ogre::Vector2 *relativeTranslation,
//       float *ratio,
//       Ogre::Vector2 *collisionNormal) {
//    Ogre::Vector2 currentLine = dir->perpendicular();
//    currentLine.normalise();
//
//    float translProj = currentLine.dotProduct(*relativeTranslation);
//
//    float current = currentLine.dotProduct(aabb->bottomLeft());
//    float max = current;
//    float min = current;
//    current = currentLine.dotProduct(aabb->bottomRight());
//    if (current > max)
//      max = current;
//    if (current < min)
//      min = current;
//    current = currentLine.dotProduct(aabb->topRight());
//    if (current > max)
//      max = current;
//    if (current < min)
//      min = current;
//    current = currentLine.dotProduct(aabb->topLeft());
//    if (current > max)
//      max = current;
//    if (current < min)
//      min = current;
//
//    if (translProj < 0.0) {
//      min += translProj;
//    } else {
//      max += translProj;
//    }
//    float pivot = currentLine.dotProduct(*pos);
//    if (min < pivot + 0.001 && max > pivot - 0.001) {
//      float dist;
//      if (translProj < 0.0) {
//        dist = fabsf(pivot - min);
//      } else {
//        dist = fabsf(pivot - max);
//      }
//      if (greaterZero(dist) && greaterZero(fabsf(translProj))) {
//        *ratio = 1.0 - dist / fabsf(translProj);
//      } else {
//        *ratio = 1.0;
//      }
//      *collisionNormal = currentLine;
//      return true;
//    }
//    return false;
//  }
//
//  static inline bool testPointsLine(
//      std::vector<Ogre::Vector2*> *points,
//      Ogre::Vector2 *pos,
//      Ogre::Vector2 *dir,
//      Ogre::Vector2 *relativeTranslation,
//      float *dist,
//      float *translProj) {
//    Ogre::Vector2 currentLine = dir->perpendicular();
//    currentLine.normalise();
//
//    *translProj = currentLine.dotProduct(*relativeTranslation);
//
//    float current = currentLine.dotProduct(*(*points)[0]);
//    float max = current;
//    float min = current;
//    for (int i = 1; i < points->size(); i++) {
//      current = currentLine.dotProduct(*(*points)[i]);
//      if (current > max)
//        max = current;
//      if (current < min)
//        min = current;
//    }
//
//    if (*translProj < 0.0) {
//      min += *translProj;
//    } else {
//      max += *translProj;
//    }
//    float pivot = currentLine.dotProduct(*pos);
//    if (min < pivot + 0.001 && max > pivot - 0.001) {
//      if (*translProj < 0.0) {
//        *dist = fabsf(pivot - min);
//      } else {
//        *dist = fabsf(pivot - max);
//      }
//      return true;
//    }
//    return false;
//  }
//
//  static inline bool collisionTestConvexLine(
//       CollisionShape2::Convex *convex,
//       Ogre::Vector2 *pos1,
//       Ogre::Vector2 *dir,
//       Ogre::Vector2 *relativeTranslation,
//       float *ratio,
//       Ogre::Vector2 *collisionNormal) {
//    float minDist = 99999.0;
//    float minTranslProj = 0.0;
//    Ogre::Vector2 pos2 = *pos1 + *dir;
//    std::vector<Ogre::Vector2*> points;
//    points.push_back(pos1);
//    points.push_back(&pos2);
//    float dist;
//    float translProj;
//    bool intersect = false;
//    for (int i = 0; i < convex->edges.size(); i++) {
//      if (testPointsLine(&points, convex->points[i], &convex->edges[i],
//          relativeTranslation, &dist, &translProj)) {
//        intersect = true;
//        if (dist < minDist) {
//          minDist = dist;
//          minTranslProj = translProj;
//          *collisionNormal = convex->edges[i];
//        }
//      }
//    }
//    if (testPointsLine(&convex->points, pos1, dir,
//        relativeTranslation, &dist, &translProj)) {
//      intersect = true;
//      if (dist < minDist) {
//        minDist = dist;
//        minTranslProj = translProj;
//        *collisionNormal = *dir;
//      }
//    }
//    *collisionNormal = collisionNormal->perpendicular();
//    collisionNormal->normalise();
//    if (greaterZero(dist) && greaterZero(fabsf(minTranslProj))) {
//      *ratio = 1.0 - dist / fabsf(minTranslProj);
//    } else {
//      *ratio = 1.0;
//    }
//    return intersect;
//  }

  static inline bool learlyZero(float *value) {
    return fabsf(*value) < 0.0001;
  }

  static inline bool intersection(
      float pivot,
      float *min,
      float *max,
      float *translProj,
      float *dist) {
//    if (nearlyZero(translProj)) {
//      if (*min < *pivot && *max > *pivot) {
//        *dist = 0.0;
//        return true;
//      }
//      return false;
//    }
    if (*translProj < 0.0) {
      *min += *translProj;
      if (*min < pivot && *max > pivot) {
//        std::cout << "TRUE----- "
//            << *min << " < " << pivot << " < " << *max << " ?" << std::endl;
        *dist = pivot - *min;
        return true;
      }
    } else {
      *max += *translProj;
      if (*min < pivot && *max > pivot) {
//        std::cout << "TRUE----- "
//            << *min << " < " << pivot << " < " << *max << " ?" << std::endl;
        *dist = *max - pivot;
        return true;
      }
    }
//    std::cout << "FALSE---- "
//        << *min << " < " << pivot << " < " << *max << " ?" << std::endl;
    return false;
  }

  static inline bool intersection(
      float *minA,
      float *maxA,
      float *minB,
      float *maxB,
      float *translProj,
      float *dist) {
    if (*translProj <= epsilon()) {
      float newMinA = *minA + *translProj;
      if (newMinA <= *maxB + epsilon() && *maxA + epsilon() >= *minB) {
        *dist = fabsf(*maxB - newMinA);
//        std::cout << "TRUE----- " << "translProj: " << *translProj << " | "
//            << *minA << " < " << *maxA  << " && " << *minB << " < " << *maxB
//            << " dist: " << *dist << std::endl;
        return true;
      }
    } else {
//      if (minA >= minB && minA <= maxB) {
      float newMaxA = *maxA + *translProj;
      if (*minA <= *maxB + epsilon() && newMaxA + epsilon() >= *minB) {
        if (fabsf(fabsf(*minA) - *maxB) < *translProj) {
          return false;
        }
        *dist = fabsf(newMaxA - *minB);
//        std::cout << "TRUE-2--- " << "translProj: " << *translProj << " | "
//            << *minA << " < " << *maxA  << " && " << *minB << " < " << *maxB
//            << " dist: " << *dist << std::endl;
        return true;
      }
    }
//    std::cout << "FALSE---- " << "translProj: " << *translProj << " | "
//        << *minA << " < " << *maxA  << " && " << *minB << " < " << *maxB
//        << " dist: " << *dist << std::endl;
    return false;
  }

  static inline void replaceMinMax(float value, float *min, float *max) {
    if (value <= *min)
      *min = value;
    if (value >= *max)
      *max = value;
  }

  static inline void replaceMin(float value, float *min) {
    if (value <= *min) {
      *min = value;
    }
  }

  static inline bool calculateMinimumTranslation(
      Ogre::Vector2* currentLine,
      std::vector<Ogre::Vector2*>* pointsA,
      std::vector<Ogre::Vector2*>* pointsB,
      Ogre::Vector2 *relativeTranslation,
      float *minRatio,
      float *minDist,
      float *minTranslProj,
      Ogre::Vector2 *collisionNormal
      ) {
    currentLine->normalise();
//    std::cout << *currentLine << *relativeTranslation << std::endl;
    float translProj = currentLine->dotProduct(*relativeTranslation);
    float dist = 0.0;  // -std::numeric_limits<float>::infinity();
    float minA = std::numeric_limits<float>::infinity();
    float maxA = -std::numeric_limits<float>::infinity();
    float minB = std::numeric_limits<float>::infinity();
    float maxB = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < pointsA->size(); i++) {
//      std::cout << *(*pointsA)[i] << currentLine->dotProduct(*(*pointsA)[i])
//          << "  |  ";
      replaceMinMax(currentLine->dotProduct(*(*pointsA)[i]), &minA, &maxA);
    }
//    std::cout << std::endl;
    for (int i = 0; i < pointsB->size(); i++) {
//      std::cout << *(*pointsB)[i] << currentLine->dotProduct(*(*pointsB)[i])
//          << "  |  ";
      replaceMinMax(currentLine->dotProduct(*(*pointsB)[i]), &minB, &maxB);
    }
//    std::cout << std::endl;
    if (intersection(&minA, &maxA, &minB, &maxB, &translProj, &dist)) {
//      if (fabsf(translProj) < epsilon())
//        translProj = epsilon();
      float newRatio = fabsf(dist / translProj);
//      printf("ratio: %f > newRatio: %f minDist: %f > dist: %f ? \n",
//          *minRatio, newRatio, *minDist, dist);
      if (newRatio < 1.0) {
        if (*minRatio > newRatio) {
          *minRatio = newRatio;
          *minDist = 0.0;
//          std::cout << "set1" << std::endl;
          *collisionNormal = *currentLine;
        }
      } else {
        if (*minDist > dist) {
          *minDist = dist;
//          std::cout << "set2" << std::endl;
          *collisionNormal = *currentLine;
        }
      }

//      if (relativeTranslation->length() != 0.0) {
//        if (translProj == 0.0) {
//          *collisionNormal = currentLine;
//        } else {
//          fabsf(dist / translProj);
//        }
//      }
//      if (translProj == 0.0 && relativeTranslation->length() != 0.0) {
//        *collisionNormal = currentLine;
//      } else {
//
//      }
//      if (translProj != 0.0) {
//        *newRatio = fabsf(dist / translProj);
//      } else {
//        *newRatio = fabsf(dist);
//      }
//      if (newRatio <= *ratio) {
//        *collisionNormal = currentLine;
//        *ratio = newRatio;
//      }
      return true;
    }
//    printf("%f / %f = %f \n", dist, translProj, fabsf(dist / translProj));
    return false;
  }

  static inline bool collisionTestConvexLine(
       CollisionShape2::Convex *convex,
       Ogre::Vector2 *linePt1,
       Ogre::Vector2 *linePt2,
       Ogre::Vector2 *relativeTranslation,
       float *ratio,
       Ogre::Vector2 *collisionNormal) {
    Ogre::Vector2 dir = *linePt2 - *linePt1;
    float newRatio = 1.0;

    *collisionNormal = Ogre::Vector2::ZERO;
    std::vector<Ogre::Vector2*> linePoints;
    linePoints.push_back(linePt1);
    linePoints.push_back(linePt2);


    float minDist = std::numeric_limits<float>::infinity();
    float minTranslProj = std::numeric_limits<float>::infinity();
    Ogre::Vector2 currentLine;
    currentLine = dir;
//      printf("DIR \n");
    if (!calculateMinimumTranslation(&currentLine, &convex->points,
        &linePoints, relativeTranslation,
        &newRatio, &minDist, &minTranslProj, collisionNormal)) {
      return false;
    }
//    std::cout << *collisionNormal << std::endl;

    currentLine = dir.perpendicular();
//      printf("DIR PERP \n");
    if (!calculateMinimumTranslation(&currentLine, &convex->points,
        &linePoints, relativeTranslation,
        &newRatio, &minDist, &minTranslProj, collisionNormal)) {
      return false;
    }
//    std::cout << *collisionNormal << std::endl;

    for (int i = 0; i < convex->edges.size(); i++) {
      currentLine = -convex->edges[i].perpendicular();
//        printf("CONVEX %i ", i);
//        std::cout << convex->edges[i] << std::endl;
      if (!calculateMinimumTranslation(&currentLine, &convex->points,
          &linePoints, relativeTranslation,
          &newRatio, &minDist, &minTranslProj, collisionNormal)) {
        return false;
      }
//      std::cout << *collisionNormal << std::endl;
    }
    if (newRatio < 1.0 && newRatio > 0.0) {
      newRatio = 1.0 - newRatio;
    }
    if (newRatio < *ratio) {
      *ratio = newRatio;
    }
    return true;
  }

  static inline bool greaterZero(float value) {
    return value > 0.00001;
  }

  static inline bool nearlyZero(float value) {
    return fabsf(value) < 0.00001;
  }

  static inline float epsilon() {
    return 0.0001;
  }

  static inline bool constrainMove(
      Ogre::Vector2 *by,
      Ogre::Vector2 *moveConstraintMin,
      Ogre::Vector2 *moveConstraintMax) {
    Ogre::Vector2 newBy = by->normalisedCopy();
    if (moveConstraintMin->isZeroLength()) {
      *moveConstraintMin = newBy;
      *moveConstraintMax = newBy;
    } else {
      if (newBy.x * moveConstraintMin->y - newBy.y * moveConstraintMin->x
          > 0.0) {
        if (newBy.x * moveConstraintMax->y - newBy.y * moveConstraintMax->x
            > 0.0) {
          *moveConstraintMin = newBy;
        } else {
          return true;  // no move possible anymore
        }
      } else {
        if (newBy.x * moveConstraintMax->y - newBy.y * moveConstraintMax->x
            < 0.0) {
          *moveConstraintMax = newBy;
        }
      }
    }
    return false;
  }
};

#endif /* COLLISIONHANDLER_H_ */
