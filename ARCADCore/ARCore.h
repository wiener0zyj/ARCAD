#pragma once
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
////VTK
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include <vtkAutoInit.h>
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkRenderWindow.h"  
#include "vtkRenderWindowInteractor.h" 
////BOOST
#include <boost/serialization/split_free.hpp>  
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include "cvmat_serialization.h"

using namespace cv;
using namespace std;

class CameraPose {
public:
    CameraPose();
    CameraPose(const Mat & rotation, const Mat & transform);
    void setPose(const Mat & rotation, const Mat & transform);
public:
    Mat rotationMatrix = Mat::zeros(3, 3, CV_32FC1);
    Mat rvec = Mat::zeros(1, 3, CV_32FC1);
    Mat tvec = Mat::zeros(1, 3, CV_32FC1);
    bool flag = false;
};
inline CameraPose::CameraPose() {
    flag = false;
}
inline CameraPose::CameraPose(const Mat & rotation, const Mat & transform) :rvec(rotation), tvec(transform) {
    flag = true;
    Rodrigues(rotation, rotationMatrix);
}
inline void CameraPose::setPose(const Mat & rotation, const Mat & transform) {
    flag = true;
    rvec = rotation.clone();
    tvec = transform.clone();
    Rodrigues(rotation, rotationMatrix);
}

class ARCore {
public:
    ARCore();
    void init(double zyjBoardLength);
public:
    bool calibrationNow(const vector<cv::Mat> & frames);
    bool grabCalibrationFrames(string videoname, std::vector<cv::Mat> & images4calib);
    bool calcCamPoseFromFrame(const cv::Mat & frame, CameraPose & camPose);
    bool locateZYJBoard(const cv::Mat & frame, std::vector<cv::Point2d> & sortedCorners, bool progress);

public:
    void saveCalibrationResult();
    void readCalibrationResult();
    void setZYJBoard(double length);
    void drawZYJBoardCube(cv::Mat & frame, double boardLength, const CameraPose & camPose);
    void drawPoint(cv::Mat & frame, cv::Point3d original, const CameraPose & camPose, cv::Scalar pointColor = { 0,0,255 }, int thickness = 3);
    void drawPolyLine(cv::Mat & frame, vector<Vec3d> polyPoints, bool closed, const CameraPose & camPose, cv::Scalar polyColor = { 0,0,255 }, int thickness = 1);
    void drawCurveLine(cv::Mat & frame, vector<Vec3d> polyPoints, bool closed, const CameraPose & camPose, cv::Scalar polyColor = { 0,0,255 }, int thickness = 1);
    void drawCubeLine(cv::Mat & frame, cv::Point3d original, double width, double length, double height, const CameraPose & camPose, cv::Scalar lineColor = { 0,0,255 }, int thickness = 1);
    void drawCube(cv::Mat & frame, cv::Point3d original, double width, double length, double height, const CameraPose & camPose, cv::Scalar cubeColor = { 0,0,255 });
    //void drawCylinderLine(cv::Mat & frame, cv::Point3d original, double radius, double height, const CameraPose & camPose, cv::Scalar lineColor = { 0,0,255 }, int thickness = 1);
private:
    bool judgeIfRect(vector<Point> corners, vector<Point2i> contour);
    bool judgeIfOutZYJBoard(const std::vector<std::vector<cv::Point>> & contours, const vector<Vec4i> & hierarchy, int index, std::vector<int> & innerIndexRecord);
    Point getTheNearestCorner(vector<Point2d> MarkerContour, Point2d point);
    void contoursMat2Vec(const std::vector<cv::Mat> & contoursMat, std::vector<std::vector<cv::Point>> & contours, const cv::Mat & hierarchyMat, std::vector<cv::Vec4i> & hierarchy);
    // fit the line or curve based on the given points
    void polynomial_curve_fit(std::vector<cv::Point2d>& key_point, cv::Mat& A, int n = 3);
public:
    cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0)); /* the inner param matrix of camera */
    cv::Mat distCoeffs = cv::Mat(1, 5, CV_32FC1, cv::Scalar::all(0)); /* the five distortion param:k1,k2,p1,p2,k3 */
    std::vector <cv::Vec3d> zyjBoardCorner3dCoords = {};
};

struct TmpPosValue {
    int index = 0;
    double value = 0.0;
};