#include "cappuccino.h"
#include "sub_ingredients.h"

Cappuccino::Cappuccino() {
  name = "Cappuccino";
  ingredients.emplace_back(new Espresso(2));
  ingredients.emplace_back(new Milk(2));
  ingredients.emplace_back(new MilkFoam(1));
}

Cappuccino::Cappuccino(const Cappuccino &cap) : EspressoBased(cap) {
  name = cap.name;
  for (auto &ptr : cap.side_items) {
    side_items.emplace_back(ptr->clone());
  }
}

Cappuccino::~Cappuccino() {
  for (const auto &i : side_items)
    delete i;
  side_items.clear();
}

void Cappuccino::operator=(const Cappuccino &cap) {
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

std::string Cappuccino::get_name() { return name; }

double Cappuccino::price() {
  double res = 0;
  for (auto &ptr : ingredients) {
    res += ptr->price();
  }
  for (auto &ptr : side_items) {
    res += ptr->price();
  }
  return res;
}

void Cappuccino::add_side_item(Ingredient *side) {
  side_items.emplace_back(side);
}

std::vector<Ingredient *> &Cappuccino::get_side_items() { return side_items; }