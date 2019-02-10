
// MFCApplication1Dlg.cpp : 實作檔
//


#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
//#include "highguilh"
using namespace std;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int board_w = 11;
int board_h = 8;
int board_n = board_w * board_h;// 格子數=寬*高
int board_nx = (board_w - 1) * (board_h - 1);// 內部角個數=(寬-1)*(高-1)
CvSize board_sz = cvSize(board_w , board_h );// 內部row和column的角個數
CComboBox m_combo;
vector< vector< Point3f > > object_points1;
vector< vector< Point2f > > image_points1;
vector< vector< Point3f > > object_points2;
vector< vector< Point2f > > image_points2;
vector< vector< Point3f > > object_points3;
vector< vector< Point2f > > image_points3;
vector< vector< Point3f > > object_points4;
vector< vector< Point2f > > image_points4;
vector< vector< Point3f > > object_points5;
vector< vector< Point2f > > image_points5;
vector <Point2f> centers1;
vector <Point2f> centers2;
vector <Point2f> centers3;
vector <Point2f> centers4;
vector <Point2f> centers5;
vector <Point> points_for_click;
Point VertexLeftTop(-1, -1);
Point VertexRightDown(-1, -1);
float square_size = 4;
CvMat* intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);
CvMat* distortion_coeffs = cvCreateMat(4, 1, CV_32FC1);
// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 對話方塊

CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_combo);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)	
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMFCApplication1Dlg::OnCbnSelchangeCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CMFCApplication1Dlg::OnCbnDropdownCombo2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication1Dlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication1Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CMFCApplication1Dlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCApplication1Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 訊息處理常式

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	FILE * pNewStdin = nullptr;
	AllocConsole();
	freopen_s(&pNewStdin,"CONOUT$", "w", stdout);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Find Corners
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	/*
	GetDlgItem(IDC_EDIT1)->SetWindowTextW(L"1");
	Mat img = imread("pig.png");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	waitKey(0);
	*/

	//success load 1 image	
	/*
	Mat img = imread("CameraCalibration/1.bmp");
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	Size size(8, 11);
	vector <Point2f> centers1;
	bool sucess = findChessboardCorners(img, size, centers1, CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &
	CV_CALIB_CB_NORMALIZE_IMAGE);
	printf("%s", sucess ? "true" : "false");
	drawChessboardCorners(img, size, Mat(centers1), sucess);
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	waitKey(0);
	*/
	//
	string onename = "CameraCalibration/";
	string twoanme = ".bmp";
	string threename = "Find_Corners_";
	for (int i = 1; i <= 2; i++)
	{
		string indexxx = "ind";
		indexxx = to_string(i);
		Mat img = imread(onename + indexxx + twoanme);
		Mat gray;
		cvtColor(img, gray, CV_BGR2GRAY);
		//	Size size(board_w, board_h);
		//	vector <Point2f> centers1;
		bool found = findChessboardCorners(
			img,
			board_sz,
			centers1,
			CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &CV_CALIB_CB_NORMALIZE_IMAGE
		);
		printf("%s", found ? " true\n " : " false\n ");
		drawChessboardCorners(img, board_sz, Mat(centers1), found);
		namedWindow(threename + indexxx, WINDOW_NORMAL);

		resizeWindow(threename + indexxx, 800, 800);
		imshow(threename + indexxx, img);
		print(centers1);
	}

	waitKey(0);

}

