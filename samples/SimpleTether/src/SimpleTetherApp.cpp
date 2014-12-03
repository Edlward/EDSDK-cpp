#include <boost/filesystem.hpp>
#include "EDSDKcpp.h"

using namespace EDSDK;

void browserDidAddCamera(CameraRef camera);
void browserDidRemoveCamera(CameraRef camera);
void browserDidEnumerateCameras();
void didRemoveCamera(CameraRef camera);
void didAddFile(CameraRef camera, CameraFileRef file);

CameraRef mCamera;

void setup()
{
    CameraBrowser::instance()->connectAddedHandler(&browserDidAddCamera);
    CameraBrowser::instance()->connectRemovedHandler(&browserDidRemoveCamera);
    CameraBrowser::instance()->connectEnumeratedHandler(&browserDidEnumerateCameras);
    CameraBrowser::instance()->start();
}

void takePicture()
{
    if (mCamera != NULL && mCamera->hasOpenSession()) {
        mCamera->requestTakePicture();
    }
}

void keyDown()
{
    switch (getchar()) {
        case '\n':
            takePicture();
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

void main1()
{
    setup();
    while (1)
    {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    }
}

void main2()
{
    setup();
    int numShots = 3;
    while ( numShots-- ) {
        takePicture();
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 2, false);
    }
}


int main()
{
    // idle
//    main1();
    
    // take 3 pictures with 2 seconds interval
    // (program must start after camera is turned on due to lack of non-blocking keyboard event handler)
    main2();
    
    return 0;
}
