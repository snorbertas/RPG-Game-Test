#pragma once
#include "DEEvents.h"

#define CAMERA_MODE_LOCKED 1
#define CAMERA_MODE_CUTSCENE 4

class Camera
{
public:
	Camera();
	~Camera();
	int x;						// Camera x
	int y;						// Camera y
	int mode = CAMERA_MODE_LOCKED;	// Camera mode
	int move_to_x;				// x Destination for camera to slide towards
	int move_to_y;				// y Destination for camera to slide towards
	int x_vel;					// Camera's x velocity for sliding
	int y_vel;					// Camera's y velocity for sliding
};


/* ============================= HandleCamera =============================
 *		Handles the camera movement.
 *
 *		Called from HandleGame function.
 */
	void HandleCamera(Game* g);

	
/* ============================== MoveCamera ==============================
 *		Moves the camera in the CAMERA_MODE_CUTSCENE mode.
 *
 *		Used for cinematic effect purposes.
 */
	void MoveCamera(Game* g,
		int to_x, int to_y,					// x and y destination for camera
		int x_vel = 1,						// x velocity for camera movement
		int y_vel = 1);						// y veolocity for camera movement
											// x veolocity for camera movement
	
/* =============================== SetCamera ==============================
 *		Sets the camera position to these x/y coordinates and mode.
 *
 *		Used when desired.
 */
	void SetCamera(Game* g,
		int x, int y,						// x and y coordinates for camera
		int mode);							// Camera mode

int RegisterCameraEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
