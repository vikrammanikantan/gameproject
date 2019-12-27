#include "model.h"
#include <ge211.h>
#include <catch.h>
#include <cmath>

struct Test_access {

    Model& m_;

    int meter_fullness()
    {
        return m_.meter_fullness_;
    }

    void set_meter_fullness(int meter)
    {
        m_.meter_fullness_ = meter;
    }

    Ball& find_hit_logo(Ball& b)
    {
        return m_.find_hit_logo_(b);
    }

    bool check_lands(Ball& b)
    {
        return m_.check_lands(b);
    }

    bool sufficient_time()
    {
        return m_.sufficient_time();
    }

    void drop_logo ()
    {
        m_.drop_logo(scene_dimensions.width/2,0);
    }

    std::vector<Ball>& get_logos()
    {
        return m_.logos_;
    }

    std::vector<Ball>& get_bullets()
    {
        return m_.bullets_;
    }

    void collide()
    {
        m_.check_and_remove_shots();
    }

    void set_last_shot(int t)
    {
        m_.loop_of_last_shot_ = t;
    }
};

TEST_CASE("Game start")
{
    Model m{};
    Test_access t{m};

    CHECK(m.get_cannon().angle == 90);
    CHECK(t.meter_fullness() == meter_max);
    CHECK(t.m_.get_campus() == ge211::Rectangle{scene_dimensions.width/5, scene_dimensions.height-20, scene_dimensions.width*3/5, 20});

    CHECK(m.get_cannon().cannon.dimensions() == cannon_dims);
    CHECK(m.get_meter().top_left() == meter_pos);

    CHECK(m.get_bullets().empty());
    CHECK(m.get_logos().empty());

    CHECK(m.count_landed()==0);
}

TEST_CASE("Collision tests")
{
    Model m;
    Test_access t{m};

    m.launch_ball();
    CHECK(m.get_bullets().size() == 1);
    Ball& blt = t.get_bullets().at(0);
    CHECK(blt.radius_ == m.bullet_radius);

    m.drop_logo(5,0);
    CHECK(m.get_logos().size() == 1);
    Ball& logo = t.get_logos().at(0);

    blt.center_.x = 10;
    blt.center_.y = 10;
    CHECK(blt.center_.x ==10);
    CHECK(blt.center_.y == 10);

    logo.center_.x = 100;
    logo.center_.y = 100;
    CHECK(logo.center_.x ==100);
    CHECK(logo.center_.y == 100);

    //the logo and the bullet are far away and shouldn't collide
    CHECK_FALSE(logo.hits_ball(blt));

    //updating the game state so the bullet and the logo are close together
    logo.center_.x = 11;
    logo.center_.y = 11;
    CHECK(logo.center_.x ==11);
    CHECK(logo.center_.y == 11);

    CHECK(logo.hits_ball(blt));

}

TEST_CASE("Check Ball Functions")
{
    Model m;

    m.cannon_to({m.get_cannon().cannon.x, scene_dimensions.height/2});

    m.launch_ball();
    m.update(0,0);

    CHECK(m.get_bullets().size() == 1);
    Ball blt = m.get_bullets().at(0);

    CHECK(blt.radius_ == m.bullet_radius);

    Ball oldBlt = blt;

    blt = blt.next();
    CHECK(blt.top_left().x == oldBlt.top_left().x + bullet_speed);
    CHECK(blt.top_left().y == oldBlt.top_left().y);

    blt = blt.next();
    CHECK(blt.top_left().x == oldBlt.top_left().x + 2*bullet_speed);
    CHECK(blt.top_left().y == oldBlt.top_left().y);

    m.drop_logo(0,0);
    CHECK(m.get_logos().size() == 2);
    Ball logo = m.get_logos().at(0);

    CHECK(logo.radius_ == m.logo_radius);

    Ball oldLog = logo;

    logo = logo.next();
    CHECK(logo.top_left().x == oldLog.top_left().x + std::get<0>(oldLog.velocity_));
    CHECK(logo.top_left().y == oldLog.top_left().y + std::get<1>(oldLog.velocity_));
}

TEST_CASE("Meter Test")
{
    Model m;
    Test_access t {m};

    t.m_.update(0,0);
    t.m_.launch_ball();

    for (int x = 0; x < 100; x++)
    {
        t.m_.update(0,0);
    }
    //there shouldn't have been enough time yet
    CHECK_FALSE(t.sufficient_time());
    t.set_meter_fullness(meter_max);

    for (int x = 0; x < 10; x++)
    {
        t.m_.launch_ball();
        t.m_.update(0,0);
    }

    CHECK(t.m_.meter_empty());
    CHECK_FALSE(t.sufficient_time());

    for (int x = 0; x< 149; x++)
    {
        t.m_.update(10,0);
    }

    //enough time should've passed by now
    CHECK(t.sufficient_time());

}

TEST_CASE("Campus Check")
{
    Model m;
    Test_access t {m};

    m.drop_logo(scene_dimensions.width/2,0);
    m.update(scene_dimensions.width/2,0);

    //using a pointer to the logo just dropped
    Ball* b = &t.get_logos().at(0);

    //checking that the logo will fall on the center of the campus
    CHECK(b->center_.x == scene_dimensions.width/2);

    //the logo should be very far from the campus at this point
    CHECK_FALSE(t.check_lands(*b));

    //updating the model far enough so that the logo falls onto the campus
    for (int x = 0; x < 1000; x++)
    {
        t.m_.update(scene_dimensions.width/2,0);
    }

    CHECK(t.check_lands(t.get_logos().at(0)));
}