// Intrinsic Matrices
void CMFCApplication1Dlg::OnBnClickedButton2()
{
	string onename = "CameraCalibration/";
	string twoanme = ".bmp";
	string threename = "Find_Corners_";
	//	vector <Point2f> centers2;
	Mat intrinsic_matrix;
	Mat distortion_coeffs;
	int flag = 0;
	vector< Mat > rvecs, tvecs;

	//	CvSize size = CvSize(board_w, board_h);
	for (int i = 1; i <= 2; i++)
	{

		string indexxx = "ind";
		indexxx = to_string(i);
		Mat img = imread(onename + indexxx + twoanme);
		Mat gray;
		cvtColor(img, gray, CV_BGR2GRAY);

		bool found = findChessboardCorners(
			img,
			board_sz,
			centers2,
			CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &CV_CALIB_CB_NORMALIZE_IMAGE
		);

		//	print(centers2);

		vector< Point3f > obj;

		for (int yee = 0; yee < board_h; yee++)
		{
			for (int haha = 0; haha < board_w; haha++)
			{
				obj.push_back(Point3f((float)haha * square_size, (float)yee * square_size, 0));
			}
		}

		if (found == true) {
			cout << i << ". Found Intrinsic Matrices!" << endl;
			image_points1.push_back(centers2);
			object_points1.push_back(obj);
		}

		if (found == true) {
			calibrateCamera(
				object_points1, image_points1,
				board_sz,
				intrinsic_matrix, distortion_coeffs,
				rvecs, tvecs, flag
			);
		}
		if (found == true) {
			cout << "Intrinsic" << endl << intrinsic_matrix << endl;
			//	cout << "Distortion" << endl << distortion_coeffs << endl;
			//print(rvecs);
		}


	}//for end
	waitKey(0);
}

//Distortion
void CMFCApplication1Dlg::OnBnClickedButton3()
{
	//Distortion
	// TODO: 在此加入控制項告知處理常式程式碼
	string onename = "CameraCalibration/";
	string twoanme = ".bmp";
	string threename = "Find_Corners_";
	//	vector <Point2f> centers;
	Mat intrinsic_matrix;
	Mat distortion_coeffs;
	int flag = 0;
	vector< Mat > rvecs, tvecs;

	//	CvSize size = CvSize(board_w, board_h);
	for (int i = 1; i <= 2; i++)
	{

		string indexxx = "ind";
		indexxx = to_string(i);
		Mat img = imread(onename + indexxx + twoanme);
		Mat gray;
		cvtColor(img, gray, CV_BGR2GRAY);

		bool found = findChessboardCorners(
			img,
			board_sz,
			centers3,
			CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &CV_CALIB_CB_NORMALIZE_IMAGE
		);

		//	print(centers);

		vector< Point3f > obj;

		for (int yee = 0; yee < board_h; yee++)
		{
			for (int haha = 0; haha < board_w; haha++)
			{
				obj.push_back(Point3f((float)haha * square_size, (float)yee * square_size, 0));
			}
		}

		if (found == true) {
			cout << i << ". Found Distortion Matrices!" << endl;
			image_points3.push_back(centers3);
			object_points3.push_back(obj);
		}

		if (found == true) {
			calibrateCamera(
				object_points3, image_points3,
				board_sz,
				intrinsic_matrix, distortion_coeffs,
				rvecs, tvecs, flag
			);
		}
		if (found == true) {
		//	cout << "Intrinsic" << endl << intrinsic_matrix << endl;
			cout << "Distortion" << endl << distortion_coeffs << endl;
			//print(rvecs);
		}


	}//for end
	waitKey(0);

}

