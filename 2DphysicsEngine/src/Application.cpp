#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    anchor = Vec2(Graphics::Width() / 2, 30);


    Body* box = new Body(BoxShape(200,100), Graphics::Width() / 2, Graphics::Height() / 2, 2.0);
	bodies.push_back(box);
    
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;    
                if (event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = -50 * PIXELS_PER_METER;
				if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_DOWN)  
					pushForce.y = 0;
				if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = 0;
                break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (bodies[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (bodies[0]->position - mouseCursor).Magnitude() * 5.0;
                    bodies[0]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    static int timePreviousFrame = 0;

    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    timePreviousFrame = SDL_GetTicks();

    // Limit the delta time to 15 milliseconds
    if (deltaTime > 0.015f)
		deltaTime = 0.015f;

    for (auto body : bodies) {        
        body->AddForce(pushForce);

        Vec2 drag = Force::GenerateDragForce(*body, 0.001);
        body->AddForce(drag);

        //Vec2 weight =  Vec2(0, 9.8) * body->mass * PIXELS_PER_METER;
        //body->AddForce(weight);

        float torque = 2000.f;
        body->AddTorque(torque);
    }

    for (auto body : bodies) {
        body->Update(deltaTime);
    }

    //checkBoundaries of the windows
    for (auto body : bodies) {

        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circle = (CircleShape*)body->shape;
            //Making shure the body doesn't go out of the screen
            if (body->position.y + circle->radius > Graphics::Height()) {
                body->position.y = Graphics::Height() - circle->radius;
                body->velocity.y *= -BOUNCIENESS;
            }
            else if (body->position.y < 0) {
                body->position.y = circle->radius;
                body->velocity.y *= -BOUNCIENESS;
            }

            if (body->position.x + circle->radius > Graphics::Width()) {
                body->position.x = Graphics::Width() - circle->radius;
                body->velocity.x *= -BOUNCIENESS;
            }
            else if (body->position.x - circle->radius < 0) {
                body->position.x = circle->radius;
                body->velocity.x *= -BOUNCIENESS;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0b0f3b);

    if (leftMouseButtonDown) {
        Graphics::DrawLine(bodies[0]->position.x, bodies[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }

    for(auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circle = (CircleShape*)body->shape;
			Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation ,0xFFFFFFFF);
		}
        else if(body->shape->GetType() == BOX){
            BoxShape* box = (BoxShape*)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, box->worldVertices, 0xFFFFFFFF);
		
        }
	}


    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto body : bodies)
		delete body;

    Graphics::CloseWindow();
}