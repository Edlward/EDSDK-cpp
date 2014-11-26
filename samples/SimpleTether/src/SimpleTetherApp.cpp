#include <boost/filesystem.hpp>
//#include "cinder/app/AppNative.h"
#include "Cinder-EDSDK.h"


using namespace EDSDK;
//using namespace ci;
//using namespace ci::app;


//void drawTextureInRect(gl::Texture texture, Rectf rect, bool fit = true) {
//    if (fit) {
//        Rectf textureRect = Rectf(Vec2i::zero(), texture.getSize());
//        Rectf r = textureRect.getCenteredFit(rect, true);
//        gl::draw(texture, r);
//    } else {
//        Area a =  Area(rect.getCenteredFit(texture.getBounds(), true));
//        gl::draw(texture, a, rect);
//    }
//}

class SimpleTetherApp {
public:
//    void prepareSettings(Settings* settings);
    void setup();
    void keyDown();
    void draw();

    void browserDidAddCamera(CameraRef camera);
    void browserDidRemoveCamera(CameraRef camera);
    void browserDidEnumerateCameras();

    void didRemoveCamera(CameraRef camera);
    void didAddFile(CameraRef camera, CameraFileRef file);

private:
    CameraRef mCamera;
//    gl::Texture mPhotoTexture;
};

//void SimpleTetherApp::prepareSettings(Settings* settings) {
//    settings->enableHighDensityDisplay();
//    settings->setWindowSize(640, 480);
//}

void SimpleTetherApp::setup()
{
    CameraBrowser::instance()->connectAddedHandler(&SimpleTetherApp::browserDidAddCamera, this);
    CameraBrowser::instance()->connectRemovedHandler(&SimpleTetherApp::browserDidRemoveCamera, this);
    CameraBrowser::instance()->connectEnumeratedHandler(&SimpleTetherApp::browserDidEnumerateCameras, this);
    CameraBrowser::instance()->start();
}

void SimpleTetherApp::keyDown()
{
    switch (getchar()) {
        case '\n':
            if (mCamera != NULL && mCamera->hasOpenSession()) {
                mCamera->requestTakePicture();
            }
            break;
        default:
            break;
    }
}

void SimpleTetherApp::draw()
{
    
}

#pragma mark - CAMERA BROWSER

void SimpleTetherApp::browserDidAddCamera(CameraRef camera) {
    std::cout << "added a camera: " << camera->getName() << std::endl;
    if (mCamera != NULL) {
        return;
    }

    mCamera = camera;
    mCamera->connectRemovedHandler(&SimpleTetherApp::didRemoveCamera, this);
    mCamera->connectFileAddedHandler(&SimpleTetherApp::didAddFile, this);
    std::cout << "grabbing camera: " << camera->getName() << std::endl;

    EDSDK::Camera::Settings settings = EDSDK::Camera::Settings();
//    settings.setPictureSaveLocation(kEdsSaveTo_Both);
//    settings.setShouldKeepAlive(false);
    EdsError error = mCamera->requestOpenSession(settings);
    if (error == EDS_ERR_OK) {
        std::cout << "session opened" << std::endl;
    }
}

void SimpleTetherApp::browserDidRemoveCamera(CameraRef camera) {
    // NB - somewhat redundant as the camera will notify handler first
    std::cout << "removed a camera: " << camera->getName() << std::endl;
    if (camera != mCamera) {
        return;
    }

    std::cout << "our camera was disconnected" << std::endl;
    mCamera = NULL;
}

void SimpleTetherApp::browserDidEnumerateCameras() {
    std::cout << "enumerated cameras" << std::endl;
}

#pragma mark - CAMERA

void SimpleTetherApp::didRemoveCamera(CameraRef camera) {
    std::cout << "removed a camera: " << camera->getName() << std::endl;
    if (camera != mCamera) {
        return;
    }

    std::cout << "our camera was disconnected" << std::endl;
    mCamera = NULL;
}

int main()
{
    SimpleTetherApp *app = new SimpleTetherApp();
    app->setup();
    while (1)
    {
        app->keyDown();
    }
    
    return 0;
}

void SimpleTetherApp::didAddFile(CameraRef camera, CameraFileRef file)
{
    boost::filesystem::path destinationFolderPath = boost::filesystem::path("/Users/zhao.he/Desktop/Captures");
    camera->requestDownloadFile(file, destinationFolderPath, [&](EdsError error, boost::filesystem::path outputFilePath) {
        if (error == EDS_ERR_OK) {
            std::cout << "image downloaded to '" << outputFilePath << "'" << std::endl;
        }
    });

//    camera->requestReadFile(file, [&](EdsError error, ci::Surface surface) {
//        if (error == EDS_ERR_OK) {
//            mPhotoTexture = gl::Texture(surface);
//        }
//    });
}

//CINDER_APP_NATIVE(SimpleTetherApp, RendererGl)