//Extrincis
void CMFCApplication1Dlg::OnBnClickedButton4()
{
	CString strTemp;
	//((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowText(strTemp);	
	m_combo.GetWindowText(strTemp);
	//wcout << strTemp.GetString() << " " << endl;
	int yee;
	yee = _ttoi(strTemp);
	cout << yee << endl;
	string onename = "CameraCalibration/";
	string twoanme = ".bmp";
	string threename = "Find_Corners_";
	//	vector <Point2f> centers;
	Mat intrinsic_matrix;
	Mat distortion_coeffs;
	int flag = 0;
	vector< Mat > rvecs, tvecs;
	int i;
	i = yee;
	string indexxx = "ind";
	indexxx = to_string(i);
	Mat img = imread(onename + indexxx + twoanme);
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);

	bool found = findChessboardCorners(
		img,
		board_sz,
		centers4,
		CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &CV_CALIB_CB_NORMALIZE_IMAGE
	);

	//	print(centers);

	vector< Point3f > obj;

	for (int yee = 0; yee < board_h; yee++)
	{
		for (int haha = 0; haha < board_w; haha++)
		{
			obj.push_back(Point3f((float)haha * square_size, (float)yee * square_size, 0));
		}
	}

	if (found == true) {
		cout << i << ". found Extrinsic Matrics!" << endl;
		image_points4.push_back(centers4);
		object_points4.push_back(obj);
	}

	if (found == true) {
		calibrateCamera(
			object_points4, image_points4,
			board_sz,
			intrinsic_matrix, distortion_coeffs,
			rvecs, tvecs, flag
		);
	}
	if (found == true) {
	//	cout << "Intrinsic" << endl << intrinsic_matrix << endl;
	//	cout << "Distortion" << endl << distortion_coeffs << endl;
		//print(rvecs);
	}
	if (found == true) {
		Mat R_matrix;
		Rodrigues(rvecs[0], R_matrix);
		cout << "R_matrix" << endl << R_matrix << endl;
		Mat Result;
		hconcat(R_matrix, tvecs[0], Result);
		print(tvecs[0]);
		cout << endl;
		cout << "Extrinsic Matrics" << endl << Result << endl;
	}
	

}




void CMFCApplication1Dlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}


void CMFCApplication1Dlg::OnCbnDropdownCombo2()
{
	
}

