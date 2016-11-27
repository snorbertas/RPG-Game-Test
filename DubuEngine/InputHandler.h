#pragma once

/* =============================== LeftClick ==============================
 *		Handles left mouse clicks.
 *
 *		Called from main function when ALLEGRO_EVENT_MOUSE_BUTTON_DOWN or
 *		ALLEGRO_EVENT_MOUSE_BUTTON_UP event is triggered and button == 1.
 */
	void LeftClick(Game* g,					// Game pointer
		bool release,						// Key released (true) or pressed (false)
		ALLEGRO_SAMPLE** sample_sfx);		// Pointer to array of sound effects


/* =============================== RightClick =============================
 *		Handles right mouse clicks.
 *
 *		Called from main function when ALLEGRO_EVENT_MOUSE_BUTTON_DOWN or 
 *		ALLEGRO_EVENT_MOUSE_BUTTON_UP event is triggered and button == 2.
 */
	void RightClick(Game* g,
		bool release,						
		ALLEGRO_SAMPLE** sample_sfx);


/* =========================== KeyboardFunction ===========================
 *		Handles keyboard input.
 *
 *		Called from main function when ALLEGRO_EVENT_KEY_DOWN or
 *		ALLEGRO_EVENT_KEY_UP event triggered.
 */
	void KeyboardFunction(Game* g,
		int kid,
		bool release,
		ALLEGRO_SAMPLE** sample_sfx,
		ALLEGRO_FONT** font = NULL);		// Pointer to array of fonts

/* ============================== ChooseEvent ==============================
 *		Sets the tool for editor to spawn a new event.
 *
 *		Called when desired to change the tool into a specific event type.
 *		(Same with ChooseBackground, ChooseForeground, ChoosePlatform,
 *		except the different tool types).
 */
	int ChooseEvent(Game* g,				// Game pointer
		int type);							// Event type

	int ChooseBackground(Game* g,
		int type);							// Background type

	int ChooseForeground(Game* g,
		int type);							// Foreground type

	int ChoosePlatform(Game* g,
		int type);							// Platform type

/* ========================== NextUnusedPlatform ==========================
 *		Returns the next avaible platform index for a specified platform type.
 *
 *		Called from Choose* functions.
 */
	int NextUnusedPlatform(Game* g,
		int tool);							// Tool type


/* =============================== BrowseFile =============================
 *		Handles file browsing for saving/loading map files via Editor.
 *
 *		Called as a thread when desired in Editor.
 */
	unsigned int __stdcall BrowseFile(void *data);	// *data should point to a Game object


/* ========================== GetCharacterFromKey =========================
 *		Returns a character acquired from a specific key.
 *
 *		Called when desired (typing, etc.).
 */
	const char* GetCharacterFromKey(int key_id,		// Key ID
		bool shift);								// Shift held down or not (capitalization)


/* ============================== ToClipboard =============================
 *		Copies a specified string into the clipboard. (Windows Only atm)
 *
 *		Used by the Editor to Copy platforms and other objects (CTRL+C).
 */
	void ToClipboard(const string &s);			// String to write to clipboard


/* ============================= FromClipboard ============================
 *		Returns the string stored in the clipboard. (Windows Only atm)
 *
 *		Used by the Editor to Paste platforms and other objects (CTRL+V).
 */
	string FromClipboard();


/* ========================== RememberEditorState =========================
 *		Saves/memorizes the editor state to allow undos to this state.
 *
 *		Called when the user makes a change in the map via Editor.
 */
	void RememberEditorState(Game* g);


/* =============================== UndoEditor =============================
 *		Reverses the editor state by one step.
 *
 *		Called by Editor when an undo is requested (CTRL+Z).
 */
	void UndoEditor(Game* g);

	
/* ================================ QuickSave =============================
 *		Quickly saves the current map.
 *
 *		Called from InputHandler (CTRL+C).
 */
	void QuickSave(Game* g);


/* ========================== RequestBindKeyInput =========================
 *		Requests the user to input a key to bind.
 *
 *		Called when a button for a specific keybind is clicked.
 */
	void RequestBindKeyInput(Game* g,
		int* target_bind);				// Pointer to targetted control/key to request a bind to


/* ================================ BindKey ===============================
 *		Binds a key.
 *
 *		Called when a requested keybind is inputted.
 */
	void BindKey(int* target,			// Pointer to targetted control/key to bind to
		int key_id);					// Key ID to bind to


/* =============================== KeyIsBound =============================
 *		Returns a pointer to an integer storing a keybind setting.
 *
 *		Called to check for duplicate key binds.
 */
	int* KeyIsBound(Game* g,
		int key_id);					// The key to check whether it's bound or not