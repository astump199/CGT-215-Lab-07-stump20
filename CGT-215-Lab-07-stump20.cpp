// CGT-215-Lab-07-stump20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

int main()
{
    // 1. Create window (800x600) and physics world with downward gravity (0, 1)
    RenderWindow window(VideoMode(800, 600), "Bounce");
    World world(Vector2f(0, 1));

    // 2. Create the ball (placed near top-center to allow clear trajectory)
    PhysicsCircle ball;
    ball.setCenter(Vector2f(400, 100));
    ball.setRadius(20);
    // Apply an initial velocity impulse so it bounces around and hits the center block
    ball.applyImpulse(Vector2f(0.1f, 0.05f));
    world.AddPhysicsBody(ball);

    // 3. Create boundaries (Floor, Ceiling, Left Wall, Right Wall)

    // Floor
    PhysicsRectangle floor;
    floor.setSize(Vector2f(800, 20));
    floor.setCenter(Vector2f(400, 590));
    floor.setStatic(true);
    world.AddPhysicsBody(floor);

    // Ceiling
    PhysicsRectangle ceiling;
    ceiling.setSize(Vector2f(800, 20));
    ceiling.setCenter(Vector2f(400, 10));
    ceiling.setStatic(true);
    world.AddPhysicsBody(ceiling);

    // Left Wall
    PhysicsRectangle leftWall;
    leftWall.setSize(Vector2f(20, 600));
    leftWall.setCenter(Vector2f(10, 300));
    leftWall.setStatic(true);
    world.AddPhysicsBody(leftWall);

    // Right Wall
    PhysicsRectangle rightWall;
    rightWall.setSize(Vector2f(20, 600));
    rightWall.setCenter(Vector2f(790, 300));
    rightWall.setStatic(true);
    world.AddPhysicsBody(rightWall);

    // 4. Create the center obstacle box
    PhysicsRectangle centerBox;
    centerBox.setSize(Vector2f(100, 100));
    centerBox.setCenter(Vector2f(400, 300));
    centerBox.setStatic(true);
    world.AddPhysicsBody(centerBox);

    // 5. Collision Callbacks

    // Floor collision callback ("thud 1", "thud 2", ...)
    int thudCount = 1;
    floor.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };

    // Center box collision callback ("bang 1", "bang 2", "bang 3") — exits after 3 hits
    int bangCount = 1;
    centerBox.onCollision = [&bangCount, &window](PhysicsBodyCollisionResult result) {
        cout << "bang " << bangCount << endl;
        if (bangCount >= 3) {
            cout << "Center box struck 3 times. Exiting..." << endl;
            window.close();
        }
        bangCount++;
        };

    // 6. Game Loop
    Clock clock;
    Time lastTime = clock.getElapsedTime();

    while (window.isOpen()) {
        // Handle window closing events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Calculate delta time in milliseconds
        Time currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        int deltaTimeMS = deltaTime.asMilliseconds();

        if (deltaTimeMS > 0) {
            world.UpdatePhysics(deltaTimeMS);
            lastTime = currentTime;
        }

        // Render frame
        window.clear(Color(0, 0, 0));
        window.draw(ball);
        window.draw(floor);
        window.draw(ceiling);
        window.draw(leftWall);
        window.draw(rightWall);
        window.draw(centerBox);
        window.display();
    }

    return 0;
}