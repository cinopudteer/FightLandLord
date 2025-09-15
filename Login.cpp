// Login.cpp: 实现文件
//

#include "pch.h"
#include "test.h"
#include "afxdialogex.h"
#include "Login.h"
#include "testDoc.h"

// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CtestDoc* pDoc,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_nMode(0) // 默认选择服务器
	, m_strIPAddress(_T("127.0.0.1")) // 默认IP
	, m_pDoc(pDoc)
	, IPP(0)
	, m_uPort(12345) // << 新增 >> 设置一个常用的默认端口
{
	
}

Login::~Login()
{
}

void Login::OnBnClickedRadioServer()
{
	m_nMode = 0;
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_STATIC)->SetWindowText(_T("正在等待玩家连接"));
	// 立刻开始监听
	m_pDoc->InitializeNetwork(true, _T(""), m_uPort);
}
void Login::OnBnClickedRadioClient()
{
	m_nMode = 1;
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_IP)->EnableWindow(TRUE);
	m_pDoc->InitializeNetwork(false,m_strIPAddress, m_uPort);
}

LRESULT Login::OnCloseLogin(WPARAM wParam, LPARAM lParam)
{
	// 收到来自Doc的关闭命令后，安全地关闭对话框
	EndDialog(IDOK);
	return 0;
}

BOOL Login::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDOK)->EnableWindow(FALSE); // << 修改 >> 服务器初始时“开始游戏”按钮不可用
	return TRUE;
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_uPort); // << 新增 >> 关联端口编辑框和m_uPort变量
	DDV_MinMaxUInt(pDX, m_uPort, 1024, 65535); // << 推荐 >> 添加数据验证，防止用户输入无效端口
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_IP, &Login::OnEnChangeEditIp)
	ON_BN_CLICKED(IDC_RADIO_SERVER, &Login::OnBnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &Login::OnBnClickedRadioClient)
	ON_MESSAGE(WM_CLOSE_LOGIN, &Login::OnCloseLogin)
END_MESSAGE_MAP()


// Login 消息处理程序

void Login::OnEnChangeEditIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void Login::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void Login::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	if (m_nMode == 1) // 如果是客户端
	{
		// 尝试连接，如果失败则不关闭对话框
		if (!m_pDoc->InitializeNetwork(false, m_strIPAddress, m_uPort))
		{
			AfxMessageBox(_T("连接服务器失败！\n请检查IP地址或服务器是否已启动。"));
			return; // << 关键 >> 连接失败，函数返回，对话框不关闭
		}
	}

	// 对于服务器，此按钮只有在人满后才可点击，所以直接关闭即可
	// 对于客户端，代码能执行到这里说明已经连接成功
	CDialogEx::OnOK();
}

void Login::UpdateStatus(int connectedPlayers)
{
	CString status;
	status.Format(_T("正在等待玩家连接 (%d/2)..."), connectedPlayers);
	GetDlgItem(IDC_STATIC)->SetWindowText(status);
}

void Login::EnableStartButton()
{
	GetDlgItem(IDC_STATIC)->SetWindowText(_T("所有玩家已就绪！"));
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

