# EDSDK-cpp
EDSDK-cpp is a C++ wrapper of Canon's camera control framework EDSDK. See Canon's [compatibility chart](http://www.usa.canon.com/cusa/consumer/standard_display/sdk_homepage#SDKQ7) for a list of supported cameras.

### Features
- Image capture to memory or on-disk
- Live view
- Camera keep-alive
- Simultaneous control of multiple cameras


### Dependencies
- [EDSDK](http://www.usa.canon.com/cusa/consumer/standard_display/sdk_homepage) (2.15)
- [Boost](http://www.boost.org/) (1.56)
- [Qt](http://qt-project.org/) (4.8.6)

Note: Qt is only to handle live view images, you may use another library to handle images.

### Install

1. Move the `EDSDK` folder into `EDSDK-cpp/lib`
2. `brew install boost --universal`
3. Open `samples/SimpleTether/SimpleTether.xcodeproj` and build the target in x86_64 or 32-bit.

### Example Code

```C++
void CaptureApp::setup() {
    CameraBrowser::instance()->connectAddedHandler(&CaptureApp::browserDidAddCamera, this);
    CameraBrowser::instance()->start();
}

void CaptureApp::browserDidAddCamera(CameraRef camera) {
    mCamera = camera;
    mCamera->connectFileAddedHandler(&CaptureApp::didAddFile, this);
    EdsError error = mCamera->requestOpenSession();
    if (error == EDS_ERR_OK) {
        mCamera->requestTakePicture();
    }
}

void CaptureApp::didAddFile(CameraRef camera, CameraFileRef file) {
    fs::path destinationFolderPath = expandPath(fs::path("~/Desktop/Captures"));
    camera->requestDownloadFile(file, destinationFolderPath, [this](EdsError error, fs::path outputFilePath) {
        if (error == EDS_ERR_OK) {
            console() << "downloaded image to: " << outputFilePath << endl;
        }
    });
}
```

*NOTE* - due to a bug introduced in EDSDK 2.10, the `CameraBrowser` will only call the camera removed handler for cameras with an open session.

### Credits

EDSDK-cpp is largely based on [Cinder-EDSDK](https://github.com/pizthewiz/Cinder-EDSDK).