//Argumented Reality
void CMFCApplication1Dlg::OnBnClickedButton5()
{
	//Argumented Reality
	// TODO: 在此加入控制項告知處理常式程式碼
	string onename = "CameraCalibration/";
	string twoanme = ".bmp";
	string threename = "Draw_Cube_";
	//	vector <Point2f> centers2;
	Mat intrinsic_matrix;
	Mat distortion_coeffs;
	int flag = 0;
	vector< Mat > rvecs, tvecs;
	for (int i = 1; i <= 2; i++)
	{

		string indexxx = "ind";
		indexxx = to_string(i);
		Mat img = imread(onename + indexxx + twoanme);
		Mat gray;
		cvtColor(img, gray, CV_BGR2GRAY);

		bool found = findChessboardCorners(
			img,
			board_sz,
			centers5,
			CV_CALIB_CB_ADAPTIVE_THRESH & CV_CALIB_CB_FAST_CHECK &CV_CALIB_CB_NORMALIZE_IMAGE
		);

		//	print(centers2);

		vector< Point3f > obj;

		for (int yee = 0; yee < board_h; yee++)
		{
			for (int haha = 0; haha < board_w; haha++)
			{
				obj.push_back(Point3f((float)haha * square_size, (float)yee * square_size, 0));
			}
		}

		if (found == true) {
			cout << i << ". Found Intrinsic Matrices!" << endl;
			image_points5.push_back(centers5);
			object_points5.push_back(obj);
		}

		if (found == true) {
			calibrateCamera(
				object_points5, image_points5,
				board_sz,
				intrinsic_matrix, distortion_coeffs,
				rvecs, tvecs, flag
			);
		}
		if (found == true) {
			//cout << "Intrinsic" << endl << intrinsic_matrix << endl;
			//	cout << "Distortion" << endl << distortion_coeffs << endl;
			//print(rvecs);
		}
		//Drawing start
		if (found == true)
		{
			const string x = "X";
			const string y = "Y";
			const string z = "Z";
			Mat camera_matrix = intrinsic_matrix;
			Mat dist_coeffs = distortion_coeffs;
			vector<Point2f>corners1 = centers5;
			Mat rotation_vector; // Rotation in axis-angle form
			Mat translation_vector;
			vector<Point3d> point3D;
			vector<Point2d> point2D;
			vector<Point3f>  objectPoints;
			//for (int jj = 0; jj<board_sz.height; jj++)
			//{
			//	for (int ii = 0; ii < board_sz.width; ii++)
			//	{
			//		objectPoints.push_back(Point3f(ii*square_size, jj*square_size, 0));
			//	}
			//}
			for (int jj = 0; jj < board_h; jj++)
			{
				for (int ii = 0; ii < board_w; ii++)
				{
					objectPoints.push_back(Point3f(ii*square_size, jj*square_size, 0));
				}
			}
			//below are the 3d object point(world point) to drow x , y z axis.
			point3D.push_back(Point3d(0, 0, -4.0)); //-z this point represents 10( cm or mm accrounding to BoardBoxSize unit  ) 
			point3D.push_back(Point3d(4.0, 0, 0));  //x
			point3D.push_back(Point3d(0, 4.0, 0));  //y

		 ////below are the 3d object point(world point) to drow Box.
			point3D.push_back(Point3d(12, 0, -12.0));//(x,y,z)//4
			point3D.push_back(Point3d(12, 8, -12.0));//5
			point3D.push_back(Point3d(20, 8, -12.0));//6
			point3D.push_back(Point3d(20, 0, -12.0));//7
			point3D.push_back(Point3d(24,12, -12.0));//8
			point3D.push_back(Point3d(24,20 , -12.0));//9
			point3D.push_back(Point3d(32,12 , -12.0));//10
			point3D.push_back(Point3d(32, 20, -12.0));//11
			point3D.push_back(Point3d(28, 16, -12.0));//12

			if (found)
			{

				//	cornerSubPix(img, corners1, Size(8, 8), Size(1, 1),	TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

					//	drawChessboardCorners(view, patternSize, corners1, found);

					// The following are two important funtion
				cv::solvePnP(objectPoints, corners1, camera_matrix, dist_coeffs, rotation_vector, translation_vector);//Gives you rotation_vector, translation_vector

			  //following funtion gives you point2d from point3D world point to drow them on 2d image.
				cv::projectPoints(point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, point2D);


				// following are just drowing funtion to drow object on output image.

				//Tp drow x,y z axis on image.
				cv::line(img, corners1[0], point2D[0], cv::Scalar(0, 0, 255), 3);//z
				cv::line(img, corners1[0], point2D[1], cv::Scalar(255, 0, 0), 3);//x
				cv::line(img, corners1[0], point2D[2], cv::Scalar(0, 255, 0), 3);//y


				putText(img, x, Point(point2D[1].x - 10, point2D[1].y - 10), FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 0, 0), 2);
				putText(img, y, Point(point2D[2].x - 10, point2D[2].y - 10), FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 255, 0), 2);
				putText(img, z, Point(point2D[0].x - 10, point2D[0].y - 10), FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255), 2);
				circle(img, point2D[0], 3, cv::Scalar(0, 0, 255), 4, 8, 0);
				circle(img, point2D[1], 3, cv::Scalar(255, 0, 0), 4, 8, 0);
				circle(img, point2D[2], 3, cv::Scalar(0, 255, 0), 4, 8, 0);

				// To drow box on image. It will writen for pattern size 9,6.
				//If you are using diffrent change corners1 point and point2D point. 

				cv::line(img, corners1[3], point2D[3], cv::Scalar(0, 0, 255), board_w);
				cv::line(img, corners1[25], point2D[4], cv::Scalar(0, 0, 255), board_w);
				cv::line(img, corners1[27], point2D[5], cv::Scalar(0, 0, 255), board_w);
				cv::line(img, corners1[5], point2D[6], cv::Scalar(0, 0, 255), board_w);

				cv::line(img, corners1[3], corners1[5], cv::Scalar(0, 255, 0), board_w);
				cv::line(img, corners1[5], corners1[27], cv::Scalar(0, 255, 0), board_w);
				cv::line(img, corners1[27], corners1[25], cv::Scalar(0, 255, 0), board_w);
				cv::line(img, corners1[25], corners1[3], cv::Scalar(0, 255, 0), board_w);

				cv::line(img, point2D[3], point2D[4], cv::Scalar(255, 0, 0), board_w);
				cv::line(img, point2D[4], point2D[5], cv::Scalar(255, 0, 0), board_w);
				cv::line(img, point2D[5], point2D[6], cv::Scalar(255, 0, 0), board_w);
				cv::line(img, point2D[3], point2D[6], cv::Scalar(255, 0, 0), board_w);

				//Draw Pyramid
				cv::line(img, corners1[39], corners1[41], cv::Scalar(255, 255, 0), board_w);
				cv::line(img, corners1[41], corners1[63], cv::Scalar(255, 255, 0), board_w);
				cv::line(img, corners1[63], corners1[61], cv::Scalar(255, 255, 0), board_w);
				cv::line(img, corners1[61], corners1[39], cv::Scalar(255, 255, 0), board_w);

				cv::line(img, corners1[39], point2D[11], cv::Scalar(173, 255, 47), board_w);
				cv::line(img, corners1[41], point2D[11], cv::Scalar(173, 255, 47), board_w);
				cv::line(img, corners1[63], point2D[11], cv::Scalar(173, 255, 47), board_w);
				cv::line(img, corners1[61], point2D[11], cv::Scalar(173, 255, 47), board_w);


			}

			namedWindow(threename + indexxx, WINDOW_NORMAL);

			resizeWindow(threename + indexxx, 800, 800);
			imshow(threename + indexxx, img);
			imwrite(threename + indexxx + ".jpg", img);
			cout << "point2D" << endl;
			print(point2D);
			cout << "size:" << endl;
			cout << point2D.size() << endl;
		
		}


	}//for end
	waitKey(0);
}

