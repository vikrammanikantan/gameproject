#include "model.h"

// created by Vikram and Aimee

#include <ge211.h>
#define PI 3.14159265

Ball empty_ball = Ball(ge211::Position{0,0}, {0,0}, 0, 0);

void Model::update(int rand, int rand_type)
{
    if (!sunk() && playing){
        if (check_invaded()){
            sink(1);
        }
        else{
            //check and fill meter
            if (sufficient_time()){
                reset_meter_();
            }

            //check for ball collisions & delete
            check_and_remove_shots();

            //check for landings, land
            land_logos();

            //check for outs & delete
            remove_on_out();

            //move all logos (not landed) and bullets to their nexts
            for (Ball& bullet : bullets_){
                bullet=bullet.next();
            }
            for (Ball& logo : logos_){
                if (!logo.landed_)
                    logo=logo.next();
            }

            //increase speed of future logos
            if (loops%1000==0 && loops!=0 && logo_vel_.height!=max_logo_speed){
                logo_vel_= (logo_speed+=1)*ge211::Dimensions{0, 1};
            }

            //drop new logo every 36
            if (loops%80==0){
                drop_logo(rand, rand_type);
            }

            /*if (loops>2000){
                playing_=false;
            }*/
            loops++;
        }
    }
    else{
        //no more moving happens
    }
}

void Model::launch_ball() {
    if (meter_fullness_>0){
        loop_of_last_shot_ = loops;
        bullets_.push_back(Ball(ge211::Position{cannon_.cannon.center().x, cannon_.cannon.center().y}, bullet_velocity(), bullet_radius, 0));
        meter_fullness_--;
        meter_={meter_pos.x, meter_pos.y, full_meter_dims.width, meter_fullness_*full_meter_dims.height/meter_max};
    }
}

// Drops a new logo into the game
void Model::drop_logo(int rand, int rand_type){
    logos_.push_back(Ball(ge211::Position{rand, 0}, {logo_vel_.width, logo_vel_.height}, logo_radius, rand_type));
}

// Changes the cannon to point in the direction of that position
void Model::cannon_to(ge211::Position mouse_pos){
    cannon_.angle=position_to_angle(mouse_pos);
}

//get number of landed logos
int Model::count_landed() const{
    int count = 0;
    for (Ball enemy : logos_){
        if (enemy.landed_){
            count++;
        }
    }
    return count;
}

bool Model::meter_empty() const{
    if (meter_fullness_ == 0){
        return true;
    }
    return false;
}
// Returns the cannon's state.
Cannon Model::get_cannon() const{
    return cannon_;
}
// Returns the states of all the
// logos.
std::vector<Ball> const & Model::get_logos() const{
    return logos_;
}
// Returns the states of all the
// bullets.
std::vector<Ball> const & Model::get_bullets() const{
    return bullets_;
}
// returns speed the icons should be falling at
ge211::Dimensions Model::get_logo_velocity(){
    return logo_vel_;
}
//get meter state, aka rectangle whose dimensions are based on the fullness
ge211::Rectangle Model::get_meter() const{
    return meter_;
}
//return the current state of the campus
ge211::Rectangle Model::get_campus() const{
    return campus_;
}
ge211::Dimensions Model::scene_dims() const{
    return scene_dimensions;
}

///////
//helpers
///////
double Model::position_to_angle(ge211::Position pos){
    double angle = 0.0;

    if (pos.x-cannon_.cannon.x == 0) angle = 0.0;
    else angle = atan2(-(pos.y - cannon_.cannon.center().y),(pos.x-cannon_.cannon.center().x))* 180 / PI;

    return angle;
}

std::tuple<double, double> Model::bullet_velocity(){
    double x = bullet_speed*cos(cannon_.angle*PI/180);
    double y = -bullet_speed*sin(cannon_.angle*PI/180);
    return std::tuple<double, double>(x,y);
}

bool Model::sufficient_time()
{
    if (loops-loop_of_last_shot_>=150 && meter_fullness_!=meter_max)
    {
        return true;
    }
    return false;
}
void Model::reset_meter_()
{
    meter_fullness_ = meter_max;
    meter_={meter_pos.x, meter_pos.y, full_meter_dims.width, meter_fullness_*full_meter_dims.height/meter_max};
}
void Model::sink(int y)
{
    //cannon and logos need to sink with it
    cannon_.cannon.y += y;
    campus_.y += y;
    for (Ball& logo : logos_){
        if (logo.landed_){
            logo.center_.y+=y;
        }
    }
}

bool Model::sunk() const
{
    return campus_.y-cannon_.cannon.height>scene_dimensions.height;
}

// Returns a pointer to a hit
// logo given a bullet, or nullptr if none.
Ball& Model::find_hit_logo_(Ball const & bullet){
    for (Ball& logo : logos_){
        if (bullet.hits_ball(logo)&&!logo.landed_){
            return logo;
        }
    }
    return empty_ball;
}

//Looks through each bullet and checks if it hits a logo.
// Removes all bullets and Logos hit.
void Model::check_and_remove_shots(){
    for (Ball& bullet : bullets_){
        Ball& logo = find_hit_logo_(bullet);
        if (logo != empty_ball){
            //destroy that bullet and logo
            destroy_on_hit_(bullet, logo);
            score++;
        }
    }
}

// Removes the bullet and logo .
void Model::destroy_on_hit_(Ball& bullet, Ball& logo){
    std::swap(bullets_.back(),bullet);
    bullets_.pop_back();

    std::swap(logos_.back(), logo);
    logos_.pop_back();
}

// Removes a bullet or logo when it exits the screen
void Model::remove_on_out(){
    for (Ball& bullet : bullets_){
        if (bullet.hits_bottom(scene_dims())||bullet.hits_left(scene_dims())||bullet.hits_right(scene_dims())||bullet.hits_top(scene_dims())){
            std::swap(bullets_.back(),bullet);
            bullets_.pop_back();
            break;
        }
    }
    for (Ball& logo : logos_){
        if (logo.hits_bottom(scene_dims())&&!logo.landed_){
            std::swap(logos_.back(), logo);
            logos_.pop_back();
            break;
        }
    }
}

// Checks if a logo lands on the campus
bool Model::check_lands(Ball const& b) const{
    return (b.center_.y + logo_radius > campus_.y)&&(b.center_.x>campus_.x&&b.center_.x<campus_.x+campus_.width);
}

// Looks through the logos and finds all that land on the campus and lands them
void Model::land_logos()
{
    for (Ball& b : logos_)
    {
        if (check_lands(b.next())&&!b.landed_)
        {
            b=b.next();
            b.landed_ = true;
        }
    }
}

//returns true if there are more than max_landed_logos on the campus
bool Model::check_invaded(){
    if (count_landed()>=max_landed_logos){
        return true;
    }
    return false;
}
