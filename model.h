#pragma once

#include <ge211.h>
#define PI 3.14159265

//
// Created by Vikram and Aimee on 2019-03-01.
//
#pragma once
#include <ge211.h>
#include "ball.h"

// Model constants :
double const bullet_speed = 10;
double const min_logo_speed = 1;
double const max_logo_speed = 3;
time_t const wait_period = 2;
ge211::Dimensions const scene_dimensions = ge211 :: Dimensions{1024, 768};
ge211 :: Dimensions const full_meter_dims = ge211 :: Dimensions{5, 100};
ge211 :: Dimensions const cannon_dims = ge211 :: Dimensions{50, 100};
ge211::Position const meter_pos = ge211 :: Position{5, 50};
int const meter_max = 10;
int const max_landed_logos = 5;

//Angle of cannon and a rectangle representing cannon
// with the upper left corner being the point which cannon rotates around
struct Cannon
{
    double angle;//in degrees, starts at 90
    ge211::Rectangle cannon;
};

// The whole state of the game.
class Model {
//
// PRIVATE DATA MEMBERS
    // The velocity of the logos
    ge211::Dimensions logo_vel_ = min_logo_speed * ge211::Dimensions{0, 1};

    //Level of the meter
    int meter_fullness_ = meter_max;

    // The bullets
    std::vector<Ball> bullets_;

    // The falling logos
    std::vector<Ball> logos_;

    //loop of last shot
    int loop_of_last_shot_=0;

    int loops = 0;



    //campus position top left
    ge211::Rectangle campus_ = ge211::Rectangle{scene_dimensions.width/5, scene_dimensions.height-20, scene_dimensions.width*3/5, 20};

    //the cannon on the campus
    Cannon cannon_{90, ge211::Rectangle{campus_.x+campus_.width/2, campus_.y-cannon_dims.height/2, cannon_dims.width, cannon_dims.height}};

    //the meter object
    ge211::Rectangle meter_{meter_pos.x, meter_pos.y, full_meter_dims.width, full_meter_dims.height};

    public :
    int const logo_radius = 30;
    int const bullet_radius = 15;
    bool playing = true;
    int score= 0;
        //
        // PUBLIC FUNCTIONS
        //
        // Returns true if the meter is empty
        bool meter_empty() const;

        // Returns the cannon's state.
        Cannon get_cannon() const;

        // Returns the states of all the
        // logos.
        std::vector<Ball> const &
        get_logos() const;

        // Returns the states of all the
        // bullets.
        std::vector<Ball> const &
        get_bullets() const;

        // returns speed the icons should be falling at
        ge211::Dimensions get_logo_velocity();

        // Updates the model state for
        // one time step .
        void update(int, int);

        // Launches a new bullet.
        void launch_ball();

        // Drops a new logo into the game
        void drop_logo(int, int);

        // Changes the cannon to point in the direction of that position
        void cannon_to(ge211::Position);

        //get meter state, aka rectangle whose dimensions are based on the fullness
        ge211::Rectangle get_meter() const;

        //get number of landed logos
        int count_landed() const;

        //return the current state of the campus
        ge211::Rectangle get_campus() const;

        ge211::Dimensions scene_dims() const;

        //returns true if the top left of campus is at the bottom of the screen
        bool sunk() const;

    private :
    //
    // POSSIBLE HELPER FUNCTIONS
    //

    double logo_speed=min_logo_speed;

    // Returns a pointer to a hit
    // logo given a bullet, or nullptr if none .
    Ball& find_hit_logo_(Ball const &);

    //Looks through each bullet and checks if it hits a logo.
    // Removes all bullets and Logos hit.
        void check_and_remove_shots();

    // Removes the bullet and logo .
        void destroy_on_hit_(Ball &, Ball &);

    // Removes a bullet when it exits the screen
        void remove_on_out();

    // Check how much time has passed since the last shot and return true if it is
    //time to update the meter.
        bool sufficient_time();

    // Returns the meter to full.
        void reset_meter_();

    // Checks if a logo lands on the campus
        bool check_lands(Ball const &) const;

    // Looks through the logos and finds all that land on the campus and lands them
        void land_logos();

    //returns true if there are more than max_landed_logos on the campus
        bool check_invaded();

    //sinks the campus by x pixels
        void sink(int x);

    //calculates the angle between the cannon and the pos
        double position_to_angle(ge211::Position pos);

        //find the velocity of the bullet from the cannon angle
        std::tuple<double, double> bullet_velocity();

    // Test access
        friend struct Test_access;
};