TEST_CASE("Cannon Check")
{
    Model m {};

    CHECK(m.get_cannon().angle == 90);

    m.cannon_to({5,-5});

    int toX = 5;
    int toY = -5;

    double angle = atan2(-(toY - m.get_cannon().cannon.center().y),(toX-m.get_cannon().cannon.center().x))* 180 / PI;

    m.update(0,0);

    CHECK(m.get_cannon().angle == angle);
}



//Ball test cases
TEST_CASE("Ball::hits_side")
{
    Model m;

    Ball ball({m.bullet_radius+1, m.bullet_radius+1}, {-1,0}, m.bullet_radius, 3);

    CHECK_FALSE( ball.hits_left(scene_dimensions));
    CHECK_FALSE( ball.hits_right(scene_dimensions));

    ball.center_.x += -2;
    CHECK( ball.hits_left(scene_dimensions));
    CHECK_FALSE( ball.hits_right(scene_dimensions));

    ball.center_.x = scene_dimensions.width - m.bullet_radius + 1;
    CHECK_FALSE(ball.hits_left(scene_dimensions));
    CHECK( ball.hits_right(scene_dimensions));


    ball.center_.x = scene_dimensions.width/2;
    CHECK_FALSE( ball.hits_right(scene_dimensions));
    CHECK_FALSE( ball.hits_left(scene_dimensions));
}

TEST_CASE("Ball::hits_bottom")
{
    Model m;
    Ball ball({scene_dimensions.width/2, scene_dimensions.height - m.bullet_radius -1}, {0,1}, m.bullet_radius, 3);

    CHECK_FALSE(ball.hits_left(scene_dimensions));
    CHECK_FALSE(ball.hits_right(scene_dimensions));

    CHECK_FALSE(ball.hits_bottom(scene_dimensions));
    ball.center_.y = scene_dimensions.height - ball.radius_ + 1 ;
    CHECK( ball.hits_bottom(scene_dimensions) );
    ball.center_.y = scene_dimensions.height/2 ;
    CHECK_FALSE(ball.hits_bottom(scene_dimensions));
}

TEST_CASE("Ball::hits_top")
{
    Model m;
    Ball ball({scene_dimensions.width/2, m.bullet_radius + 1}, {0,-1}, m.bullet_radius, 3);

    CHECK_FALSE(ball.hits_left(scene_dimensions));
    CHECK_FALSE(ball.hits_right(scene_dimensions));

    CHECK_FALSE(ball.hits_top(scene_dimensions) );
    ball.center_.y = ball.radius_ - 1 ;
    CHECK( ball.hits_top(scene_dimensions) );
    ball.center_.y = scene_dimensions.height/2 ;
    CHECK_FALSE(ball.hits_top(scene_dimensions));

}

TEST_CASE("Ball::next")
{
    Model m;
    Ball ball({scene_dimensions.width/2, scene_dimensions.height - m.bullet_radius -1}, {3,4}, m.bullet_radius, 3);

    Ball oldBall (ball);

    ball = ball.next();

    CHECK(ball.center_.x == oldBall.center_.x + 3);
    CHECK(ball.center_.y == oldBall.center_.y + 4);
}

TEST_CASE("Shooting Landed Logos")
{
    Model m {};
    Test_access t {m};

    //going to create a logo in m, and set its status to landed
    t.get_logos().push_back(Ball(ge211::Position{scene_dimensions.width/2, scene_dimensions.height/2},
                                 {0, 0}, m.logo_radius, 0));
    Ball* logo = &t.get_logos().at(0);

    logo->landed_ = true;
    t.m_.update(0,0);
    CHECK(t.m_.count_landed() == 1);

    //creating a bullet that's in contact with the logo
    t.get_bullets().push_back(Ball(ge211::Position{scene_dimensions.width/2 + m.logo_radius - 1,
                                                   scene_dimensions.height/2 + m.logo_radius - 1},
                                   {0, 0}, m.bullet_radius, 0));

    Ball* bullet = &t.get_bullets().at(0);

    t.m_.update(0,0);
    CHECK(logo->hits_ball(*bullet));

    //now "colliding" them and showing that the logo doesn't go away.
    t.collide();

    //checking that the set of bullets and logos remains unchanged

    CHECK_FALSE(t.get_bullets().empty());
    CHECK_FALSE(t.get_logos().empty());
    CHECK(t.m_.count_landed() == 1);
}

TEST_CASE("Dropping Logos")
{

    Model m {};

    for (int x = 0; x < 200; x++) {
        m.update(scene_dimensions.width/2, 0);
    }

    //3 logos shouldve been dropped automatically in this time period
    CHECK(m.get_logos().size() == 3);

    for (int x = 0; x < 50; x++) {
        m.update(scene_dimensions.width/2, 0);
    }
    //4 logos shouldve been dropped automatically in this total time period
    CHECK(m.get_logos().size() == 4);

}