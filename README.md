# EDSDK-cpp
EDSDK-cpp is a C++ wrapper of Canon's camera control framework EDSDK. See Canon's [compatibility chart](http://www.usa.canon.com/cusa/consumer/standard_display/sdk_homepage#SDKQ7) for a list of supported cameras.

### Features
Image capture to memory or on-disk, camera keep-alive, simultaneous control of multiple cameras, camera connect and disconnect handling. It does *not* offer Live View support.


### Dependencies
- [EDSDK]((http://www.usa.canon.com/cusa/consumer/standard_display/sdk_homepage) (2.14)
- [Boost](http://www.boost.org/) (1.56)

### Install

1. Move the `EDSDK` folder into `EDSDK-cpp/lib`
2. `brew install boost --universal`
3. Open `samples/SimpleTether/SimpleTether.xcodeproj` and build the target in 32-bit, as EDSDK is only in x86_64.

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