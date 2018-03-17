#include <stdio.h>
#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "surf.h"

using namespace cv;

cv::RotatedRect surfTemplateMatching(cv::Mat& img_object, cv::Mat& img_scene) {
    if( !img_object.data || !img_scene.data ) {
        std::cout<< " --(!) Error reading images " << std::endl;
    }

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 300;

    SurfFeatureDetector detector( minHessian, 4, 2, true, false );

    std::vector<KeyPoint> keypoints_object, keypoints_scene;

    detector.detect( img_object, keypoints_object );
    detector.detect( img_scene, keypoints_scene );

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descriptors_object, descriptors_scene;

    extractor.compute( img_object, keypoints_object, descriptors_object );
    extractor.compute( img_scene, keypoints_scene, descriptors_scene );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );


    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_object.rows; i++ )
    { double dist = matches[i].distance;
      if( dist < min_dist ) min_dist = dist;
      if( dist > max_dist ) max_dist = dist;
    }

    //printf("-- Max dist : %f \n", max_dist );
    //printf("-- Min dist : %f \n", min_dist );

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
       { good_matches.push_back( matches[i]); }
    }
    /*
    Mat img_matches;
    drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
                 good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    */
    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( int i = 0; i < good_matches.size(); i++ )
    {
      //-- Get the keypoints from the good matches
      obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
      scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, CV_RANSAC );

    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
    obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
    std::vector<Point2f> scene_corners(4);

    std::vector<Point2f> obj_center(1);
    std::vector<Point2f> scene_center(1);
    obj_center[0] = cvPoint(img_object.cols / 2.0, img_object.rows / 2.0);

    perspectiveTransform( obj_corners, scene_corners, H);
    perspectiveTransform( obj_center, scene_center, H);

    /*
    //printf("scene center %f %f\n", scene_center[0].x, scene_center[0].y);
    line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

    imshow( "Good Matches & Object detection", img_matches );
    waitKey(0);
    */
    //printf("scene angle %f %f\n", scene_corners[3].x - scene_corners[0].x, scene_corners[3].y - scene_corners[0].y);
    float newWidth = (scene_corners[0].x - scene_corners[1].x) * (scene_corners[0].x - scene_corners[1].x) +
            (scene_corners[0].y - scene_corners[1].y) * (scene_corners[0].y - scene_corners[1].y);
    newWidth = sqrt(newWidth);
    float newHeight = (scene_corners[2].x - scene_corners[1].x) * (scene_corners[2].x - scene_corners[1].x) +
            (scene_corners[2].y - scene_corners[1].y) * (scene_corners[2].y - scene_corners[1].y);
    newHeight = sqrt(newHeight);
    Size2f newSize (newWidth, newHeight);


    return RotatedRect(scene_center[0],
                        newSize,
                        -atan2(scene_corners[3].x - scene_corners[0].x,
                            scene_corners[3].y - scene_corners[0].y) * 180.0 / CV_PI);

}