//Rotation, Scaling, Translation
void CMFCApplication1Dlg::OnBnClickedButton8()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString strSource1;
	CEdit *wEdit1;

	wEdit1 = (CEdit*)GetDlgItem(IDC_EDIT1);

	wEdit1->GetWindowText(strSource1); //讀取
	double yee1;
	yee1 = _tstof(strSource1);
	cout << yee1 << endl;
	//////////////////////////////////
	CString strSource2;
	CEdit *wEdit2;

	wEdit2 = (CEdit*)GetDlgItem(IDC_EDIT2);

	wEdit2->GetWindowText(strSource2); //讀取
	double yee2;
	yee2 = _tstof(strSource2);
	cout << yee2 << endl;
	///////////////////////////////////////
	CString strSource3;
	CEdit *wEdit3;

	wEdit3 = (CEdit*)GetDlgItem(IDC_EDIT3);

	wEdit3->GetWindowText(strSource3); //讀取
	double yee3;
	yee3 = _tstof(strSource3);
	cout << yee3 << endl;

	CString strSource4;
	CEdit *wEdit4;

	wEdit4= (CEdit*)GetDlgItem(IDC_EDIT4);

	wEdit4->GetWindowText(strSource4); //讀取
	double yee4;
	yee4 = _tstof(strSource4);
	cout << yee4 << endl;

	//http://monkeycoding.com/?p=605
	Mat src = imread("OriginalTransform.png");
	Mat dst1 = Mat::zeros(src.rows, src.cols, src.type());
	Mat dst2 = Mat::zeros(src.rows, src.cols, src.type());
	//設定旋轉中心、旋轉角度和縮放倍率
	Point center = Point(src.cols / 2, src.rows / 2);
	double angle = yee1;
	double scale = yee2;
	Mat rot_mat = getRotationMatrix2D(center, angle, scale);
	Mat translate_m = cv::Mat(2, 3, CV_64FC1);
	translate_m.at<double>(0, 0) = 1;
	translate_m.at<double>(0, 1) = 0;
	translate_m.at<double>(0, 2) = yee3;
	translate_m.at<double>(1, 0) = 0;
	translate_m.at<double>(1, 1) = 1;
	translate_m.at<double>(1, 2) = yee4;
	warpAffine(src, dst1, translate_m, dst1.size());
	warpAffine(dst1, dst2, rot_mat, dst2.size());
	imshow("Original Image", src);
	imshow("Rotation + Translation Image", dst2);
	waitKey(0);
}


void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}


void CMFCApplication1Dlg::OnEnChangeEdit2()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}


void CMFCApplication1Dlg::OnEnChangeEdit3()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}


