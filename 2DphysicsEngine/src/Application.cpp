#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include <iostream>
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    anchor = Vec2(Graphics::Width() / 2, 30);


    Body* boxA = new Body(BoxShape(200,200), Graphics::Width()/2, Graphics::Height() / 2, 1.0);
    Body* boxB = new Body(BoxShape(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0);

    boxA->angularVelocity = 0.4;
    boxB->angularVelocity = 0.1;

   
	bodies.push_back(boxA);
    bodies.push_back(boxB);
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
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    bodies[0]->position.x = x;
                    bodies[0]->position.y = y;
					break;
            //case SDL_MOUSEMOTION:
            //    mouseCursor.x = event.motion.x;
            //    mouseCursor.y = event.motion.y;
            //    break;
            //case SDL_MOUSEBUTTONDOWN:
            //    if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //        leftMouseButtonDown = true;
            //        int x, y;
            //        SDL_GetMouseState(&x, &y);
            //        mouseCursor.x = x;
            //        mouseCursor.y = y;
            //    }
            //    break;
            //case SDL_MOUSEBUTTONUP:
            //    if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //        leftMouseButtonDown = false;
            //        Vec2 impulseDirection = (bodies[0]->position - mouseCursor).UnitVector();
            //        float impulseMagnitude = (bodies[0]->position - mouseCursor).Magnitude() * 5.0;
            //        bodies[0]->velocity = impulseDirection * impulseMagnitude;
            //    }
            //    break;
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
    Graphics::ClearScreen(0xFF0b0f3b);

    // Limit the delta time to 15 milliseconds
    if (deltaTime > 0.015f)
		deltaTime = 0.015f;

    for (auto body : bodies) {        
        //body->AddForce(pushForce);

        //Vec2 drag = Force::GenerateDragForce(*body, 0.001);
        //body->AddForce(drag);

        //Vec2 weight =  Vec2(0, 9.8) * body->mass * PIXELS_PER_METER;
        //body->AddForce(weight);

        //Vec2 wind = Vec2(20 * PIXELS_PER_METER, 0);
        //body->AddForce(wind);

    }

    for (auto body : bodies) {
        body->Update(deltaTime);
    }

    for (auto body : bodies) {
        body->isColiding = false;
    }


    //Check collisions
    for (int i = 0; i < bodies.size() - 1; i++) {
        for (int j = i+1; j < bodies.size(); j++)
        {
            Body* bodyA = bodies[i];
            Body* bodyB = bodies[j];

            Contact contact;
            if (CollisionDetection::IsColliding(bodyA, bodyB, contact)) {
             
                //contact.ResolveCollision();


                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 20, contact.start.y + contact.normal.y * 20, 0xFFFFFFFF);
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 5, 0xFFFFFFFF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 5, 0xFFFFFFFF);

                bodyA->isColiding = true;
				bodyB->isColiding = true;

                
            }
        }
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

   

    for(auto body : bodies) {

        Uint32 color = body->isColiding ? 0xFF0000FFF : 0xFFFFFFFF;

        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circle = (CircleShape*)body->shape;
			Graphics::DrawCircle(body->position.x, body->position.y, circle->radius, body->rotation , color);
		}
        else if(body->shape->GetType() == BOX){
            BoxShape* box = (BoxShape*)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, box->worldVertices, color);
		
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