#include "mouse.h"

namespace Blib {
Mouse mouse;
}


void Blib::Mouse::Update(GLFWwindow *window) {
    auto rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (rightButtonState == GLFW_PRESS && rightButtonPrssed == false) {
        rightButtonPrssed = true;
        glfwGetCursorPos(window, &mouseDownX, &mouseDownY);
    } 
    if (rightButtonPrssed) {
        double mouseCurrentX, mouseCurrentY;
        glfwGetCursorPos(window, &mouseCurrentX, &mouseCurrentY);
        glfwSetCursorPos(window, mouseDownX, mouseDownY);

        deltaRight = {mouseCurrentX - mouseDownX, mouseCurrentY - mouseDownY};
    }
    if (rightButtonState == GLFW_RELEASE && rightButtonPrssed == true) {
        rightButtonPrssed = false;
        deltaRight = {0, 0};
    }
}