void CMFCApplication1Dlg::OnEnChangeEdit4()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}

//Mouse Callback
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if (event == CV_EVENT_RBUTTONUP) {
		VertexRightDown.x = x;
		VertexRightDown.y = y;
	}
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		points_for_click.push_back(Point(x, y));		
	}

	else if (event == EVENT_RBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		VertexLeftTop.x = x;
		VertexLeftTop.y = y;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}

	else if (event == EVENT_MOUSEMOVE)
	{
	//	cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

	}
	
}

//Perspective Transformation
void CMFCApplication1Dlg::OnBnClickedButton9()
{
	points_for_click.clear();
	//https://www.opencv-srf.com/2011/11/mouse-events.html
	//http://answers.opencv.org/question/56015/getting-coordinates-for-perspective-transform-using-mousecallback/
	//http://monkeycoding.com/?p=653
	//http://answers.opencv.org/question/55298/problem-with-perspective/?answer=55301#post-id-55301
	// TODO: 在此加入控制項告知處理常式程式碼

	Mat pers = imread("OriginalPerspective.png");
	namedWindow("Original Image", 0);

	setMouseCallback("Original Image", CallBackFunc, NULL);
	
	/*
	//Draw a rectangal with mouse right button, and store the 4 pints data
	while (true) {
		if (VertexLeftTop.x == -1 && VertexRightDown.x == -1) {
			imshow("Original Image", pers);
		}
		if (VertexLeftTop.x != -1 && VertexRightDown.x != -1) {
			rectangle(pers, Rect(VertexLeftTop, VertexRightDown), Scalar(255, 0, 0), 2);
			VertexLeftTop.x = -1;
			VertexLeftTop.y = -1;
			VertexRightDown.x = -1;
			VertexRightDown.y = -1;
			cout <<"points_for_click.size()="<< points_for_click.size() << endl;
			print(points_for_click);
			cout << endl;
			imshow("Original Image", pers);
		}
		if (cvWaitKey(33) == 27) {
			break;
		}

		if (points_for_click.size() == 4)
		{
			cout << "4 points clicked" << endl;
			print(points_for_click);
			cout<<endl;
			cout  << points_for_click[0]<< endl;
			break;
		}

	} // end While
	*/
	
	while (true)// Draw a red point when the mouse click on the image
	{
		if (points_for_click.size() == 0)
		{
			imshow("Original Image", pers);
		}
		if (points_for_click.size() > 0 && points_for_click.size() <= 4)
		{
			for (int hehe = 1; hehe <= points_for_click.size(); hehe++)
			{
				circle(pers, points_for_click[hehe -1],8, Scalar(0,0,255),-1);
				
			}
		imshow("Original Image", pers);
		}

		if (points_for_click.size() == 4)
		{
			cout << "4 points clicked" << endl;
			print(points_for_click);
			cout << endl;
			cout  << points_for_click[0]<< endl;
			break;
		}
		if (cvWaitKey(33) == 27) {
			break;
		}

	}//end while
	
	
	vector<Point2f> P, Q;
	P.push_back(points_for_click[0]);
	Q.push_back(Point2f(20, 20)); // 10 pixel border on all sides

	P.push_back(points_for_click[1]);
	Q.push_back(Point2f(450, 20));

	P.push_back(points_for_click[2]);
	Q.push_back(Point2f(450, 450));

	P.push_back(points_for_click[3]);
	Q.push_back(Point2f(20, 450));

	Mat rot = cv::getPerspectiveTransform(P, Q);

	Mat result;
	cv::warpPerspective(pers, result, rot, Size(450, 450));

	imshow("result", result);
	
	waitKey(0);
	
}

