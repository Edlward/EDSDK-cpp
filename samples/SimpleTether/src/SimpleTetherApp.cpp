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


void browserDidAddCamera(CameraRef camera);
void browserDidRemoveCamera(CameraRef camera);
void browserDidEnumerateCameras();
void didRemoveCamera(CameraRef camera);
void didAddFile(CameraRef camera, CameraFileRef file);

CameraRef mCamera;
//    gl::Texture mPhotoTexture;

//void SimpleTetherApp::prepareSettings(Settings* settings) {
//    settings->enableHighDensityDisplay();
//    settings->setWindowSize(640, 480);
//}

void setup()
{
    CameraBrowser::instance()->connectAddedHandler(&browserDidAddCamera);
    CameraBrowser::instance()->connectRemovedHandler(&browserDidRemoveCamera);
    CameraBrowser::instance()->connectEnumeratedHandler(&browserDidEnumerateCameras);
    CameraBrowser::instance()->start();
}

void keyDown()
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


#pragma mark - CAMERA BROWSER

void browserDidAddCamera(CameraRef camera) {
    std::cout << "added a camera: " << camera->getName() << std::endl;
    if (mCamera != NULL) {
        return;
    }

    mCamera = camera;
    mCamera->connectRemovedHandler(&didRemoveCamera);
    mCamera->connectFileAddedHandler(&didAddFile);
    std::cout << "grabbing camera: " << camera->getName() << std::endl;

    EDSDK::Camera::Settings settings = EDSDK::Camera::Settings();
//    settings.setPictureSaveLocation(kEdsSaveTo_Both);
//    settings.setShouldKeepAlive(false);
    EdsError error = mCamera->requestOpenSession(settings);
    if (error == EDS_ERR_OK) {
        std::cout << "session opened" << std::endl;
    }
}

void browserDidRemoveCamera(CameraRef camera) {
    // NB - somewhat redundant as the camera will notify handler first
    std::cout << "removed a camera: " << camera->getName() << std::endl;
    if (camera != mCamera) {
        return;
    }

    std::cout << "our camera was disconnected" << std::endl;
    mCamera = NULL;
}

void browserDidEnumerateCameras() {
    std::cout << "enumerated cameras" << std::endl;
}

#pragma mark - CAMERA

void didRemoveCamera(CameraRef camera) {
    std::cout << "removed a camera: " << camera->getName() << std::endl;
    if (camera != mCamera) {
        return;
    }

    std::cout << "our camera was disconnected" << std::endl;
    mCamera = NULL;
}


void didAddFile(CameraRef camera, CameraFileRef file)
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


int main()
{
    setup();
    while (1)
    {
        keyDown();
    }
    
    return 0;
}

//CINDER_APP_NATIVE(SimpleTetherApp, RendererGl)
