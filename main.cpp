#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "concave_hull.h"

using namespace std;

cv::Mat img;
vector <Point> pts;
const cv::Scalar WHITE(255,255,255);
const cv::Vec3b RED(0,0,255);

void mouseCallBack(int event, int xPos, int yPos, int flags, void* userdata){
    if  ( event != cv::EVENT_LBUTTONDOWN ){
        return;
    }
    pts.push_back (Point(xPos, yPos));
    cv::circle(img, cv::Point (xPos, yPos), 3, WHITE, 3);
    cv::imshow("aaa",img);
}
void traverse (cv::Mat& img, vector <vector<int> >& used) {
    queue <Point> q;
    q.push (Point(0,0));
    used[0][0] = true;
    while (!q.empty()) {
        Point t = q.front();
        q.pop();
        if (t.x+1 < img.cols && !used[t.y][t.x+1] && !img.at<cv::Vec3b>(t.y, t.x+1)[0]) {
            q.push (Point (t.x+1, t.y));
            used[t.y][t.x+1] = true;
        }
        if (t.x-1 >= 0 && !used[t.y][t.x-1] && !img.at<cv::Vec3b>(t.y, t.x-1)[0]) {
            q.push (Point (t.x-1, t.y));
            used[t.y][t.x-1] = true;
        }
        if (t.y+1 < img.rows && !used[t.y+1][t.x] && !img.at<cv::Vec3b>(t.y+1, t.x)[0]) {
            q.push (Point (t.x, t.y+1));
            used[t.y+1][t.x] = true;
        }
        if (t.y-1 >= 0 && !used[t.y-1][t.x] && !img.at<cv::Vec3b>(t.y-1, t.x)[0]) {
            q.push (Point (t.x, t.y-1));
            used[t.y-1][t.x] = true;
        }
    }
}

void fillPolygon (vector <Point>& pts) {
    Polygon p = concaveHull(pts, 180);
    cout << p.size() << endl;
    img = cv::Mat::zeros(500,500,CV_8UC3);
    int n = p.size();
    for (int i = 0; i < p.size(); ++i) {
        cv::line(img, cv::Point(p[i].x,p[i].y), cv::Point(p[(i+1)%n].x,p[(i+1)%n].y), WHITE, 2);
    }
    vector <vector <int> > used(img.rows, vector <int>(img.cols, false));
    traverse (img, used);
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if (!used[i][j]) {
                img.at<cv::Vec3b> (i,j) = RED;
            }
        }
    }
    for (int i = 0; i < p.size(); ++i) {
        cv::line(img, cv::Point(p[i].x,p[i].y), cv::Point(p[(i+1)%n].x,p[(i+1)%n].y), WHITE, 2);
    }
    cv::imshow("aaa",img);
}

int main()
{
    img = cv::Mat::zeros(500,500,CV_8UC3);
    cv::namedWindow("aaa");
    cv::setMouseCallback("aaa", mouseCallBack);
    cv::imshow("aaa", img);
    while (true) {
        int key = cv::waitKey(0);
        if (key == 'c') {
            img = cv::Mat::zeros(500,500,CV_8UC3);
            pts.clear();
            cv::imshow("aaa",img);
            continue;
        } else {
            fillPolygon(pts);
        }
    }
    return 0;
}

