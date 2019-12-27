//
// Created by Vikram and Aimee on 2019-03-12.
//
#pragma once

#include <ge211.h>

// The position and velocity of one ball,
// as well as the landed state (which we only care about if it is a logo).
// Both the logos and bullets are of the Ball struct.
// Method for finding the next state of the ball.
struct Ball
{
    //constructor
    //the direction is either down for logos or given by the cannon for bullets
    Ball(ge211::Position center, std::tuple<double, double> const& velocity, int radius, int pic);

    // The to left of the bounding box
    ge211::Position top_left() const;

    // Returns a new ball like this one but whose position has been
    // updated by adding on the velocity.
    Ball next() const;

    int picture_;
    int radius_;

    // The velocity of the ball in pixels per tick.
    std::tuple<double, double> velocity_;

    // The position of the center of the ball.
    ge211::Position center_;

    // If this ball is a logo, this says whether it is falling or landed
    // If this is a bullet it will always be false
    bool landed_ = false;

    // Determines whether the ball
    // hits the screen edges.
    bool hits_top(ge211::Dimensions) const;

    bool hits_left(ge211::Dimensions) const;

    bool hits_right(ge211::Dimensions) const;

    bool hits_bottom(ge211::Dimensions) const;

    bool hits_campus(ge211::Rectangle campus) const;

    bool hits_ball(Ball b) const;
};

// Compares two `Ball`s for equality. This may be useful for testing.
// Two `Ball`s are equal if all their member variables are pairwise
// equal.
bool operator==(Ball const&, Ball const&);

// Inequality for `Ball`s.
bool operator!=(Ball const&, Ball const&);
