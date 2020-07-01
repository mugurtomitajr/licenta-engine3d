//
// Created by Mugur Tomita Jr. on 18/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_VIEWCONTAINER_H
#define ENGINE_VIEWCONTAINER_H

#include "UIVisibleObject.h"
#include "shape/Rectangle.h"

#include "../../support/Util.h"
#include "../drawer/Drawer.h"

class ViewContainer : public UIVisibleObject {
public:
    enum FlexType {Column, Row};

    enum ExpandType {Wrap, Fill};
    enum GrowType {Contain, Grow};

    bool layoutUpdated = false;

    FlexType flexType = Column;
    ExpandType expandType = Wrap;
    GrowType growType = Contain;

    BiVector usedSpace;
    int nbFillChildren = 0;

    ViewContainer(FlexType p_flexType, ExpandType p_expandType, GrowType p_growType, BiVector p_position, BiLine p_margin, Color backgroundColor) : UIVisibleObject(p_position, p_margin, new Rectangle(BiVector(), BiVector(), backgroundColor)) {
        this -> flexType = p_flexType;
        this -> growType = p_growType;
        this -> expandType = p_expandType;
    }

    ViewContainer(const ViewContainer &that) : UIVisibleObject(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    ViewContainer &operator=(const ViewContainer &that) {
        UIVisibleObject::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~ViewContainer() {
        this->clear();
    }

    void clear() {
        UIVisibleObject::clear();
    }

    void Update(double dt) {
        if(this -> layoutUpdated) {
            this -> resetLayout();
            this -> fillLayout();
            this -> rearrangeFilledLayout();
        }
        UIVisibleObject::Update(dt);
    };

    void Render(double dt, double inter) {
        UIVisibleObject::Render(dt, inter);
    }

    ViewContainer* setUsedSpace(const BiVector &p_usedSpace) {
        this -> usedSpace = p_usedSpace;
        return this;
    }

    BiVector getUsedSpace() const {
        return this -> usedSpace;
    }

    void resetLayout() {
        // if this is set to WRAP => it needs to calculate own dimensions

        // if this is set to FILL => it already has dimensions
        double previousX = 0.0f;
        double previousY = 0.0f;

        this -> nbFillChildren = 0;

        if(this -> flexType == FlexType::Row) {
            double currentX = 0.0f;
            double maxY = 0.0f;
            for (auto &it : this -> children) {
                if(!it -> hasStarted) {
                    it -> Start(this -> renderer);
                }
                if(IsType<ViewContainer, AliveObject>(it)) {
                    it -> resetLayout();
                    if(ViewContainer* casted = dynamic_cast<ViewContainer*>(it)) {
                        if(casted -> expandType == ExpandType::Fill) {
                            this -> nbFillChildren += 1;
                        }
                    }
                }

                previousX = (it->getMargin()).source.x;
                previousY = (it->getMargin()).source.y;
                it -> setPosition(BiVector(previousX + currentX, previousY));

                currentX += previousX + (it -> getUsedSpace()).x + (it->getMargin()).destination.x;

                if((it -> getUsedSpace()).y + previousY + (it->getMargin()).destination.y > maxY) {
                    maxY = (it -> getUsedSpace()).y + previousY + (it->getMargin()).destination.y;
                }
            }

            this -> setDimensions(BiVector(currentX, maxY));
            this -> setUsedSpace(BiVector(currentX, maxY));

        } else if(this -> flexType == FlexType::Column) {
            double currentY = 0.0f;
            double maxX = 0.0f;
            for (auto &it : this -> children) {
                if(!it -> hasStarted) {
                    it -> Start(this -> renderer);
                }
                if(IsType<ViewContainer, AliveObject>(it)) {
                    it -> resetLayout();
                    if(ViewContainer* casted = dynamic_cast<ViewContainer*>(it)) {
                        if(casted -> expandType == ExpandType::Fill) {
                            this -> nbFillChildren += 1;
                        }
                    }
                }

                previousX = (it->getMargin()).source.x;
                previousY = (it->getMargin()).source.y;
                it -> setPosition(BiVector(previousX, previousY + currentY));

                currentY += previousY + (it -> getUsedSpace()).y + (it->getMargin()).destination.y;

                if((it -> getUsedSpace()).x + previousX + (it->getMargin()).destination.x > maxX) {
                    maxX = previousX + (it -> getUsedSpace()).x + (it->getMargin()).destination.x;
                }
            }

            this -> setDimensions(BiVector(maxX, currentY));
            this -> setUsedSpace(BiVector(maxX, currentY));
        }

        this -> layoutUpdated = false;
    }

    void fillLayout() {
        if(this -> parent == nullptr) {
            this -> setDimensions(BiVector(WORLD_WIDTH, WORLD_HEIGHT));
        }
        if(this -> expandType == Fill && this -> parent != nullptr) {
            if(ViewContainer* casted = dynamic_cast<ViewContainer*>(this -> parent)) {
                BiVector parentDimensions = casted->getDimensions();
                BiVector parentUsedSpace = casted->getUsedSpace();

                double remainX = parentDimensions.x - parentUsedSpace.x;
                double remainY = parentDimensions.y - parentUsedSpace.y;

                if(casted -> flexType == Column) {
                    if(this -> growType == Grow) {
                        this->setDimensions(BiVector(
                                parentDimensions.x - this->getMargin().source.x - this->getMargin().destination.x,
                                this->getDimensions().y + (remainY / ((double) casted->nbFillChildren))));
                    } else if(this -> growType == Contain) {
                        this->setDimensions(BiVector(
                                this -> getDimensions().x,
                                this->getDimensions().y + (remainY / ((double) casted->nbFillChildren))));
                    }
                } else if(casted -> flexType == Row) {
                    if(this -> growType == Grow) {
                        this->setDimensions(
                                BiVector((remainX / ((double) casted->nbFillChildren)) + this->getDimensions().x,
                                         parentDimensions.y - this->getMargin().source.y - this->getMargin().destination.y));
                    } else if(this -> growType == Contain) {
                        this->setDimensions(
                                BiVector((remainX / ((double) casted->nbFillChildren)) + this->getDimensions().x,
                                         this->getDimensions().y));
                    }
                }
            }
        } else if(this -> parent != nullptr) {
            if(ViewContainer* casted = dynamic_cast<ViewContainer*>(this -> parent)) {
                BiVector parentDimensions = casted->getDimensions();

                if(casted -> flexType == Column) {
                    if(this -> growType == Grow) {
                        this->setDimensions(BiVector(
                                parentDimensions.x - this->getMargin().source.x - this->getMargin().destination.x,
                                this->getDimensions().y));
                    }
                } else if(casted -> flexType == Row) {
                    if(this -> growType == Grow) {
                        this->setDimensions(
                                BiVector(this->getDimensions().x,
                                         parentDimensions.y - this->getMargin().source.y - this->getMargin().destination.y));
                    }
                }
            }
        }

        for (auto &it : this -> children) {
            it->fillLayout();
        }
    }

    virtual void rearrangeFilledLayout() {
        double previousX = 0.0f;
        double previousY = 0.0f;
        if(this -> flexType == FlexType::Row) {
            double currentX = 0.0f;
            for (auto &it : this -> children) {
                previousX = (it->getMargin()).source.x;
                previousY = (it->getMargin()).source.y;
                it -> setPosition(BiVector(previousX + currentX, previousY));

                currentX += previousX + (it -> getDimensions()).x + (it->getMargin()).destination.x;

                it -> rearrangeFilledLayout();
            }
        } else if(this -> flexType == FlexType::Column) {
            double currentY = 0.0f;
            for (auto &it : this -> children) {
                previousX = (it->getMargin()).source.x;
                previousY = (it->getMargin()).source.y;
                it -> setPosition(BiVector(previousX, previousY + currentY));

                currentY += previousY + (it -> getDimensions()).y + (it->getMargin()).destination.y;

                it -> rearrangeFilledLayout();
            }
        }
    }

    void copy(const ViewContainer &that) {
        UIVisibleObject::copy(that);
        this -> layoutUpdated = that.layoutUpdated;
        this -> flexType = that.flexType;
        this -> expandType = that.expandType;
        this -> growType = that.growType;
        this -> usedSpace = that.usedSpace;
        this -> nbFillChildren = that.nbFillChildren;
    }

    void addChild(AliveObject *child) {
        AliveObject::addChild(child);
        this -> layoutUpdated = true;
    }
};

#endif // ENGINE_VIEWCONTAINER_H
