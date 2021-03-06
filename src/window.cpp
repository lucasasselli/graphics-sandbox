#include "window.h"

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

Delta<float> timeDelta;
Delta<float> xposDelta;
Delta<float> yposDelta;

float yaw = -90.0;
float pitch = 0.0;

GLFWwindow* window;

static void windowMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    xposDelta.sample(static_cast<float>(xpos));
    yposDelta.sample(static_cast<float>(ypos));

    yaw += xposDelta.get()*INPUT_MOUSE_SENSITIVITY;
    pitch -= yposDelta.get()*INPUT_MOUSE_SENSITIVITY;

    pitch = glm::clamp(pitch, -89.0f, +89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void windowErrorCallback(int error, const char* description)
{
    printf("Window error: %s\n", description);
    exit(EXIT_FAILURE);
}

static void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	if (!width || !height)
		return;

	glViewport(0, 0, width, height);
}

void windowInit(int width, int height)
{
    // glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
    glfwSetErrorCallback(windowErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Callbacks
    glfwSetCursorPosCallback(window, windowMouseCallback);
    // glfwSetKeyCallback(window, inputKeyCallback);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);
    gladLoadGL();
    glfwSwapInterval(1);
}

void windowExit()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void inputParse()
{
    timeDelta.sample(glfwGetTime());

    float cameraSpeed = INPUT_CAMERA_SPEED * timeDelta.get();

    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}