#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;
    particles.push_back(smallBall);

    Particle* bigBall = new Particle(400, 400, 10.0);
    bigBall->radius = 35;
    particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
					Particle* newParticle = new Particle(event.button.x, event.button.y, 1.0);
					newParticle->radius = 4;
					particles.push_back(newParticle);
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

    for (auto particle : particles) {

        
        particle->AddForce(pushForce);

        Vec2 friction = Force::GenerateFrictionForce(*particle, 5);
        particle->AddForce(friction);
    }

    Vec2 gravitationalForce = Force::GenerateGravitaionalForce(*particles[0], *particles[1], 2000, 5, 100);
    particles[0]->AddForce(gravitationalForce);
    particles[1]->AddForce(gravitationalForce * -1);

    for (auto particle : particles) {
        particle->Integrate(deltaTime);

        //Making shure the particle doesn't go out of the screen
        if (particle->position.y + particle->radius > Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -BOUNCIENESS;
        }
        else if (particle->position.y < 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -BOUNCIENESS;
        }

        if (particle->position.x + particle->radius > Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -BOUNCIENESS;
        }
        else if (particle->position.x - particle->radius < 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -BOUNCIENESS;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0b0f3b);

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFA65828);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF2EAAB8);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : particles)
		delete particle;

    Graphics::CloseWindow();
}