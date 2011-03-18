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

  T1* a;
  T2* b;

  void initActors(T1* a, T2* b) {
    this->a = a;
    this->b = b;
    init();
  }

  virtual void init() = 0;

  virtual void interact() = 0;

  virtual void print() {
    a->print();
    b->print();
  }
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
