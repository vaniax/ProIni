#pragma once

#include <iostream>

namespace GFrost {

class Vector2 {
public:
    Vector2() = default;
    virtual ~Vector2() = default;

    inline bool operator==(const Vector2& rhs) const {
        return (x_ == rhs.x_) && (y_ == rhs.y_); }

    float x_{};
    float y_{};
};



class Vector3 : public Vector2 {
public:
    float z_{};
};

class Vector4 : public Vector3 {
public:
    float w_{};
};

class Quaternion : public Vector4 {};


class Rect {
public:
    Rect() {}

    Rect(const Vector4 &rhs) {
        this->x_ = rhs.x_;
        this->y_ = rhs.y_;
        this->z_ = rhs.z_;
        this->w_ = rhs.w_;
    }

    Rect(int left, int top, int right, int bottom) {
        this->x_ = left;
        this->y_ = top;
        this->z_ = right;
        this->w_ = bottom;
    }

    int Left() { return x_; }
    int Top() { return y_; }
    int Right() { return z_; }
    int Bottom() { return w_; }

    Rect& operator=(const Rect & rhs) {
        return *this;
    }

    inline bool operator==(const Rect& rhs) const {
        return (x_ == rhs.x_) && (y_ == rhs.y_)
            && (z_ == rhs.z_) && (w_ == rhs.w_);
    }

    inline std::string ToString() {

        return "{ Left:"   + std::to_string(Left()) +
               ", Top:"    + std::to_string(Top()) +
               ", Right:"  + std::to_string(Right()) +
               ", Bottom:" + std::to_string(Bottom()) + "}";
    }

private:
    int x_{};
    int y_{};
    int z_{};
    int w_{};
};




class Color {
public:
    Color() = default;
    virtual ~Color() = default;

    float r_{};
    float g_{};
    float b_{};
    float a_{};
};

} //namespace GFrost