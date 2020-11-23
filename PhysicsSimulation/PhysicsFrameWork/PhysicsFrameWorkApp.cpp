#include "PhysicsFrameWorkApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "OBB.h"
#include "CueBall.h"
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

PhysicsFrameWorkApp::PhysicsFrameWorkApp() {

}

PhysicsFrameWorkApp::~PhysicsFrameWorkApp() {

}

bool PhysicsFrameWorkApp::startup() {
	//increase the 2d line count to maximize the number of objecs we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.001f);

	//table walls
	OBB* box2 = new OBB(glm::vec2(-30, -25), 50,4,  glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);
	OBB* box3 = new OBB(glm::vec2(30, -25),  50,4,  glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);
	OBB* box4 = new OBB(glm::vec2(-60, 0),	 4, 40, glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);
	OBB* box5 = new OBB(glm::vec2(60, 0),	 4, 40, glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);
	OBB* box6 = new OBB(glm::vec2(30, 25),   50,4,  glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);
	OBB* box7 = new OBB(glm::vec2(-30, 25),  50,4,  glm::vec2(0, 0), 100000.0f, glm::vec4(0, 1, 0, 1),  true);


	//cue ball
	ball1 = new CueBall(glm::vec2(-40, 0) , glm::vec2(100, 0) , 2.8f, 2); 
	//Red balls
	Sphere* ball3 = new Sphere(glm::vec2(20, 0),  glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball9 = new Sphere(glm::vec2(23, -1), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball10 = new Sphere(glm::vec2(23, 1), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball4 = new Sphere(glm::vec2(26, 0),  glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball5 = new Sphere(glm::vec2(26, 3),  glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball11 = new Sphere(glm::vec2(26, -3),glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball6 = new Sphere(glm::vec2(29, -1), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball12 = new Sphere(glm::vec2(29, 1), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1));
	Sphere* ball13 = new Sphere(glm::vec2(29, -4),glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball14 = new Sphere(glm::vec2(29, 4), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball7 = new Sphere(glm::vec2(31, 0),  glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball8 = new Sphere(glm::vec2(31, 3),  glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball15 = new Sphere(glm::vec2(31, -3),glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball16 = new Sphere(glm::vec2(31, 6), glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 
	Sphere* ball17 = new Sphere(glm::vec2(31, -6),glm::vec2(0, 0), 2.0f, 2, glm::vec4(1, 0, 0, 1)); 

	//add walls to scene
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(box4);
	m_physicsScene->AddActor(box5);
	m_physicsScene->AddActor(box6);
	m_physicsScene->AddActor(box7);

	//add balls to scene
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(ball6);
	m_physicsScene->AddActor(ball7);
	m_physicsScene->AddActor(ball8);
	m_physicsScene->AddActor(ball9);
	m_physicsScene->AddActor(ball10);
	m_physicsScene->AddActor(ball11);
	m_physicsScene->AddActor(ball12);
	m_physicsScene->AddActor(ball13);
	m_physicsScene->AddActor(ball14);
	m_physicsScene->AddActor(ball15);
	m_physicsScene->AddActor(ball16);
	m_physicsScene->AddActor(ball17);

	return true;
}

void PhysicsFrameWorkApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsFrameWorkApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();


	double x = 0,  y = 0;
	glfwGetCursorPos(getWindowPtr(), &x, &y);
	ball1->GetMousePosition(glm::vec2(x, y));
	
	aie::Gizmos::clear();
	//update physics
	m_physicsScene->Update(deltaTime);
	//create the start pos dot
	aie::Gizmos::add2DCircle(glm::vec2(-40, 0), 0.5f, 12, glm::vec4(0, 0, 0, 1));
	//update visual representations of objects
	m_physicsScene->UpdateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

}

void PhysicsFrameWorkApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	setBackgroundColour(0.1f, 0.5f, 0.1f, 1);
	// draw your stuff here!
	static float aspectRatio = 21 / 9;

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	char result[64];
	snprintf(result, 64, "Score: %i", m_physicsScene->GetScore());
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, result, 0, 0);

	// done drawing sprites
	m_2dRenderer->end();

}