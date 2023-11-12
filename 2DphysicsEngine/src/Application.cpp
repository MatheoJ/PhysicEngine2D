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

    anchor = Vec2(Graphics::Width() / 2, 30);

    int numParticles = 15;
    int particlesSpacing = restLength;

    for (int i = 0; i < numParticles; i++) {
        Particle* particle = new Particle(Graphics::Width() / 2, (numParticles+1)*particlesSpacing - (particlesSpacing * i), 2.0);
		particle -> radius = 5;
		particles.push_back(particle);
    }
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
                    Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
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

        Vec2 drag = Force::GenerateDragForce(*particle, 0.001);
        particle->AddForce(drag);

        Vec2 weight =  Vec2(0, 9.8) * particle->mass * PIXELS_PER_METER;
        particle->AddForce(weight);
    }

    for (int i = 0; i < particles.size() - 1; i++) {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], *particles[i + 1], restLength, k);
		particles[i]->AddForce(springForce);
		particles[i + 1]->AddForce(springForce * -1);
    }

    particles[particles.size() - 1]->AddForce(Force::GenerateSpringForce(*particles[particles.size() - 1], anchor, restLength, k));


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

    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }

    for (int i = 0; i < particles.size() - 1; i++) {
        Graphics::DrawLine(particles[i]->position.x, particles[i]->position.y, particles[i + 1]->position.x, particles[i + 1]->position.y, 0xFF313131);
		Graphics::DrawFillCircle(particles[i]->position.x, particles[i]->position.y, particles[i]->radius, 0xFFFFFFFF);
    }

    Graphics::DrawFillCircle(particles[particles.size() - 1]->position.x, particles[particles.size() - 1]->position.y, particles[particles.size() - 1]->radius, 0xFFFFFFFF);
    Graphics::DrawLine(anchor.x, anchor.y, particles[particles.size() - 1]->position.x, particles[particles.size() - 1]->position.y, 0xFF313131);

    // Draw the anchor
    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF313131);

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