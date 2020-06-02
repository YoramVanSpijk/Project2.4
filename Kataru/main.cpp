#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tigl.h"
#include "FpsCam.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

using namespace cv;
using namespace std;

void init();
void update();
void draw();

const string CONTROLS_TAG = "Controls";
const string MASK_TAG = "Mask";

GLFWwindow* window;
GLuint camTexId = 99;

Mat frame, hsvImage, erosionDst, dilationDst, element,
croppedImageL, croppedImageR, lHSV, rHSV, mask;
bool skinToneDetected = false;
int hLowThreshold, hHighThreshold, sLowThreshold, sHighThreshold, vLowThreshold, vHighThreshold, minAreaBlob, maxAreaBlob, offsetHighThreshold, offsetLowThreshold;
SimpleBlobDetector::Params params;
int main(void)
{	
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1280, 720, "Hand detection", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

    return 0;
}

FpsCam* camera;
VideoCapture cap;

void init()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    camera = new FpsCam(window);


    // Camera
    VideoCapture c(0);
    cap = c;
    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
    }

    glGenTextures(1, &camTexId);
    glBindTexture(GL_TEXTURE_2D, camTexId);

    element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    offsetLowThreshold = offsetHighThreshold = 40;
    minAreaBlob = 1000;
    maxAreaBlob = 50000;
}

void update()
{
    camera->update(window);
}

void draw()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glm::mat4 worldModel = glm::mat4(1.0f);
    worldModel = glm::scale(worldModel, glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 projection = glm::perspective(glm::radians(55.0f), width / (float)height, 0.1f, 100.0f);
    tigl::shader->setProjectionMatrix(projection);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tigl::shader->setModelMatrix(worldModel);
    tigl::shader->setViewMatrix(camera->getMatrix());

    tigl::shader->enableColor(false);
    tigl::shader->enableTexture(true);

    if (cap.read(frame)) {
        flip(frame, frame, 3);

		// Rectangles die worden getekend bij het vinden van de kleur
		Rect rLeft = Rect((frame.size().width / 2) - 10, (frame.size().height / 2) - 20, 20, 40);
		Rect rRight = Rect((frame.size().width / 2) + 10, (frame.size().height / 2) - 20, 20, 40);

		// Uitgesnede delen opslaan in aparte Mat objecten
		croppedImageL = frame(rLeft);
		croppedImageR = frame(rRight);

		// Teken de rectangles op de main frame
		rectangle(frame, rLeft, Scalar(0, 255, 0), 1.5, 8, 0);
		rectangle(frame, rRight, Scalar(0, 255, 0), 1.5, 8, 0);

		if (skinToneDetected) {
			cvtColor(frame, hsvImage, CV_BGR2HSV);

			inRange(hsvImage,
				Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
				Scalar(hHighThreshold, sHighThreshold, vHighThreshold), mask);

			blur(mask, mask, Size(3, 3));

			morphologyEx(mask, mask, MORPH_OPEN, element);
			dilate(mask, mask, Mat(), Point(-1, -1), 3);

			bitwise_not(mask, mask);

			imshow(MASK_TAG, mask * 255);

			SimpleBlobDetector::Params params;
			params.minThreshold = 10;
			params.maxThreshold = 200;
			params.filterByArea = true;
			params.minArea = minAreaBlob;
			params.maxArea = maxAreaBlob;

			vector<KeyPoint> keypoints;
			Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
			detector->detect(mask, keypoints); //hiero
			//drawKeypoints(frame, keypoints, frame, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			//for (int i = 0; i < keypoints.size(); i++) {
			//	Point2f p = keypoints[i].pt;
			//	cout << "X: " << p.x << ", Y: " << p.y << "\n";
			//	circle(frame, p, 4, Scalar(0, 0, 255), -1, 8, 0);
			//}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cout << "S KEY PRESSED > Color detection running..." << endl;

			// Converteren naar HSV om Max Hue te verkijgen
			cvtColor(croppedImageL, lHSV, CV_BGR2HSV);
			cvtColor(croppedImageR, rHSV, CV_BGR2HSV);

			// Calculeer het gemiddelde over de linker en rechter array
			Scalar hsvMeansSample1 = mean(lHSV);
			Scalar hsvMeansSample2 = mean(rHSV);

			// Pak het hoogste getal en reken met die waarde de min waarde uit
			hHighThreshold = max(hsvMeansSample1[0], hsvMeansSample2[0]) + (offsetHighThreshold / 2);
			hLowThreshold = min(hsvMeansSample1[0], hsvMeansSample2[0]) - (offsetLowThreshold / 2);

			sHighThreshold = max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;
			sLowThreshold = min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;

			vHighThreshold = max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;
			vLowThreshold = min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;

			// Creer window voor controls
			namedWindow("Controls", CV_WINDOW_NORMAL);

			// Creeren van alle trackbars
			createTrackbar("hMin", CONTROLS_TAG, &hLowThreshold, 179);
			createTrackbar("hMax", CONTROLS_TAG, &hHighThreshold, 179);
			createTrackbar("sMin", CONTROLS_TAG, &sLowThreshold, 255);
			createTrackbar("sMax", CONTROLS_TAG, &sHighThreshold, 255);
			createTrackbar("vMin", CONTROLS_TAG, &vLowThreshold, 255);
			createTrackbar("vMax", CONTROLS_TAG, &vHighThreshold, 255);
			createTrackbar("bMin", CONTROLS_TAG, &minAreaBlob, 30000);
			createTrackbar("bMax", CONTROLS_TAG, &maxAreaBlob, 50000);

			skinToneDetected = true;
		}

        cvtColor(frame, frame, CV_BGR2RGB);
        glBindTexture(GL_TEXTURE_2D, camTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glm::mat4 camModel = glm::mat4(1.0f) * worldModel;
    camModel = glm::translate(camModel, glm::vec3(-1.0f, -0.5f, -1.0f));
    tigl::shader->setModelMatrix(camModel);

    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec2(1, -1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, -1)));
    tigl::end();
}
