// cv_cam_capture_test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

int main()
{
	cv::VideoCapture cap0(0);//�f�o�C�X(���J����)�̃I�[�v��
	cv::VideoCapture cap1(1);//�f�o�C�X(�E�J����)�̃I�[�v��

	if (!cap0.isOpened() || !cap1.isOpened())//�J�����f�o�C�X������ɃI�[�v���������m�F�D
	{
		//�ǂݍ��݂Ɏ��s�����Ƃ��̏���
		return -1;
	}

	//cv::VideoWriter writer("videofile.mp4", CV_FOURCC_MACRO('D', 'I', 'V', 'X'), 15.0, cv::Size(854, 480), true);
	cv::VideoWriter writer("videofile.avi", CV_FOURCC_MACRO('M', 'P', 'E', 'G'), 15.0, cv::Size(854, 480), true);

	while (1)//�������[�v
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
			cv::imshow("window0", combined_img);//�摜��\���D
		}

		int key = cv::waitKey(1);
		if (key == 113)//q�{�^���������ꂽ�Ƃ�
		{
			break;//while���[�v���甲����D
		}
		else if (key == 115)//s�������ꂽ�Ƃ�
		{
			//�t���[���摜��ۑ�����D
			//cv::imwrite("img.png", frame);
		}
	}
	cv::destroyAllWindows();
	return 0;
}

