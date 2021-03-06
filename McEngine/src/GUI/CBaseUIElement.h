//================ Copyright (c) 2013, PG, All rights reserved. =================//
//
// Purpose:		the base class for all UI Elements
//
// $NoKeywords: $buie
//===============================================================================//

#ifndef CBASEUIELEMENT_H
#define CBASEUIELEMENT_H

#include "cbase.h"
#include "Engine.h"
#include "KeyboardListener.h"

class CBaseUIElement : public KeyboardListener
{
public:
	CBaseUIElement(float xPos, float yPos, float xSize, float ySize, UString name);
	virtual ~CBaseUIElement() {;}

	// main
	virtual void draw(Graphics *g) = 0;
	virtual void update();

	// keyboard input
	virtual void onKeyUp(KeyboardEvent &e) {;}
	virtual void onKeyDown(KeyboardEvent &e) {;}
	virtual void onChar(KeyboardEvent &e) {;}

	// setters
	virtual void setPos(float xPos, float yPos) {if (m_vPos.x != xPos || m_vPos.y != yPos) {m_vPos.x = xPos; m_vPos.y = yPos; onMoved();}}
	virtual void setPosX(float xPos) {if (m_vPos.x != xPos) {m_vPos.x = xPos; onMoved();}}
	virtual void setPosY(float yPos) {if (m_vPos.y != yPos) {m_vPos.y = yPos; onMoved();}}
	virtual void setPos(Vector2 position) {if (m_vPos != position) {m_vPos = position; onMoved();}}

	virtual void setRelPos(float xPos, float yPos) {if (m_vmPos.x != xPos || m_vmPos.y != yPos) {m_vmPos.x = xPos; m_vmPos.y = yPos; onMoved();}}
	virtual void setRelPos(Vector2 position) {if (m_vmPos != position) {m_vmPos = position; onMoved();}}
	virtual void setRelPosX(float xPos) {if (m_vmPos.x != xPos) {m_vmPos.x = xPos; onMoved();}}
	virtual void setRelPosY(float yPos) {if (m_vmPos.y != yPos) {m_vmPos.y = yPos; onMoved();}}

	virtual void setSize(float xSize, float ySize) {if (m_vSize.x != xSize || m_vSize.y != ySize) {m_vSize.x = xSize; m_vSize.y = ySize; onResized();}}
	virtual void setSizeX(float xSize) {if (m_vSize.x != xSize) {m_vSize.x = xSize; onResized();}}
	virtual void setSizeY(float ySize) {if (m_vSize.y != ySize) {m_vSize.y = ySize; onResized();}}
	virtual void setSize(Vector2 size) {if (m_vSize != size) {m_vSize = size; onResized();}}

	virtual void setVisible(bool visible) {m_bVisible = visible;}
	virtual void setActive(bool active) {m_bActive = active;}
	virtual void setKeepActive(bool keepActive) {m_bKeepActive = keepActive;}
	virtual void setDrawManually(bool drawManually) {m_bDrawManually = drawManually;}
	virtual void setPositionManually(bool positionManually) {m_bPositionManually = positionManually;}
	virtual void setEnabled(bool enabled) {if (enabled != m_bEnabled) {m_bEnabled = enabled; if (m_bEnabled) {onEnabled();} else {onDisabled();}}}
	virtual void setBusy(bool busy) {m_bBusy = busy;}
	virtual void setName(UString name) {m_sName = name;}

	// getters
	inline const Vector2& getPos() const {return m_vPos;}
	inline const Vector2& getSize() const {return m_vSize;}
	inline UString getName() const {return m_sName;}
	inline const Vector2& getRelPos() const {return m_vmPos;}

	virtual bool isActive() {return m_bActive || isBusy();}
	virtual bool isVisible() {return m_bVisible;}
	virtual bool isEnabled() {return m_bEnabled;}
	virtual bool isBusy() {return m_bBusy && isVisible();}
	virtual bool isDrawnManually() {return m_bDrawManually;}
	virtual bool isPositionedManually() {return m_bPositionManually;}
	virtual bool isMouseInside() {return m_bMouseInside && isVisible();}

	// actions
	void stealFocus() {m_bMouseInsideCheck = true; m_bActive = false; onFocusStolen();}

protected:
	// events
	virtual void onResized() {;}
	virtual void onMoved() {;}

	virtual void onFocusStolen() {;}
	virtual void onEnabled() {;}
	virtual void onDisabled() {;}

	virtual void onMouseInside() {;}
	virtual void onMouseOutside() {;}
	virtual void onMouseDownInside() {;}
	virtual void onMouseDownOutside() {;}
	virtual void onMouseUpInside() {;}
	virtual void onMouseUpOutside() {;}

	// vars
	UString m_sName;

	// attributes
	bool m_bVisible;
	bool m_bActive;			// we are doing something, e.g. textbox is blinking and ready to receive input
	bool m_bBusy;			// we demand the focus to be kept on us, e.g. click-drag scrolling in a scrollview
	bool m_bEnabled;

	bool m_bKeepActive;		// once clicked, don't lose m_bActive, we have to manually release it (e.g. textbox)
	bool m_bDrawManually;
	bool m_bPositionManually;
	bool m_bMouseInside;

	// position and size
	Vector2 m_vPos;
	Vector2 m_vmPos;
	Vector2 m_vSize;

private:
	bool m_bMouseInsideCheck;
	bool m_bMouseUpCheck;
};

#endif
