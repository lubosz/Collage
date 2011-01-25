/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTION_H_
#define INTERACTION_H_

class AbstractInteraction {};

template <class T1, class T2>
class Interaction : public AbstractInteraction {
 public:

  Interaction(T1* a, T2* b) {
    this->a = a;
    this->b = b;
  }

  virtual ~Interaction() {}

  bool ok;

 private:
  T1* a;
  T2* b;
};
#endif /* INTERACTION_H_ */
