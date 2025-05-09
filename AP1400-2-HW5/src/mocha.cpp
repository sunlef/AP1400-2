#include "mocha.h"
#include "sub_ingredients.h"

Mocha::Mocha() {
  name = "Mocha";
  ingredients.emplace_back(new Espresso(2));
  ingredients.emplace_back(new Milk(2));
  ingredients.emplace_back(new MilkFoam(1));
  ingredients.emplace_back(new Chocolate(1));
}

Mocha::Mocha(const Mocha &cap) : EspressoBased(cap) {
  name = cap.name;
  for (auto &ptr : cap.side_items) {
    side_items.emplace_back(ptr->clone());
  }
}

Mocha::~Mocha() {
  for (const auto &i : side_items)
    delete i;
  side_items.clear();
}

void Mocha::operator=(const Mocha &cap) {
  if (this != &cap) {
    EspressoBased::operator=(cap);
    for (const auto &i : side_items)
      delete i;
    side_items.clear();
    name = cap.name;
    for (auto &ptr : cap.side_items) {
      side_items.emplace_back(ptr->clone());
    }
  }
}

std::string Mocha::get_name() { return name; }

double Mocha::price() {
  double res = 0;
  for (auto &ptr : ingredients) {
    res += ptr->price();
  }
  for (auto &ptr : side_items) {
    res += ptr->price();
  }
  return res;
}

void Mocha::add_side_item(Ingredient *side) { side_items.emplace_back(side); }

std::vector<Ingredient *> &Mocha::get_side_items() { return side_items; }