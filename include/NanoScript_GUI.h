/** \file NanoScript_GUI.h
* \brief NanoScript functions for creating graphical user interface (GUI) items, 
* such as message boxes, dialogs, etc.
*
* Functions with boolean returns values, return true for success,
* unless otherwise noted.
*
* All dialog boxes are modal. This locks out other interaction with the GUI 
* until removed by the user clicking on a button on the dialog.
*
* The customizable dialog text uses printf stlye formating. 
*
* Copyright Bruker 1988-2011. All rights reserved.
*/

// Comments in this file follow Doxygen syntax, 
// for use external in documentation. When reading this file directly,
// you can ignore commented words that start with "\", such as "\brief"

#ifndef __NANOSCRIPT_GUI_H__
#define __NANOSCRIPT_GUI_H__

#ifdef _MSC_VER_
#pragma once
#endif

#ifndef NS_API
#define NS_API __declspec(dllexport)
#endif

/** \brief Pointer to function that takes void and returns void.
*
* Used for a callback. For example, the AddButtonControl function
* provides a way to associate a button with your own function
* that will get called when the button is pushed.
*/
typedef void (*PFV)();

/** \brief Handle to a displayed dialog box
*
* This is simply a pointer to \c void to keep
* track of the displayed dialog box.  
*/
typedef void* DialogBoxHandle;

/** \brief Create a modal dialog box
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param message Pointer to the actual message that
* should be displayed in the dialog box.
*
* \return A handle to the displayed dialog box. The memory this handle refers to is
* deleted when the call to RunDialog returns, and cannot be reused
*/


NS_API DialogBoxHandle ModalDialog(char *title, char* message = 0);

/** \brief Add an integer input control to a modal dialog box.
*
* \param dlg The handle to the dialog box the integer
* input control should be added to.
* \param label Pointer to the text that should be displayed
* on the left side of the integer input control.
* \param value A reference to the \c integer value that should
* be connected to the input control.
* \param minValue The minimum value that can be entered
* in the integer input control.
* \param maxValue The maximum value that can be entered
* in the integer input control.
*/
NS_API void AddIntControl(DialogBoxHandle dlg, char *label, int &value, int minValue, int maxValue);

/** \brief Add an float input control to a modal dialog box.
*
* \param dlg The handle to the dialog box the float
* input control should be added to.
* \param label Pointer to the text that should be displayed
* on the left side of the integer input control.
* \param value A reference to the \c float value that should
* be connected to the input control.
* \param minValue The minimum value that can be entered
* in the float input control.
* \param maxValue The maximum value that can be entered
* in the float input control.
* \param resolution The minimum resolution of the float
* input control. This also represents the step size
* for changes done via mouse input.
*/
NS_API void AddFloatControl(DialogBoxHandle dlg, char *label, float &value, float minValue, float maxValue, float resolution = 0.01f);

/** \brief Add an string input control to a modal dialog box.
*
* \param dlg The handle to the dialog box the text
* input control should be added to.
* \param label Pointer to the text that should be displayed
* on the left side of the integer input control.
* \param string A pointer to the string value that should
* be connected to the input control.
* \param stringLength The maximum length of the string
* input. The value *must* not be larger then the actual
* available space in the selected storage container.
*
* \todo Using 'string' as an argument name seems rather
* odd as this is considered a reserved identifier.
*/
NS_API void AddStringEntry(DialogBoxHandle dlg, char *label, char* string, int stringLength);

/** \brief Add a button to a modal dialog box.
*
* A specific action can be associated with the button
* by assigning a pointer to a function of the type
* 'void f(void)' to it.
*
* \param dlg The handle to the dialog box the button
* input control should be added to.
* \param caption Pointer to the text that should be
* displayed on the button.
* \param action Pointer to a function of the type
* 'void f(void)' that should be associated with the
* button.
*
* \note The button has a fixed size in the current
* implementation which has to be considered when
* chosing a caption for it to avoid cut-offs.
* \note The action associated with the button is
* executed 'immediately. To pass arguments to the
* function they have to be defined in a scope
* that can be freely read/modified from the function
* that is called. The same applies to any potential
* 'return' value of the associated function.
*/
NS_API void AddButtonControl(DialogBoxHandle dlg, char *caption, PFV action);