//Disparity
void CMFCApplication1Dlg::OnBnClickedButton6()
{
	//
	Mat img1, img2, g1, g2;
	// TODO: 在此加入控制項告知處理常式程式碼
	img1 = imread("imL.png");
	img2 = imread("imR.png");

	//imshow("result", g1);
	Mat dispbm, dispsgbm;
	Mat dispnorm_bm, dispnorm_sgbm;
	Mat falseColorsMap, sfalseColorsMap;
	int ndisparities = 64;   /**< Range of disparity */
	int SADWindowSize = 9; /**< Size of the block window. Must be odd */
	Ptr<StereoBM> sbm = StereoBM::create(ndisparities, SADWindowSize);
	Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,    //int minDisparity
		96,     //int numDisparities
		5,      //int SADWindowSize
		600,    //int P1 = 0
		2400,   //int P2 = 0
		1,     //int disp12MaxDiff = 0
		16,     //int preFilterCap = 0
		2,      //int uniquenessRatio = 0
		20,    //int speckleWindowSize = 0
		30,     //int speckleRange = 0
		true);  //bool fullDP = false
				//-- Check its extreme values
	double minVal; double maxVal;
	
		//grab and retrieve each frames of the video sequentially 		

		imshow("Video0", img1);
		imshow("Video1", img2);
		cvtColor(img1, g1, CV_BGR2GRAY);
		cvtColor(img2, g2, CV_BGR2GRAY);

		sbm->compute(g1, g2, dispbm);
		minMaxLoc(dispbm, &minVal, &maxVal);
		dispbm.convertTo(dispnorm_bm, CV_8UC1, 255 / (maxVal - minVal));

	//	sgbm->compute(g1, g2, dispsgbm);
	//	minMaxLoc(dispsgbm, &minVal, &maxVal);
	//	dispsgbm.convertTo(dispnorm_sgbm, CV_8UC1, 255 / (maxVal - minVal));

		imshow("Result", dispnorm_bm);
	//	imshow("With L-R Disparity Check", dispnorm_sgbm);
		waitKey(0);
}

// L-R Disparity Check
void CMFCApplication1Dlg::OnBnClickedButton7()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	Mat img_1, img_2, g_1, g_2;
	// TODO: 在此加入控制項告知處理常式程式碼
	img_1 = imread("imL.png");
	img_2 = imread("imR.png");

	//imshow("result", g_1);
	Mat dispbm, dispsgbm;
	Mat dispnorm_bm, dispnorm_sgbm;
	Mat falseColorsMap, sfalseColorsMap;
	int ndisparities = 64;   /**< Range of disparity */
	int SADWindowSize = 9; /**< Size of the block window. Must be odd */
	Ptr<StereoBM> sbm = StereoBM::create(ndisparities, SADWindowSize);
	Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,    //int minDisparity
		96,     //int numDisparities
		5,      //int SADWindowSize
		600,    //int P1 = 0
		2400,   //int P2 = 0
		1,     //int disp12MaxDiff = 1
		16,     //int preFilterCap = 0
		2,      //int uniquenessRatio = 0
		20,    //int speckleWindowSize = 0
		30,     //int speckleRange = 0
		true);  //bool fullDP = false
				//-- Check its extreme values
	double minVal; double maxVal;

	//grab and retrieve each frames of the video sequentially 		

	imshow("Video_0", img_1);
	imshow("Video_1", img_2);
	cvtColor(img_1, g_1, CV_BGR2GRAY);
	cvtColor(img_2, g_2, CV_BGR2GRAY);

	sbm->compute(g_1, g_2, dispbm);
	minMaxLoc(dispbm, &minVal, &maxVal);
	dispbm.convertTo(dispnorm_bm, CV_8UC1, 255 / (maxVal - minVal));

	sgbm->compute(g_1, g_2, dispsgbm);
	minMaxLoc(dispsgbm, &minVal, &maxVal);
	dispsgbm.convertTo(dispnorm_sgbm, CV_8UC1, 255 / (maxVal - minVal));

	imshow("Without L-R Disparity Check", dispnorm_bm);
	imshow("With L-R Disparity Check", dispnorm_sgbm);

	Mat diff;	
	absdiff(dispnorm_bm, dispnorm_sgbm, diff);
	//imshow("just diff", diff);	
	add(dispnorm_sgbm, diff, diff);
	imshow("Mark Difference", diff);
	waitKey(0);
}