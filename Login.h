#pragma once
#include "afxdialogex.h"
#include "test.h"

#define WM_CLOSE_LOGIN 2000

// Login 对话框
class CtestDoc;
class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CtestDoc* pDoc,CWnd* pParent = nullptr);   // 构造函数
	virtual ~Login();
	int m_nMode; // 0 for Server, 1 for Client
	CString m_strIPAddress;
	UINT m_uPort;

	BOOL OnInitDialog();

	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnBnClickedRadioClient();
	afx_msg LRESULT OnCloseLogin(WPARAM wParam, LPARAM lParam);//关闭对话框的函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditIp();
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	// //连接的服务器IP地址

	 // << 新增 >> 公共方法，供Doc回调
	void UpdateStatus(int connectedPlayers);
	void EnableStartButton();
	DWORD IPP;
private:
	CtestDoc* m_pDoc;
};