/** \brief Run (display) the modal dialog box. 
*
* \param dlg The handle to the dialog box that should
* be displayed, and is deleted when RunDialog returns, 
* and cannot then be reused.
*
* \return The result of the dialog box. This is
* the value that is returned when the dialog box
* is closed. The actual value depends on the way
* the dialog was closed.
*
* \todo Verify what return codes are actually given
* depending on the various ways to close it.
*/
NS_API int RunDialog(DialogBoxHandle dlg);


// Error-Message Functions

/** \brief Send a message to 'Error' message dialog box of Nanoscope 
*
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content or
* type of the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API void SayError(const char *fmt, ...);

/** \brief Send a message to the 'Warning' message dialog box if Nanoscope 
*
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API void SayWarning(const char *fmt, ...);

/** \brief Write message directly to Nanoscope log
*
* Writes the message to the Nanoscope internal
* application log instead of to the GUI.
*
* \param severity The severity or assigned importance
* of the message. Valid values are: <b>0 := simple
* message</b>, <b>1 := warning</b> and <b>2 := error</b>
* \param msg Message to write to the log file.
* All messages will apear under the default log
* node.
*
* \note The size of the message is checked and limited
* to 1024 characters to prevent buffer overruns.
*/
NS_API void WriteMsg2Log(unsigned int severity, const char* msg);


// Simple Modal Dialogs

/** \brief Display a simple dialog box with a "OK" and "Cancel" button
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \return \c TRUE if the user closes the dialog box
* by clicking the "OK" button. \c FALSE if the user
* clicks the "Cancel" button to close the dialog box.
*
* \note The <b>"OK"</b> button is selected by
* <b>default</b>.
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API bool AskOkCancel(const char *title, const char *fmt, ...);

/** \brief Display a simple dialog box with a "OK" and "Cancel" button
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \return \c TRUE if the user closes the dialog box
* by clicking the "OK" button. \c FALSE if the user
* clicks the "Cancel" button to close the dialog box.
*
* \note The <b>"Cancel"</b> button is selected by
* <b>default</b>.
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API bool AskCancelOk(const char *title, const char *fmt, ...);

/** \brief Display a simple dialog box with a "Yes" and "No" button
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \return \c TRUE if the user closes the dialog box
* by clicking the "Yes" button. \c FALSE if the user
* clicks the "No" button to close the dialog box.
*
* \note The <b>"Yes"</b> button is selected by
* <b>default</b>.
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API bool AskYesNo(const char *title, const char *fmt, ...);

/** \brief Display a simple dialog box with a "Yes" and "No" button
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \return \c TRUE if the user closes the dialog box
* by clicking the "Yes" button. \c FALSE if the user
* clicks the "No" button to close the dialog box.
*
* \note The <b>"No"</b> button is selected by
* <b>default</b>.
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API bool AskNoYes(const char *title, const char *fmt, ...);


/** \brief Display a simple dialog box with a "OK" button
*
* \param title Pointer to the title that should
* be displayed on the dialog box.
* \param fmt Pointer to the 'initial' text message.
* See notes for further information.
*
* \return \c TRUE if the user closes the dialog box
* by clicking the "OK" button otherwise \c FALSE.
*
* \note The 'initial' message can contain the
* same input/string format codes usually used
* by \c printf() etc. This allows the user to
* pass additional information in the form of
* variables.
* \warning This is a variable argument call. Neither
* the correct number nor the actual content of
* the passed arguments are checked on runtime.
* The user has to take care of the correct number
* and type of the passed arguments.
* \warning If a input/string format code is
* used in the 'initial' message but no additional
* argument is actually passed the behavior is
* undetermined and can potentially crash the
* application. The user is responsible for
* the correct use of the feature.
*/
NS_API void AskOk(const char *title, const char *fmt, ...);



#endif  // __NANOSCRIPT_GUI_H__
