#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H

#include "ingredient.h"

#define DEFCLASS(CLASS_NAME, PRICE_PER_UNIT)                                   \
  class CLASS_NAME : public Ingredient {                                       \
  public:                                                                      \
    CLASS_NAME(size_t units) : Ingredient(PRICE_PER_UNIT, units) {             \
      this->name = #CLASS_NAME;                                                \
    }                                                                          \
    std::string get_name() override { return this->name; }                     \
    Ingredient *clone() const override { return new CLASS_NAME(*this); }       \
  };

DEFCLASS(Cinnamon, 5);
DEFCLASS(Chocolate, 5);
DEFCLASS(Sugar, 1);
DEFCLASS(Cookie, 10);
DEFCLASS(Espresso, 15);
DEFCLASS(Milk, 10);
DEFCLASS(MilkFoam, 5);
DEFCLASS(Water, 1);

#endif // SUB_INGREDIENTS_H