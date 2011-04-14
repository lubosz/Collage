/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTION_H_
#define INTERACTION_H_

template <class T1, class T2>
class AbstractInteraction {
 public:
  AbstractInteraction() {}
  virtual ~AbstractInteraction() {}

  T1* first;
  T2* second;

  void initActors(T1* first, T2* second) {
    this->first = first;
    this->second = second;
    init();
  }

  virtual void init() = 0;

  virtual void interact() = 0;

  virtual void print() {
    first->print();
    second->print();
  }
};

template <class T1, class T2>
class AbstractCollisionInteraction : public AbstractInteraction<T1, T2> {
 public:
  bool inside;

  void init() {
    inside = false;
    onInit();
  }

  void interact() {
    if (collisionTest()) {
      if (inside) {
        whileInside();
      } else {
        onEnter();
      }
      inside = true;
    } else {
      if (inside) {
        onLeave();
      } else {
        whileOutside();
      }
      inside = false;
    }
  }

  virtual bool collisionTest() = 0;

  virtual void onInit() {}

  virtual void onEnter() {}

  virtual void onLeave() {}

  virtual void whileInside() {}

  virtual void whileOutside() {}
};

template <class T1, class T2>
class Interaction : public AbstractInteraction<T1, T2> {
 public:
  Interaction() {}
  virtual ~Interaction() {}
  void init() {}
  void interact() {}
};

#endif /* INTERACTION_H_ */
