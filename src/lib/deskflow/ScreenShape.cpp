/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2012 - 2016 Symless Ltd.
 * SPDX-FileCopyrightText: (C) 2003 Chris Schoeneman
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include <cstdint>
#include <limits>
#include <vector>

struct Rect
{
  int32_t left, top, right, bottom;

  bool contains(int32_t x, int32_t y) const
  {
    return x >= left && x < right && y >= top && y < bottom;
  }
};

class ScreenShape
{
public:
  explicit ScreenShape(const std::vector<Rect> &monitorShapes) : monitors(monitorShapes)
  {
    if (monitors.empty())
      return;

    primary_monitor = &monitors[0];

    for (const auto &m : monitors) {
      if (m.left < leftmost_->left)
        leftmost_ = &m;
      if (m.right > rightmost_->right)
        rightmost_ = &m;
      if (m.top < topmost_->top)
        topmost_ = &m;
      if (m.bottom > bottommost_->bottom)
        bottommost_ = &m;
    }
  }

  bool contains(int32_t x, int32_t y) const
  {
    for (const Rect &m : monitors) {
      if (m.contains(x, y))
        return true;
    }

    return false;
  }

  int32_t getTop() const {
    return topmost_->top;
  }

  int32_t getBottom() const {
    return bottommost_->bottom;
  }

  int32_t getLeft() const {
    return leftmost_->left;
  }

  int32_t getRight() const {
    return rightmost_->right;
  }

  void getPrimary(Rect &out) const {
    out = *primary_monitor;
  }

private:
  std::vector<Rect> monitors;
  const Rect *primary_monitor = nullptr;
  const Rect *leftmost_ = nullptr;
  const Rect *rightmost_ = nullptr;
  const Rect *topmost_ = nullptr;
  const Rect *bottommost_ = nullptr;
};
