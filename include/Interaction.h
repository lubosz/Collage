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

  bool ok;
  T1* a;
  T2* b;

  void init(T1* a, T2* b) {
    this->a = a;
    this->b = b;
    this->ok = false;
  }

  void print() {
    printf("I(");
    a->print();
    printf(",");
    b->print();
    printf(")");
  }
};

template <class T1, class T2>
class Interaction : public AbstractInteraction<T1, T2> {
 public:
  Interaction() {}
  virtual ~Interaction() {}
};

#endif /* INTERACTION_H_ */
