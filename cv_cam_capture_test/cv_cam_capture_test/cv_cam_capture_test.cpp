// cv_cam_capture_test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

int main()
{
	cv::VideoCapture cap0(0);//デバイス(左カメラ)のオープン
	cv::VideoCapture cap1(1);//デバイス(右カメラ)のオープン

	if (!cap0.isOpened() || !cap1.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		//読み込みに失敗したときの処理
		return -1;
	}

	//cv::VideoWriter writer("videofile.mp4", CV_FOURCC_MACRO('D', 'I', 'V', 'X'), 15.0, cv::Size(854, 480), true);
	cv::VideoWriter writer("videofile.avi", CV_FOURCC_MACRO('M', 'P', 'E', 'G'), 15.0, cv::Size(854, 480), true);

	while (1)//無限ループ
	{
		{
			cv::Mat frame_left;
			cap0 >> frame_left; // get a new frame from camer
			cv::Mat frame_left_roi(frame_left, cv::Rect(640 - 427 - 1, 0, 427, 480));

			cv::Mat frame_right;
			cap1 >> frame_right; // get a new frame from camera
			cv::Mat frame_right_roi(frame_right, cv::Rect(0, 0, 427, 480));

			cv::Mat combined_img(cv::Size(854, 480), CV_8UC3);

			cv::Mat combined_left(combined_img, cv::Rect(0, 0, 427, 480));
			frame_left_roi.copyTo(combined_left);

			cv::Mat combined_right(combined_img, cv::Rect(427, 0, 427, 480));
			frame_right_roi.copyTo(combined_right);

			writer << combined_img;
			cv::imshow("window0", combined_img);//画像を表示．
		}

		int key = cv::waitKey(1);
		if (key == 113)//qボタンが押されたとき
		{
			break;//whileループから抜ける．
		}
		else if (key == 115)//sが押されたとき
		{
			//フレーム画像を保存する．
			//cv::imwrite("img.png", frame);
		}
	}
	cv::destroyAllWindows();
	return 0;
}

