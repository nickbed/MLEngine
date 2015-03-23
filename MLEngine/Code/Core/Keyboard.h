#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include <algorithm>
#include <string>

#include "../Assert/Assert.h"
namespace mauveinput{

	class Keyboard {
	public:
		Keyboard();

		~Keyboard();

		static void keyCallback(GLFWwindow*, int, int, int, int);

		bool		Initialize(GLFWwindow*);
		bool		Destroy();


		static bool getAscii(std::string);
		static bool get(unsigned int);

	private:

		GLFWwindow*	mRenderWindow;
		static bool Keys[400];
	};
}
#endif
