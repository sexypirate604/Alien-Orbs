# Alien-Orbs

An orb shooter for Linux, Windows, and OSX

What:
	I created a 3D space shooter where a camera tracks a space craft from
	a second person view. The direction of the space craft is to always move
	forward, but the ship can move up, left, right, down, up-left,
	up-right, down-left, and down-right. The ship speed can toggle between
	normal speed and turbo speed.
	The player clicks on orbs to destroy them. Larger orbs spawn two smaller
	orbs when they are destroyed.
	The object of the game is to destroy as many orbs as possible before the
	player's ship comes in contact with an alien orb. When the ship comes in
	contact with an alien orb, the game is over.

How:
	I used a GLUT function to render spheres, and a single light source to
	illuminate them.
	The space craft is modeled similarly to my project two space craft which
	was a tetrahedron.
	Picking is used to detect which orb is to be destroyed. If a click happens
	to select two or more alien orbs, then only the closest orb to the camera
	is destroyed.
	The star field in the background is a textured plane that is positioned
	at the view's back pane. The texturing technique I used was to repeat the
	texture over the back pane. To load the texture and initialize it I used
	two tutorials (see sources and comments in main.cpp), and not much of the
	code is my own.
	Orbs spawn at random intervals, and always spawn at a distance near the
	camera's back pane.
	Orbs are removed from the game and their memory deallocated when they
	are destroyed (by left-clicking), or when they are detected to have a world
	position behind the camera.

How To:
	There are three sub-sections to this, how-to play section:

	1) Game start:
	When the game first starts it is in a restart state. The only input commands
	accepted are from the keyboard.
	Keyboard:
		' ' (spacebar) : starts the game, the space craft should begin moving
			forward.
		'ESC' : quit game.

	2) In Game Controls:
	Keyboard:
		' ' : Toggle game paused. Notice CPU cycles not being wasted and display
			command will stop rendering.
		'a' : move left.
		'd' : move right.
		'w' : move up.
		's' : move down.
		'ESC' : quit game.
		'r' : for testing, simulates ship collision ending the game.
	Mouse:
		'left-click' : select orb to destroy.
		'right-click' : on keypress down, turbo ship speed. on keypress release
			up,	normal ship speed.

	3) Game End:
		This part of the game is buggy, and if I had more time I would like to
		have enabled restarting the game by hitting spacebar.
	KeyBoard:
		'ESC' : quit game.

	Note:
		Cannot play the game again after it has ended, must restart to play
		again.
		
Feel free to modify and improve this code however you like.

Cheers,

Michael


Sources:

Capturing mouse input using GLUT:
URL: http://www.lighthouse3d.com/tutorials/glut-tutorial/the-mouse/
Date: 31 Jan 2001

Calculating the frames rendered per second:
URL: http://www.lighthouse3d.com/tutorials/glut-tutorial/frames-per-second/

Picking:
URL: http://www.lighthouse3d.com/opengl/picking/

Capturing keypresses and releases using GLUT:
URL: http://www.swiftless.com/tutorials/opengl/keyboard.html

Lighting:
URL: http://www.sjbaker.org/steve/omniv/opengl_lighting.html

Textures:
Note:
	I pretty much used the template code for texturing, but mapped myself
	when drawing the object (star field back pane).
URL: http://lodev.org/lodepng/
URL: http://lodev.org/lodepng/example_opengl.cpp
