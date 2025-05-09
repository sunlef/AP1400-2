#include "espresso_based.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <thread>

EspressoBased::~EspressoBased() {
  for (const auto &i : ingredients)
    delete i;
  ingredients.clear();
}

EspressoBased::EspressoBased(const EspressoBased &esp) {
  for (auto &ptr : esp.ingredients) {
    ingredients.emplace_back(ptr->clone());
  }
  name = esp.name;
}

void EspressoBased::operator=(const EspressoBased &esp) {
  if (this != &esp) {
    for (const auto &i : ingredients)
      delete i;
    ingredients.clear();
    for (auto &ptr : esp.ingredients) {
      ingredients.emplace_back(ptr->clone());
    }
    name = esp.name;
  }
}

std::vector<Ingredient *> &EspressoBased::get_ingredients() {
  return ingredients;
}

using namespace ftxui;

void EspressoBased::brew() {
  // 1) 计算总单位数
  int total_units = 0;
  for (auto* ing : ingredients)
    total_units += ing->get_units();

  // 2) 为每个单位准备装饰和名字
  std::vector<Decorator> palette;
  std::vector<std::string> names;
  palette.reserve(total_units);
  names.reserve(total_units);

  for (auto* ing : ingredients) {
    Decorator decor;
    auto name = ing->get_name();
    if (name == "Espresso")
      decor = bgcolor(Color::RosyBrown) | bold;
    else if (name == "Milk")
      decor = bgcolor(Color::White);
    else if (name == "MilkFoam")
      decor = bgcolor(Color::GrayLight);
    else if (name == "Chocolate")
      decor = bgcolor(Color::DarkRed);
    else if (name == "Sugar")
      decor = bgcolor(Color::Yellow);
    else
      decor = bgcolor(Color::Green);

    for (int i = 0; i < static_cast<int>(ing->get_units()); ++i) {
      palette.push_back(decor);
      names.push_back(name);
    }
  }

  // 3) 动画状态
  int current = 0;

  // 4) 创建全屏界面
  auto screen = ScreenInteractive::TerminalOutput();

  // 5) 渲染器
  auto renderer = Renderer([&] {
    // 5.1) 分数 & 进度条
    float fraction = total_units ? (current / float(total_units)) : 1.f;
    int percent = int(fraction * 100);

    auto bar_elem = gauge(fraction) 
                    | size(WIDTH, EQUAL, 20);  // 你可以调节进度条宽度
    auto bar = hbox({
      bar_elem,
      text(" " + std::to_string(percent) + "%") | size(WIDTH, EQUAL, 5)
    }) | flex | border;

    // 5.2) 容器：固定宽度 20 列，从下往上 total_units 行
    Elements rows;
    for (int row = 0; row < total_units; ++row) {
      int idx = total_units - row - 1;
      if (idx < current) {
        // 已倒入：显示名字并应用装饰，居中
        rows.push_back(
          text(names[idx]) 
          | hcenter 
          | palette[idx] 
          | size(WIDTH, EQUAL, 20)
          | border
        );
      } else {
        // 未倒入：空白格
        rows.push_back(
          text("") 
          | size(WIDTH, EQUAL, 20)
          | border
        );
      }
    }
    auto container = vbox(rows) | flex;

    // 5.3) 组合
    return vbox({
      bar,
      separator(),
      container
    }) | center;  // 居中整个布局
  });

  // 6) 动画线程：每 400ms 加一行；全部完成后再等 1s 再退出
  std::thread animator([&] {
    while (current < total_units) {
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
      ++current;
      screen.PostEvent(Event::Custom);
    }
    // 动画结束后额外停 1000ms
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    screen.ExitLoopClosure()();
  });

  // 7) 启动主循环
  screen.Loop(renderer);

  // 8) 等待线程安全退出
  animator.join();
}
