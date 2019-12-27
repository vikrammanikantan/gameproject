//
// Created by Aimee and Vikram on 2019-03-12.
//

#include "ball.h"
#include <ge211.h>

Ball::Ball(ge211::Position center, std::tuple<double, double> const& velocity, int radius, int pic)
        : radius_   (radius)
        , velocity_ (velocity)
        , center_   (center)
        , picture_ (pic)
{ }

ge211::Position Ball::top_left() const
{
    return ge211::Position{center_.x-radius_, center_.y-radius_};
}

Ball Ball::next() const
{
    Ball new_ball(*this);
    new_ball.center_ = {(int)round(new_ball.center_.x+std::get<0>(velocity_)), (int)round(new_ball.center_.y+std::get<1>(velocity_))};
    return new_ball;
}

bool Ball::hits_top(ge211::Dimensions scene_dims) const{
    if(center_.y - radius_ < 0) return true;
    return false;
}

bool Ball::hits_left(ge211::Dimensions scene_dims) const{
    if(center_.x - radius_ < 0) return true;
    return false;
}

bool Ball::hits_right(ge211::Dimensions scene_dims) const{
    if(center_.x + radius_ > scene_dims.width) return true;
    return false;
}

bool Ball::hits_bottom(ge211::Dimensions scene_dims) const{
    if(center_.y + radius_ > scene_dims.height) return true;
    return false;
}

bool Ball::hits_campus(ge211::Rectangle campus) const{
    bool a = (center_.x + radius_ < campus.x) || (center_.x - radius_ > campus.x + campus.width ||
            center_.y + radius_ < campus.y || center_.y - radius_ > campus.y + campus.height);
    return !a;
}

bool Ball::hits_ball(Ball b) const{
    bool a = (center_.x + radius_ < b.top_left().x) || (center_.x - radius_ > b.top_left().x + 2*b.radius_ ||
            center_.y + radius_ < b.top_left().y || center_.y - radius_ > b.top_left().y + 2*b.radius_ );
    return !a;
}

bool operator==(Ball const& b1, Ball const& b2)
{
    return (b1.radius_==b2.radius_ && b1.center_==b2.center_ && b1.landed_==b2.landed_ && b1.velocity_==b2.velocity_);
}

bool operator!=(Ball const& b1, Ball const& b2)
{
    return !(b1 == b2);
}
