
// Created by Vikram and Aimee on 2019-03-09.

#pragma once

#include "model.h"
#include <ge211.h>

#include <string>

struct Ui : ge211::Abstract_game
{
public:
    explicit Ui(Model&);

    void draw(ge211::Sprite_set& set) override;

    ge211::Transform get_transform(ge211::Image_sprite given_spr, ge211::Dimensions needed);

    double position_to_angle (ge211::Position) const;

    ge211::Dimensions initial_window_dimensions() const override;

    std::string initial_window_title() const override;

    void on_key(ge211::Key) override;

    void on_mouse_up(ge211::Mouse_button, ge211::Position) override;

    void on_mouse_move(ge211::Position) override;

    void on_frame(double) override;

private:
    Model& model_;


    int background_layer = 1;
    int active_layer = 2;

    ge211::Color campus_col {0,200,0};
    ge211::Color health_col {200,0,0};


    ge211::Image_sprite  const
            Ohio_state_sprite    {"Ohio_state.png"};

    ge211::Image_sprite  const
            UMichigan_sprite    {"UMichigan.png"};

    ge211::Image_sprite const
            Michigan_state_sprite {"Michigan_state.png"};

    ge211::Image_sprite const
            Wisconsin_sprite {"Wisconsin.png"};

    ge211::Image_sprite const
            Purdue_sprite {"Purdue.png"};


    ge211::Image_sprite const
            Bullet_sprite {"Willie.png"};


    std::vector<ge211::Image_sprite> rivals{Ohio_state_sprite, UMichigan_sprite,
                                                Wisconsin_sprite, Purdue_sprite, Michigan_state_sprite};


    /*
    ge211::Circle_sprite  const Ohio_state_sprite    {model_.logo_radius, {0,100,200}};

    ge211::Circle_sprite const UMichigan_sprite    {model_.logo_radius, {100,0,200}};

    ge211::Circle_sprite const Michigan_state_sprite {model_.logo_radius, {100,100,200}};

    ge211::Circle_sprite const Wisconsin_sprite {model_.logo_radius, {200,100,200}};

    ge211::Circle_sprite const Purdue_sprite {model_.logo_radius, {100,200,200}};

    ge211::Circle_sprite const Bullet_sprite {model_.bullet_radius, {0,200,200}};

    std::vector<ge211::Circle_sprite> rivals{Ohio_state_sprite, UMichigan_sprite,
                                                Wisconsin_sprite, Purdue_sprite, Michigan_state_sprite};
    */
    ge211::Rectangle_sprite const Cannon_sprite {model_.get_cannon().cannon.dimensions(), {200,200,200}};
    //ge211::Image_sprite const Cannon_sprite {"Cannon.png"};

    ge211::Rectangle_sprite const
            Campus_sprite {model_.get_campus().dimensions(), campus_col};
    ge211::Rectangle_sprite const
            lakefill_sprite {{model_.scene_dims().width, model_.get_campus().dimensions().height}, {0, 0, 255}};

    ge211::Rectangle_sprite Health_sprite {model_.get_meter().dimensions(), health_col};
    ge211::Rectangle_sprite Pointer_sprite {{model_.get_meter().dimensions().width,5}, {200,200,200}};

    ge211::Font font_ = ge211::Font("sans.ttf", 50);
    ge211::Text_sprite score_end_sprite{"You protected against __ rival schools!", font_};
    ge211::Font font2_ = ge211::Font("sans.ttf", 30);
    ge211::Text_sprite score_counter_sprite{"Score:", font2_};
};
