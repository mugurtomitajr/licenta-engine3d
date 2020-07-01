//
// Created by Mugur Tomita Jr. on 19/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_EXAMPLESCENE_H
#define ENGINE_EXAMPLESCENE_H

#include "../../base/space/Camera.h"

#include "../../base/scene/Scene.h"
#include "../../base/model/ViewContainer.h"
#include "../../base/model/Button.h"
#include "../../base/model/Canvas.h"

#include "../global/menu/Menu.h"
#include "../global/menu/MenuItem.h"

class ExampleScene : public Scene {
private:
    Camera *mainCamera = nullptr;
    Camera *secondaryCamera = nullptr;
    Space *mainSpace = nullptr;

    Menu *menu = nullptr;
    ViewContainer *container = nullptr;
    ViewContainer *footer = nullptr;

public:
    ExampleScene() : Scene() {

    }

    ExampleScene(const ExampleScene &that) : Scene(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    ExampleScene &operator=(const ExampleScene &that) {
        Scene::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~ExampleScene() {
        this->clear();
    }

    void clear() {
        Scene::clear();
    }

    void copy(const ExampleScene &that) {
        Scene::copy(that);
    }

    void UpdateSpaces(const double &dt) {
        this -> mainSpace -> Update(dt);
    };

    void RenderSpaces(const double &dt, const double &inter) {

    };
    void RenderCameras(const double &dt, const double &inter) {
        this -> mainCamera -> Render(dt, inter);
        this -> secondaryCamera -> Render(dt, inter);
    };

    void onLeftButtonClick(std::string buttonId) {
        this -> Broadcast(NOTIFY_TAG_SET_MESSAGE_TEMPORARY, "Button left pressed: " + buttonId);

        if(!buttonId.compare("quit")) {
            this->Broadcast(NOTIFY_TAG_QUIT_APPLICATION, "Bye!");
        }
    }

    void onRightButtonClick(std::string buttonId) {
        this -> Broadcast(NOTIFY_TAG_SET_MESSAGE_TEMPORARY, "Button right pressed: " + buttonId);
    }

    void Initialise(SDL_Renderer *p_renderer) {
        this -> mainCamera = new Camera(TriVector(0, 200, 0), MAIN_CAMERA_DIMENSIONS, MAIN_CAMERA_FIELD_OF_VIEW, MAIN_CAMERA_NEAR, MAIN_CAMERA_FAR);
        this -> secondaryCamera = new Camera(TriVector(0, 120, -100), MAIN_CAMERA_DIMENSIONS, PI_1o4, MAIN_CAMERA_NEAR, MAIN_CAMERA_FAR);
        this -> mainSpace = new Space();

        this -> mainCamera -> bindSpace(this -> mainSpace);
        this -> secondaryCamera -> bindSpace(this -> mainSpace);

        this -> uiMessage = new Label((this -> currentUiMessage).c_str(), 42, Color::WHITE(), BiVector(0, 0), BiLine(BiVector(20, 10), BiVector(20, 10)));
        this -> renderFps = new Label("Render: ...;", 42, Color::WHITE(), BiVector(0, 0), BiLine(BiVector(20, 10), BiVector(10, 10)));
        this -> updateFps = new Label("Update: ...;", 42, Color::WHITE(), BiVector(0, 0), BiLine(BiVector(10, 10), BiVector(20, 10)));

        this -> menu = new Menu();

        this -> menu->addMenuItem((new MenuItem("new_camera_normal", "New Camera - Projection"))
                                  -> setOnLeftClick(this, &AliveObject::onLeftButtonClick)
                                  -> setOnRightClick(this, &AliveObject::onRightButtonClick));

        this -> menu->addMenuItem((new MenuItem("new_camera_ray", "New Camera - Ray Tracing"))
                                  -> setOnLeftClick(this, &AliveObject::onLeftButtonClick)
                                  -> setOnRightClick(this, &AliveObject::onRightButtonClick));

        this -> menu->addMenuItem((new MenuItem("new_space", "New Space"))
                                  -> setOnLeftClick(this, &AliveObject::onLeftButtonClick)
                                  -> setOnRightClick(this, &AliveObject::onRightButtonClick));

        this -> menu->addMenuItem((new MenuItem("quit", "Quit"))
                                          -> setOnLeftClick(this, &AliveObject::onLeftButtonClick)
                                          -> setOnRightClick(this, &AliveObject::onRightButtonClick));

        this -> container = new ViewContainer(
                FlexType::Row,
                ExpandType::Fill,
                GrowType ::Grow,
                BiVector(),
                BiLine(BiVector(20, 10), BiVector(20, 10)),
                Color::TRANSPARENT());

        // Main Camera
        ViewContainer *mainCameraContainer = new ViewContainer(
                FlexType::Column,
                ExpandType::Wrap,
                GrowType ::Grow,
                BiVector(),
                BiLine(BiVector(), BiVector()),
                Color::TRANSPARENT());

        Label *mainCameraLabel = new Label(
                "Camera 1 -> Space 1",
                42, APPLICATION_COLOR_DARK,
                BiVector(0, 0),
                BiLine(BiVector(0, 10), BiVector(0, 10)));

        Canvas *mainCameraCanvas = new Canvas(
                BiVector(),
                BiLine(BiVector(), BiVector()),
                BiVector(MAIN_CAMERA_CANVAS_DIMENSIONS.intX(), MAIN_CAMERA_CANVAS_DIMENSIONS.intY()),
                Color::TRANSPARENT());

        mainCameraCanvas -> bindCamera(this -> mainCamera);

        mainCameraContainer -> addChild(new ViewContainer(
                FlexType::Column,
                ExpandType::Fill,
                GrowType ::Grow,
                BiVector(),
                BiLine(BiVector(), BiVector()),
                Color::TRANSPARENT()));

        mainCameraContainer -> addChild(mainCameraCanvas);

        mainCameraContainer -> addChild(mainCameraLabel);

        this -> container -> addChild(mainCameraContainer);


        // Secondary Camera
        ViewContainer *secondaryCameraContainer = new ViewContainer(
                FlexType::Column,
                ExpandType::Wrap,
                GrowType ::Grow,
                BiVector(),
                BiLine(BiVector(20, 0), BiVector(20, 0)),
                Color::TRANSPARENT());

        Label *secondaryCameraLabel = new Label(
                "Camera 2 -> Space 1",
                42, APPLICATION_COLOR_DARK,
                BiVector(0, 0),
                BiLine(BiVector(0, 10), BiVector(0, 10)));

        Canvas *secondaryCameraCanvas = new Canvas(
                BiVector(),
                BiLine(BiVector(), BiVector()),
                BiVector(MAIN_CAMERA_CANVAS_DIMENSIONS.intX(), MAIN_CAMERA_CANVAS_DIMENSIONS.intY()),
                Color::TRANSPARENT());

        secondaryCameraCanvas -> bindCamera(this -> secondaryCamera);

        secondaryCameraContainer -> addChild(new ViewContainer(
                FlexType::Column,
                ExpandType::Fill,
                GrowType ::Grow,
                BiVector(),
                BiLine(BiVector(), BiVector()),
                Color::TRANSPARENT()));

        secondaryCameraContainer -> addChild(secondaryCameraCanvas);

        secondaryCameraContainer -> addChild(secondaryCameraLabel);

        this -> container -> addChild(secondaryCameraContainer);

        this -> footer = new ViewContainer(
                FlexType::Row,
                ExpandType::Wrap,
                GrowType::Grow,
                BiVector(),
                BiLine(BiVector(20, 20), BiVector(20, 10)),
                APPLICATION_COLOR_DARK);

        ViewContainer *messageContainer = new ViewContainer(FlexType::Column,ExpandType::Fill,GrowType ::Grow,BiVector(),BiLine(BiVector(), BiVector()), Color::TRANSPARENT());
        messageContainer->addChild(this -> uiMessage);

        this -> footer -> addChild(messageContainer);
        this -> footer -> addChild(this -> updateFps);
        this -> footer -> addChild(this -> renderFps);

        this -> addChild(footer);
        this -> addChild(container);
        this -> addChild(menu);

        this -> mainSpace -> Initialize();

        //this -> delegateTo(this -> mainCamera);
    }

    void verifyKeyboard(const double &dt, const Uint8 *keyboardState) {
        if (keyboardState[SDL_SCANCODE_0]) {
            this -> delegateTo(this -> mainCamera);
        } else if (keyboardState[SDL_SCANCODE_1]) {
            this -> delegateTo(this->secondaryCamera);
        }
    }
};

#endif // ENGINE_EXAMPLESCENE_H
