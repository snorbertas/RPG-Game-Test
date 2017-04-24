#pragma once
#define MAX_INTERFACES 100
#define MAX_BUTTONS 400


// Button structure
struct Button {
	bool visible = true;					// If true, button is rendered and active
	int sprite_id;							// Sprite ID for button (sprite_id + 1 is highlighted when mouse is colliding)
	int w, h;								// Width and height for button (hitbox)
	int interface_id;						// Interface ID the button belongs to
	int x = -1;								// x position for button relative to interface
	int y = -1;								// y position for button relative to interface
	float opacity = 1;						// Button transperency (<1 is transperent)

	// Visibility
	void Show() { visible = true; }
	void Hide() { visible = false; }
};

// Interface structure
struct Interface {
	int sprite_id;							// Sprite ID for interface
	int x, y;								// x, y coordinates
	bool visible;							// If true, interface is visible and active
	float opacity = 1.0;					// Transperency

	// Visibility
	void Show() { visible = true; }
	void Hide() { visible = false; }
};

/* ============================ LoadInterfaces ============================
 *		Function to load and initilizate all interfaces.
 *		Also initializes buttons for interfaces.
 *
 *		Called from main function when desired.
 */
	void LoadInterfaces(Game* g);		// Game pointer


/* ============================= NewInterface =============================
 *		Creates and overwites the (pointed) interface with specified settings.
 *
 *		Called from LoadInterfaces function.
 */
	void NewInterface(Interface* in,	// Interface pointer
		int sprite,						// Sprite ID for interface
		int x, int y);					// x, y coordinates for interface


/* =============================== NewButton ==============================
 *		Creates and overwrites the (pointed) button with specified settings.
 *
 *		Called from LoadInterfaces function.
 */
	void NewButton(Button* b,			// Button pointer
		int sprite,						// Sprite ID for button
		int x, int y,					// x, y coordinates for button (relative to interface)
		int w, int h,					// width, height for button (hitbox)
		int iid,						/* Interface ID the button belongs to
										   (only active if the interface is visible) */
		float opacity=1);				// Transperency for button (0 = invisible, but still active)


/* =========================== RenderInterfaces ===========================
 *		Renders all interfaces.
 *
 *		Called once per tick from the main function.
 */
	void RenderInterfaces(Game* g,
		SpriteStruct* sprites,				// Pointer to sprites
		ALLEGRO_FONT** font);				// Pointer to array of fonts


/* ============================ GetWindowMode =============================
 *		Returns the name of window mode by the Allegro flag.
 *
 *		Called when rendering settings (INTERFACE_OPTIONS).
 */
	const char* GetWindowMode(int wid);		// Window mode ID (Allegro flag)


/* =========================== InterfaceIsOnTop ===========================
 *		Returns true if the specified interface ID is on top of all interfaces.
 *
 *		Called when handling button clicks so only the top interface receives
 *		input.
 */
	bool InterfaceIsOnTop(Game* g,
		int id);							// Interface ID to check


/* =========================== HideAllInterfaces ==========================
 *		Hides all interfaces (except the specified ID).
 *
 *		Called when desired.
 */
	void HideAllInterfaces(Game* g,
		int except);						// Interface ID to except


/* =========================== HandleInterfaces ===========================
 *		Handles interface mechanics, animations, fades, etc.
 *
 *		Called once per game tick from the main function.
 */
	void HandleInterfaces(Game* g);


/* ========================== HandleChatInterface =========================
 *		Handles chat interface.
 *
 *		Called from HandlInterfaces function.
 */
	void HandleChatInterface(Game* g);


/* ========================= HandleLoginInterface =========================
 *		Handles login interface.
 *
 *		Called from HandlInterfaces function.
 */
	void HandleLoginInterface(Game* g);


/* ============================== GetKeyName ==============================
 *		Returns the name of the key id.
 *		Also initializes buttons for interfaces.
 *
 *		Called when rendering key names for abilities, etc.
 */
	const char* GetKeyName(int key_id);		// Key ID to get name from




/* ============================= SubmitMessage ============================
 *		Submits a message.
 *		Local command or multiplayer chat.
 *
 *		Called when a chat message is requested to submit.
 */
	void SubmitMessage(Game* g,
		const char* msg);					// Message to submit


/* ============================= HandleCommand ============================
 *		Handles commands.
 *		CURRENTLY VERY MESSY!!!
 *
 *		Called when a command is desired to be executed.
 */
	void HandleCommand(Game* g,
		const char* msg);					// Message command to handle


// =============================== Bar types ==============================
	enum BarType { EMPTY, RED, YELLOW, BLUE };
/* ================================ DrawBar ===============================
 *		Draws a progress bar with specified settings.
 *
 *		Called when drawing interfaces.
 */
	void DrawBar(Game* g, 
		SpriteStruct* sprites,				// Sprite struct
		BarType type,						// Bar type
		int x, int y,						// x, y start
		int length);						// Lenght of bar (total)


// ========================== Interface box types =========================
	enum InterfaceBoxType { BROWN, BROWN_GLOW };
/* =========================== DrawInterfaceBox ===========================
 *		Draws a progress bar with specified settings.
 *
 *		Called when drawing interfaces.
 */
	void DrawInterfaceBox(Game* g,
		SpriteStruct* sprites,				// Sprite struct
		InterfaceBoxType type,				// Interface box type
		int x, int y,						// x, y start
		int w, int h,						// Width and height
		float opacity = 1.0);				// Opacity