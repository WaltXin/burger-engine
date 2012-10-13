/*************************************
*
*		BurgerEngine Project
*		
*		Created :	27/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <vector>

#include "BurgerEngine/External/Loki/Include/loki/Functor.h"
#include "BurgerEngine/External/XController/XController.h"
//#include "BurgerEngine/External/WiiYourself/wiimote.h"

///	\name EventManager.h
///	\brief	Register and dispatch eventCallbacks
///	So far works for input but could work with other stuff
///	Here an exemple how to access control over keyboard
///	You create a function bool MyClass::Foo(unsigned char) in you class
///	Must return Fasle is want other callback to be not be called. (Pop Up menu)
///	Then you call the following
///	Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardUpKey(CallBackKeyBoard<this,MyClass::Foo>);
///	To unregister
///	Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardUpKey(CallBackKeyBoard<this,MyClass::Foo>);

class XController;

class EventManager
{
public:

	enum PAD_BUTTON
	{
		 PAD_BUTTON_1 = 0	//Xbox A
		,PAD_BUTTON_2		//Xbox B
		,PAD_BUTTON_3		//Xbox X
		,PAD_BUTTON_4		//Xbox Y

		,PAD_BUTTON_L		//Xbox LB
		,PAD_BUTTON_R		//Xbox RB

		,PAD_BUTTON_L_SITCK	//Xbox left stick button
		,PAD_BUTTON_R_STICK	//Xbox right stick button

		,PAD_BUTTON_UP		//Xbox DPad Up
		,PAD_BUTTON_DOWN	//Xbox DPad Down
		,PAD_BUTTON_LEFT	//Xbox DPad Left
		,PAD_BUTTON_RIGHT	//Xbox DPad Right

		,PAD_BUTTON_START	//Xbox Start
		,PAD_BUTTON_SELECT	//Xbox Back
		,PAD_BUTTON_MAX		//MAX
	};

	//Call back typedef
	typedef Loki::Functor<bool,LOKI_TYPELIST_1(unsigned char)> CallbackKeyboard;
	typedef Loki::Functor<bool,LOKI_TYPELIST_2(unsigned int, unsigned int)> CallbackMouseMotion;
	typedef Loki::Functor<bool,LOKI_TYPELIST_3(
		unsigned int, //Button
		unsigned int, // Coordinates
		unsigned int)> CallbackMouseButton;
	typedef Loki::Functor<bool,LOKI_TYPELIST_2(unsigned int, unsigned int)> CallbackResize;

	typedef Loki::Functor<bool,LOKI_TYPELIST_1(unsigned int)> CallbackXBoxButtonPressed;
	typedef Loki::Functor<bool,LOKI_TYPELIST_3(
		unsigned int, //0 (left stick), 1 (right stick)
		float, // x value
		float //y value
		)> CallbackXBoxJoystick;

	typedef Loki::Functor<bool,LOKI_TYPELIST_2(PAD_BUTTON, bool)> CallbackPadButtonPressed;

public:

	/// \brief	Initialize method (set what input manager to use....)
	void Init();

	/// \brief	Clear all callback list
	void Clear();

	/// \brief	Has to be called each frame
	///			Will clear the event queue
	void ProcessEventList();

	/// \brief Register callback when a key is release
	void RegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback);
	void UnRegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback);

	/// \brief Register callback when key is pressed
	void RegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback);
	void UnRegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback);

	/// \brief Register callbakc when mouse is clicked
	void RegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback);
	void UnRegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback);
	
	/// \brief Register when mouse is moved without button pressed
	void RegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback);
	void UnRegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback);

	/// \brief Register when mouse is moved with button pressed
	void RegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback);
	void UnRegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback);

	/// \brief Register when screen is resized
	void RegisterCallbackResize(CallbackResize& a_rCallback);
	void UnRegisterCallbackResize(CallbackResize& a_rCallback);

	/// \brief Register for joystick values
	void RegisterCallbackJoystick(CallbackXBoxJoystick& a_rCallback);
	void UnRegisterCallbackJoystick(CallbackXBoxJoystick& a_rCallback);

	/// \brief Register for joystick values
	void RegisterCallbackPadButton(CallbackPadButtonPressed& a_rCallback);
	void UnRegisterCallbackPadButton(CallbackPadButtonPressed& a_rCallback);

	///--------- Dispatch event ----------------
	/// \brief	Send the key event to every register Callback
	void DispatchKeyboardUpKeyEvent(unsigned char a_cKey) const;

	/// \brief	Send the key event to every register Callback
	void DispatchKeyboardDownKeyEvent(unsigned char a_cKey) const;

	/// \brief	Send the click event to every register Callback
	void DispatchMouseDownClick(int a_iButton, int a_iXCoordinates, int a_iYCoordinates) const;

	/// \brief	Send the release event to every register Callback
	void DispatchMouseUpClick(int a_iButton, int a_iXCoordinates, int a_iYCoordinates) const;

	/// \brief	Send the mouse motion event to every register Callback
	void DispatchMousePassiveMotion(unsigned int a_iXCoordinates, unsigned int a_iYCoordinates) const;

	/// \brief	Send the mouse motion to every register Callback
	void DispatchMouseActiveMotion(unsigned int a_iXCoordinates, unsigned int a_iYCoordinates) const;
	 
	/// \brief	Send the Resize event to every register callback
	void DispatchResize(unsigned int a_uWidth, unsigned int a_uHeight) const;

	/// \brief	Send the value of a joystick to every register Callback
	void DispatchJoystickValue(unsigned int a_iStick, float a_iXValue, float a_iYValue) const;

	/// \brief	Send the button pressed/released event to every register Callback
	void DispatchPadButtonPressed(PAD_BUTTON iButton, bool bPressed) const;

private:

	void ProcessXboxControllerEvents();

	bool m_pButtonPressed[PAD_BUTTON_MAX];

	//Vector containing the callbacks
	std::vector<CallbackKeyboard> m_vKeyboardUpKeyCallbacks;
	std::vector<CallbackKeyboard> m_vKeyboardDownKeyCallbacks;
	std::vector<CallbackResize> m_vResizeCallbacks;
	std::vector<CallbackMouseMotion> m_vMousePassiveMotionCallbacks;
	std::vector<CallbackMouseMotion> m_vMouseActiveMotionCallbacks;
	std::vector<CallbackMouseButton> m_vMouseClickDownCallbacks;
	std::vector<CallbackMouseButton> m_vMouseClickUpCallbacks;
	std::vector<CallbackXBoxJoystick> m_vXBoxJoystickCallbacks;
	std::vector<CallbackPadButtonPressed> m_vPadButtonCallbacks;

	XController* m_oXboxController; 
	
};

#endif //__EVENTMANAGER_H__