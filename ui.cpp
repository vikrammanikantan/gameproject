//
// Created by Vikram and Aimee on 2019-03-09.
//

#include "ui.h"
#include <ge211.h>
#include <cmath>

using namespace ge211;

Ui::Ui(Model& model)
        : model_(model)
{}

void Ui::draw(Sprite_set& set)
{
    if (!model_.sunk() && model_.playing) {
        set.add_sprite(Campus_sprite, model_.get_campus().top_left(), background_layer); //draw the campus
        set.add_sprite(lakefill_sprite, {0, model_.scene_dims().height-model_.get_campus().height/2}, background_layer);

        set.add_sprite(Cannon_sprite, model_.get_cannon().cannon.top_left(), active_layer,
                       Transform::rotation(-(model_.get_cannon().angle) + 90)); //draw the cannon

        std::vector<Ball> bullets = model_.get_bullets(); //draw the bullets (willie's)

        for (int x = 0; x < bullets.size(); x++) {
            set.add_sprite(Bullet_sprite, bullets[x].top_left(), background_layer,
                           get_transform(Bullet_sprite, {model_.bullet_radius * 2, model_.bullet_radius * 2}));
        }

        std::vector<Ball> logos = model_.get_logos(); //draw the falling rivals

        /*for (int x = 0; x< logos.size(); x++)
        {
            //need a way to determine which rival school it is
            set.add_sprite(rivals[x%rivals.size()], logos[x].top_left(), active_layer);
        }*/
        for (Ball logo: logos) {
            set.add_sprite(rivals[logo.picture_], logo.top_left(), active_layer,
                           get_transform(rivals[logo.picture_], {model_.logo_radius * 2, model_.logo_radius * 2}));
        }

        set.add_sprite(Health_sprite, {meter_pos.x, meter_pos.y}, background_layer);//draw the health meter

        set.add_sprite(Pointer_sprite, {model_.get_meter().top_left().x,
                                        model_.get_meter().top_left().y + full_meter_dims.height -
                                        model_.get_meter().height}, background_layer);
        Text_sprite::Builder builder = Text_sprite::Builder(font2_);
        builder.message("Score: " + std::to_string(model_.score)).color({255, 255, 255});
        score_counter_sprite.reconfigure(builder);
        set.add_sprite(score_counter_sprite, {0, 0}, active_layer);
    }
    else{
        Text_sprite::Builder builder = Text_sprite::Builder(font_);
        builder.message("You protected against " + std::to_string(model_.score) + " rival schools!").color({255, 255, 255});
        score_end_sprite.reconfigure(builder);
        set.add_sprite(score_end_sprite, {0, model_.scene_dims().height/2}, active_layer);
    }
}

ge211::Transform Ui::get_transform(ge211::Image_sprite given_spr, ge211::Dimensions needed)
{
    ge211::Dimensions given =  given_spr.dimensions();

    double scalex = given.width/needed.width;
    double scaley = given.height/needed.width;

    scalex = 1/scalex;
    scaley = 1/scaley;

    ge211::Transform tr1;
    tr1.set_scale_x(scalex);
    tr1.set_scale_y(scaley);

    return tr1;
}

//
// CONTROLLER FUNCTIONS
//

void Ui::on_frame(double dt)
{
    int rand = get_random().between(0, model_.scene_dims().width);
    int rand_type = get_random().between(0, (int) rivals.size()-1);
    model_.update(rand, rand_type);
}

void Ui::on_key(ge211::Key key)
{
    model_.launch_ball();
}

void Ui::on_mouse_up(ge211::Mouse_button mb, ge211::Position pos)
{
    model_.launch_ball();
}

void Ui::on_mouse_move(ge211::Position pos)
{
    model_.cannon_to(pos);
}

Dimensions Ui::initial_window_dimensions() const
{
    return model_.scene_dims();
}

std::string Ui::initial_window_title() const
{
    return "Campus Defense";
}
